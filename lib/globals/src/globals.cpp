/**************************************************************************************************/
// Project: WiFiStatusLight
// File: globals.h
// Description: Library to thread-safe access encapsulated globals elements from FreeRTOS Tasks.
// Created on: 17 nov. 2018
// Last modified date: 17 nov. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Libraries */

#include "globals.h"

/**************************************************************************************************/

/* Globals Access Mutex Macro */

#define SAFE(x) do \
{ \
    if(xSemaphoreTake(mutex, (portTickType)10)==pdTRUE) \
    { \
        x; \
        xSemaphoreGive(mutex); \
        return 1; \
    } \
    return 0; \
} while (0)

/**************************************************************************************************/

/* Constructor */

// ESPRGB constructor, get the GPIO pins numbers that going to be used for RGB
Globals::Globals(void)
{
    // Initialize mutex
	mutex = xSemaphoreCreateMutex();

    // Boolean data initialization
    data.wifi_connected = false;
    data.wifi_has_ip = false;

    // Arrays data initialization
    memcpy(data.wifi_ssid, DEFAULT_WIFI_SSID, MAX_LENGTH_WIFI_SSID);
    memcpy(data.wifi_pass, DEFAULT_WIFI_PASS, MAX_LENGTH_WIFI_PASS);
}

/**************************************************************************************************/

/* Getters and Setters */

bool Globals::get_wifi_ssid(char* to_get)
{
    SAFE( memcpy(to_get, data.wifi_ssid, MAX_LENGTH_WIFI_SSID+1) );
}

bool Globals::set_wifi_ssid(const char* to_set)
{
    SAFE( memcpy(data.wifi_ssid, to_set, MAX_LENGTH_WIFI_SSID+1) );
}

/**************************************************************************************************/

bool Globals::get_wifi_pass(char* to_get)
{
    SAFE( memcpy(to_get, data.wifi_pass, MAX_LENGTH_WIFI_PASS+1) );
}

bool Globals::set_wifi_pass(const char* to_set)
{
    SAFE( memcpy(data.wifi_pass, to_set, MAX_LENGTH_WIFI_PASS+1) );
}

/**************************************************************************************************/

bool Globals::get_wifi_connected(bool& to_get)
{
    SAFE( to_get = data.wifi_connected );
}

bool Globals::set_wifi_connected(const bool to_set)
{
    SAFE( data.wifi_connected = to_set );
}

/**************************************************************************************************/

bool Globals::get_wifi_has_ip(bool& to_get)
{
    SAFE( to_get = data.wifi_has_ip );
}

bool Globals::set_wifi_has_ip(const bool to_set)
{
    SAFE( data.wifi_has_ip = to_set );
}

/**************************************************************************************************/
