/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_internetstatus.cpp
// Description: Internet status FreeRTOS task file
// Created on: 21 dec. 2018
// Last modified date: 21 dec. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Libraries */

#include "task_internetstatus.h"

/**************************************************************************************************/

/* Task */

// Check for Internet connection by periodic ICMP ping to Google DNS (8.8.8.8)
void task_internet_status(void *pvParameter)
{
    bool wifi_connected = false;
    bool wifi_has_ip = false;

    // Get provided parameters
    tasks_argv* task_argv = (tasks_argv*)pvParameter;
    Globals* Global = task_argv->Global;

    debug("\nInternet status task initialized.\n");

    while(1)
    {
        // Check for actual WiFi status
        Global->get_wifi_connected(wifi_connected);
        Global->get_wifi_has_ip(wifi_has_ip);
        if(wifi_connected && wifi_has_ip)
        {
            // Ping and wait response
            bool internet_conn;
            Global->get_internet_connection(internet_conn);
            // To-Do
            // ...
            internet_conn = !internet_conn; // To-Rm
            Global->set_internet_connection(internet_conn);

            // Wait T_INTERNET_CHECK seconds until next internet check
            delay(T_INTERNET_CHECK);
        }

        // Task CPU release
        delay(100);
    }
}

/**************************************************************************************************/

/* Functions */

