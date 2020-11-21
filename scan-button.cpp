#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <syslog.h>
#include <stdarg.h>
#include "scan-button.h"
#include "sane-lib.h"

#ifdef __cplusplus
extern "C" {
#endif

// Флаг остановки
char stop = 0;
// handle устройства
SANE_Handle sane_handle = NULL;
// Директория для сканов
char * scan_path;
char debug = 0x00;

void log(int priority, const char *format, ...)
{
    va_list list;
    va_start(list, format); 
    vprintf(format, list);
    vsyslog(priority, format, list);
    va_end(list);
}

void onShutdown(int ignored)
{
    if (stop)
    {
        log(LOG_NOTICE, "Abort scan-button\n");
        sane_cancel (sane_handle);
        close_device(sane_handle);          
        sanelib_exit();
        _exit(0);
    }
    stop = 1;
    log(LOG_NOTICE, "Stop scan-button\n");
}

int open_scaner()
{
    // get all devices
    const SANE_Device ** device_list = NULL;
    SANE_Status sane_status = SANE_STATUS_GOOD;
    if (sane_status = get_devices(&device_list))
    {
        log(LOG_ERR, "Error get devices\n");
        return -1;
    }  
    
     // open first device
    log(LOG_NOTICE, "Open a device\n");    
    const SANE_Device *device = *device_list;
    if (!device) 
    {
        log(LOG_ERR, "No device connected\n");
        return -1;
    }    
 
    if (sane_status = open_device((SANE_Device *)device, &sane_handle))
    {
        log(LOG_ERR, "Can not open device\n");
        return -1;
    }
    
    return 0;
}

int wait_press_button(void)
{
    // Open device
    if (open_scaner() == -1)
    {
        return -1;
    }
    
    // Find option
    const char * option_name = "button-1";
    SANE_Int option_num = get_option_num(sane_handle, option_name);
    if (option_num == -1)
    {
        log(LOG_ERR, "Not find option %s\n", option_name);
        return -1;
    }

    log(LOG_NOTICE, "Find option %i = %s\n", option_num, option_name);

    // wait press button
    while(1)
    {
        if(stop) break;
        
        SANE_Int value = get_option_value(sane_handle, option_num);
        if(value == -1)
        {
            log(LOG_ERR, "Error read option %s\n", option_name);
            return -1;        
        }
        
        if (debug)
        {
            printf("Option value = %i\n", value);
        }
        
        if (value == 1)
        {
            break;
        }
        else
        {
            sleep(1);
        }
    }

    // close device
    sane_cancel (sane_handle);
    log(LOG_NOTICE, "Close the device\n");
    close_device(sane_handle);  
    
    return 0;  
}

int scan(void)
{
    log(LOG_NOTICE, "Start scan...");
    char command[PATH_MAX+100];
    char path[PATH_MAX];
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf (path, "%s/scan_%02d_%02d_%02d_%02d_%02d_%02d.jpeg", 
        scan_path, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    sprintf (command, "scanimage --resolution 300 --format=jpeg --mode Color > %s\n", path);
    
    log(LOG_NOTICE, "Scan save to %s", path);    
    int status = system(command);
    
    sync();
}

int main(int argc, char** argv) {
    openlog("scan-button", LOG_CONS, LOG_USER);
    if (argc == 1){
        log(LOG_ERR, "Error. Need set path of scan files\n");
        return -1;
    }
    
    scan_path = argv[1];
    
    if (argc > 2)
    {
        debug = 0x01;
    }
    
    log(LOG_NOTICE, "Start scan-button version %s. Path: %s\n", APP_VERSION, scan_path);
    
    //Подписываемся на CTRL+C, kill итд
    signal(SIGTERM, onShutdown);
    signal(SIGKILL, onShutdown);
    signal(SIGINT, onShutdown);    
    
    // initialize SANE
    log(LOG_NOTICE, "Init\n");
    init();

    while(1)
    {
        if (wait_press_button())
        {
            sleep(1);
        }
        
        if(stop) break;
        
        if (scan())
        {
            sleep(1);
        }
    }
    
    // free sane resources
    log(LOG_NOTICE, "Exit\n");    
    sanelib_exit();
    
    //Закрытие файла лога
    closelog();    
    
    return 0;
}

#ifdef __cplusplus
}
#endif 
