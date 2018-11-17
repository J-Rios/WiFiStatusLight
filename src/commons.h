/**************************************************************************************************/
// Project: WiFiStatusLight
// File: commons.h
// Description: Commons functions file
// Created on: 17 nov. 2018
// Last modified date: 17 nov. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef COMMONS_H
#define COMMONS_H

/**************************************************************************************************/

/* C++ compiler compatibility */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/

/* Libraries */

// FreeRTOS libraries
//#include <freertos/FreeRTOS.h>

// Device libraries (ESP-IDF)
#include <esp_system.h>
#include <esp_spi_flash.h>

// Custom libraries
#include "constants.h"

/**************************************************************************************************/

/* Functions */

extern void show_device_info(void);
extern void show_device_macs(void);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
