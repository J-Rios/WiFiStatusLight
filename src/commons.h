/**************************************************************************************************/
// Project: WiFiStatusLight
// File: commons.h
// Description: Commons functions file
// Created on: 17 nov. 2018
// Last modified date: 18 nov. 2018
// Version: 1.0.0
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
#include "globals.h"
#include "buttons.h"
#include "rgbleds.h"

/**************************************************************************************************/

/* Macros */

// Debug macro
#define debug(...) do { if(DEBUG) printf(__VA_ARGS__); } while (0)

// FreeRTOS to Arduino like delay() macro
#define delay(x) do { vTaskDelay(x/portTICK_PERIOD_MS); } while(0)

/**************************************************************************************************/

/* FreeRTOS Tasks Arguments Struct */

typedef struct 
{
    Globals* Global;
    Buttons* Btn_OTA_Update;
    RGBLEDs* LED_RGB;
} tasks_argv;

/**************************************************************************************************/

/* Functions */

extern void show_device_info(void);
extern void show_device_config(Globals* Global);
extern char* esp_get_base_mac(void);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
