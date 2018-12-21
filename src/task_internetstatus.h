/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_internetstatus.cpp
// Description: Internet status FreeRTOS task file
// Created on: 21 dec. 2018
// Last modified date: 21 dec. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef INTERNETSTATUS_H
#define INTERNETSTATUS_H

/**************************************************************************************************/

/* C++ compiler compatibility */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <string.h>

// FreeRTOS libraries
//#include <freertos/FreeRTOS.h>
//#include <freertos/event_groups.h>

// Device libraries (ESP-IDF)
/*#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>*/

// Custom libraries
#include "constants.h"
#include "globals.h"
#include "commons.h"

/**************************************************************************************************/

/* Functions */

extern void task_internet_status(void *pvParameter);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
