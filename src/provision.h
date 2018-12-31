/**************************************************************************************************/
// Project: WiFiStatusLight
// File: provision.h
// Description: System provisioning functinality based in WiFi AP and WebServer
// Created on: 31 dec. 2018
// Last modified date: 31 dec. 2018
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


// Custom libraries
#include "constants.h"
#include "globals.h"
#include "commons.h"

/**************************************************************************************************/

/* Functions */

extern void launch_provision(Globals* Global);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
