/**************************************************************************************************/
// Project: WiFiStatusLight
// File: provision.h
// Description: System provisioning functinality based in WiFi AP and WebServer
// Created on: 31 dec. 2018
// Last modified date: 01 jan. 2019
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef PROVISION_H
#define PROVISION_H

/**************************************************************************************************/

/* C++ compiler compatibility */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/

/* Libraries */

// FreeRTOS libraries


// Device libraries (ESP-IDF)
#include <nvs_flash.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>

// Custom libraries
#include "constants.h"
#include "globals.h"
#include "commons.h"

/**************************************************************************************************/

/* Functions */

extern void launch_provision(Globals* Global);

extern void wifi_start_ap(const char* ap_ssid, const char* ap_pass);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
