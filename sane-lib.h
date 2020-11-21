#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sane/sane.h>
#include <sane/saneopts.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif


#ifndef SANE_LIB_H
#define SANE_LIB_H

#define OUTPUT_PNM      0
#define OUTPUT_TIFF     1
#define OUTPUT_PNG      2
#define OUTPUT_JPEG     3

#ifdef __cplusplus
extern "C" {
#endif

// Initialize SANE
void init();
// Get all devices
SANE_Status get_devices(const SANE_Device ***device_list);
// Open a device
SANE_Status open_device(SANE_Device *device, SANE_Handle *sane_handle);
// Close SANE device
void close_device(SANE_Handle sane_handle);
// Release SANE resources
void sanelib_exit();
// Get Option num
SANE_Int get_option_num(SANE_Handle sane_handle, const char * option_name);
// get option value
SANE_Int get_option_value(SANE_Handle sane_handle, SANE_Int option_num);

#ifdef __cplusplus
}
#endif 


#endif //SANE_LIB_H
