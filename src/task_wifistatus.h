/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_wifistatus.cpp
// Description: WiFi status FreeRTOS task file
// Created on: 17 nov. 2018
// Last modified date: 17 nov. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef WIFISTATUS_H
#define WIFISTATUS_H

/**************************************************************************************************/

/* C++ compiler compatibility */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/

/* Libraries */

#include "constants.h"
#include "esprgb.h"

/**************************************************************************************************/

/* Functions */

extern void task_wifi_status(void *pvParameter);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
