/**************************************************************************************************/
// Project: WiFiStatusLight
// File: globals.h
// Description: Library to thread-safe access encapsulated globals elements from FreeRTOS Tasks.
// Created on: 17 nov. 2018
// Last modified date: 22 dec. 2018
// Version: 1.0.0
/**************************************************************************************************/

/* Include Guard */

#ifndef GLOBALS_H_
#define GLOBALS_H_

/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <string.h> // memset(), memcpy()
#include <stdio.h> // sprintf

// Include SDK and Freertos
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// Custom libraries
#include "constants.h"

/**************************************************************************************************/

/* Globals Elements Data Type */

typedef struct
{
    // Persistent Parameters
    char wifi_ssid[MAX_LENGTH_WIFI_SSID+1];
    char wifi_pass[MAX_LENGTH_WIFI_PASS+1];
    char internet_check_url[MAX_LENGTH_IPV4+1];
    char firmware_version[MAX_LENGTH_VERSION+1];
    bool first_boot_provision;

    // Volatile Parameters
    bool start_ap;
    bool start_sta;
    bool wifi_connected;
    bool wifi_has_ip;
    bool internet_connection;
    bool ota_update;
    char wifi_ip[MAX_LENGTH_IPV4+1];
} gdata;

/**************************************************************************************************/

class Globals
{
    public:
        Globals(void);

        bool get_firmware_version(char* to_get);
        bool set_firmware_version(const char* to_set);
        
        bool get_wifi_ssid(char* to_get);
        bool set_wifi_ssid(const char* to_set);

        bool get_wifi_pass(char* to_get);
        bool set_wifi_pass(const char* to_set);

        bool get_internet_check_url(char* to_get);
        bool set_internet_check_url(const char* to_set);

        bool get_first_boot_provision(bool& to_get);
        bool set_first_boot_provision(const bool to_set);

        bool get_start_ap(bool& to_get);
        bool set_start_ap(const bool to_set);

        bool get_start_sta(bool& to_get);
        bool set_start_sta(const bool to_set);

        bool get_wifi_connected(bool& to_get);
        bool set_wifi_connected(const bool to_set);

        bool get_wifi_has_ip(bool& to_get);
        bool set_wifi_has_ip(const bool to_set);

        bool get_internet_connection(bool& to_get);
        bool set_internet_connection(const bool to_set);

        bool get_ota_update(bool& to_get);
        bool set_ota_update(const bool to_set);

        bool get_wifi_ip(char* to_get);
        bool set_wifi_ip(const char* to_set);

    private:
        // Mutex
        SemaphoreHandle_t this_mutex;

        // Globals elements
        gdata data;
};

/**************************************************************************************************/

#endif
