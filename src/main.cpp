/**************************************************************************************************/
// Project: WiFiStatusLight
// File: main.cpp
// Description: Project main file
// Created on: 16 nov. 2018
// Last modified date: 17 nov. 2018
// Version: 1.0.0
/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <stdio.h>

// FreeRTOS libraries
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Device libraries (ESP-IDF)
#include "sdkconfig.h"

// Custom libraries
#include "constants.h"
#include "commons.h"
#include "task_wifistatus.h" 
#include "esprgb.h"

/**************************************************************************************************/

/* Functions Prototypes */

extern "C" { void app_main(void); }
void system_start(ESPRGB* LED_RGB);
void task_creation(ESPRGB* LED_RGB);

/**************************************************************************************************/

/* Main Function */

void app_main(void)
{
    // Elements that will exists during all system live time
    ESPRGB LED_RGB(P_O_RGBLED_R, P_O_RGBLED_G, P_O_RGBLED_B);

    // System start and FreeRTOS task creation functions
    system_start(&LED_RGB);
    task_creation(&LED_RGB);
    
    // Keep Main "Task" running to avoid lost local scope data that has been passed to Tasks
    while(1)
        delay(1000);
}

/**************************************************************************************************/

/* Functions */

// Initial system start
void system_start(ESPRGB* LED_RGB)
{
    debug("\n-------------------------------------------------------------------------------\n");
    debug("\nSystem start.\n\n");

    show_device_info();

    LED_RGB->init();
    debug("RGB LED initialized.\n");
}

// FreeRTOS Tasks creation
void task_creation(ESPRGB* LED_RGB)
{
    // Create Blink Task
    if(xTaskCreate(&task_wifi_status, "task_wifi_status", configMINIMAL_STACK_SIZE+4096, 
                   (void*)LED_RGB, tskIDLE_PRIORITY+5, NULL) != pdPASS)
    {
		debug("\nError when creating wifi status task (not enough memory?)\n");
        debug("Rebooting the system...\n\n");
        esp_restart();
    }
}
