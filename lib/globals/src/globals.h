/**************************************************************************************************/
// Project: WiFiStatusLight
// File: globals.h
// Description: Library to thread-safe access encapsulated globals elements from FreeRTOS Tasks.
// Created on: 17 nov. 2018
// Last modified date: 17 nov. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef GLOBALS_H_
#define GLOBALS_H_

/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <string.h> // memset()
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

    // Volatile Parameters
    bool wifi_connected;
    bool wifi_has_ip;
} gdata;

/**************************************************************************************************/

class Globals
{
    public:
        Globals(void);

        bool get_wifi_ssid(char* to_get);
        bool set_wifi_ssid(const char* to_set);

        bool get_wifi_pass(char* to_get);
        bool set_wifi_pass(const char* to_set);

        bool get_wifi_connected(bool& to_get);
        bool set_wifi_connected(const bool to_set);

        bool get_wifi_has_ip(bool& to_get);
        bool set_wifi_has_ip(const bool to_set);

    private:
        // Mutex
        SemaphoreHandle_t mutex;

        // Globals elements
        gdata data;
};

/**************************************************************************************************/

#endif
