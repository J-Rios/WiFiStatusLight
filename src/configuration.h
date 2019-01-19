/**************************************************************************************************/
// Project: WiFiStatusLight
// File: configuration.h
// Description: Device configuration and persistent parameters save/load functions file
// Created on: 25 dec. 2018
// Last modified date: 30 dec. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/**************************************************************************************************/

/* C++ compiler compatibility */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/

/* Libraries */

// FreeRTOS libraries


// Device libraries (ESP-IDF)
#include <cJSON.h>

// Custom libraries
#include "constants.h"
#include "globals.h"
#include "commons.h"
#include "simplespiffs.h"

/**************************************************************************************************/

/* Functions */

extern void device_config_init(SimpleSPIFFS* SPIFFS, Globals* Global);
extern uint8_t get_json_str_default_config(cJSON* json_default_config, char* cstr_json);
extern bool load_device_data(Globals* Global, cJSON* json_actual_config);
extern bool spiffs_save_parameter_char(const char* parameter, const char* value);
extern bool spiffs_save_parameter_bool(const char* parameter, const bool value);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
