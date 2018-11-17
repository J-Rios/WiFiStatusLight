/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_wifistatus.cpp
// Description: WiFi status FreeRTOS task file
// Created on: 17 nov. 2018
// Last modified date: 17 nov. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Libraries */

#include "task_wifistatus.h"

/**************************************************************************************************/

/* Data Types */


/**************************************************************************************************/

/* Task */

// Check for WiFi network changes (connection/disconnection/get IP/ lost IP) and show that status 
// through RGB LED colors
void task_wifi_status(void *pvParameter)
{
    debug("\nWiFi status task initialized.\n");

    // Get provided parameter
    ESPRGB* this_LED_RGB = (ESPRGB*)pvParameter;

    while(1)
    {
        this_LED_RGB->on(RGB_RED);
        delay(1000);
        this_LED_RGB->toggle(RGB_RED);
        delay(1000);
        this_LED_RGB->on(RGB_GREEN);
        delay(1000);
        this_LED_RGB->on(RGB_BLUE);
        delay(1000);
        this_LED_RGB->off(RGB_BLUE);
        delay(1000);
        this_LED_RGB->on(RGB_RED, false);
        delay(1000);
        this_LED_RGB->on(RGB_GREEN, false);
        delay(1000);
        this_LED_RGB->on(RGB_BLUE, false);
        delay(1000);
        this_LED_RGB->off();
        delay(1000);
        this_LED_RGB->on();
        delay(1000);
    }
}

/**************************************************************************************************/

/* Functions */

