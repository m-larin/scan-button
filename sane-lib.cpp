#include "sane-lib.h"

#ifdef __cplusplus
extern "C" {
#endif


// Initialize SANE
void init()
{
    SANE_Int version_code = 0;
    sane_init (&version_code, NULL);
    printf("SANE version code: %d\n", version_code);
}

// Get all devices
SANE_Status get_devices(const SANE_Device ***device_list)
{
    printf("Get all devices...\n");
    SANE_Status sane_status = SANE_STATUS_GOOD;
    if (sane_status = sane_get_devices (device_list, SANE_FALSE))
    {
        printf("sane_get_devices status: %s\n", sane_strstatus(sane_status));
    }   
    return sane_status;
}

// Open a device
SANE_Status open_device(SANE_Device *device, SANE_Handle *sane_handle)
{
    SANE_Status sane_status = SANE_STATUS_GOOD;
    printf("Name: %s, vendor: %s, model: %s, type: %s\n", device->name, device->model, device->vendor, device->type);
    if (sane_status = sane_open(device->name, sane_handle))
    {
        printf("sane_open status: %s\n", sane_strstatus(sane_status));
    }

    return sane_status;
}

// Close SANE device
void close_device(SANE_Handle sane_handle)
{
    sane_close(sane_handle);
}

// Release SANE resources
void sanelib_exit()
{
    sane_exit();
}

SANE_Int get_option_num(SANE_Handle sane_handle, const char * option_name)
{
    SANE_Int option_num = 0;
    while(1)
    {
        const SANE_Option_Descriptor * option_descriptor;        
        option_descriptor = sane_get_option_descriptor (sane_handle, option_num);    
        if (option_descriptor == NULL)
        {
            return -1;
        }
        
        if (option_descriptor->name != NULL && strcmp(option_name, option_descriptor->name) == 0){
            return option_num;
        }
        
        option_num++;
    }
}

SANE_Int get_option_value(SANE_Handle sane_handle, SANE_Int option_num)
{
    SANE_Int info;
    SANE_Int value;
    SANE_Status sane_status = sane_control_option (sane_handle, option_num,
                             SANE_ACTION_GET_VALUE, &value, &info);
    if (sane_status == SANE_STATUS_GOOD)
    {
        return value;
    }
    else
    {
        return -1;
    }    
}

#ifdef __cplusplus
}
#endif 
