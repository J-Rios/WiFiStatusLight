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
    char firmware_version[MAX_LENGTH_VERSION+1];
    char wifi_ssid[MAX_LENGTH_WIFI_SSID+1];
    char wifi_pass[MAX_LENGTH_WIFI_PASS+1];

    // Volatile Parameters
    bool wifi_connected;
    bool wifi_has_ip;
    bool internet_connection;
    bool ota_update;
    uint8_t device_mac[MAX_LENGTH_MAC_ADDR+1];
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

        bool get_wifi_connected(bool& to_get);
        bool set_wifi_connected(const bool to_set);

        bool get_wifi_has_ip(bool& to_get);
        bool set_wifi_has_ip(const bool to_set);

        bool get_internet_connection(bool& to_get);
        bool set_internet_connection(const bool to_set);

        bool get_ota_update(bool& to_get);
        bool set_ota_update(const bool to_set);

        bool get_device_mac(uint8_t* to_get);
        bool set_device_mac(const uint8_t* to_set);

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
