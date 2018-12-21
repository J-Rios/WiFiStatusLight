/**************************************************************************************************/
// Project: WiFiStatusLight
// File: main.cpp
// Description: Project main file
// Created on: 16 nov. 2018
// Last modified date: 18 nov. 2018
// Version: 1.0.0
/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries


// FreeRTOS libraries
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Device libraries (ESP-IDF)
#include "sdkconfig.h"
#include <nvs_flash.h>

// Tasks implementations
#include "task_wifistatus.h"
#include "task_internetstatus.h"
#include "task_ota.h"

// Custom libraries
#include "constants.h"
#include "globals.h"
#include "commons.h"
#include "buttons.h"
#include "rgbleds.h"

/**************************************************************************************************/

/* Functions Prototypes */

extern "C" { void app_main(void); }
void system_start(Globals* Global, Buttons* Btn_OTA_Update, RGBLEDs* LED_RGB);
void nvs_init(void);
void task_creation(Globals* Global, Buttons* Btn_OTA_Update, RGBLEDs* LED_RGB);

/**************************************************************************************************/

/* Main Function */

void app_main(void)
{
    // Elements that will exists during all system live time
    Globals Global;
    Buttons Btn_OTA_Update(P_I_BTN_OTA);
    RGBLEDs LED_RGB(P_O_RGBLED_R, P_O_RGBLED_G, P_O_RGBLED_B);

    // System start and FreeRTOS task creation functions
    system_start(&Global, &Btn_OTA_Update, &LED_RGB);
    task_creation(&Global, &Btn_OTA_Update, &LED_RGB);
    
    // Keep Main "Task" running to avoid lost local scope data that has been passed to Tasks
    while(1)
        delay(1000); // vTaskDelay() Macro (See commons.h)
}

/**************************************************************************************************/

/* Functions */

// Initial system start
void system_start(Globals* Global, Buttons* Btn_OTA_Update, RGBLEDs* LED_RGB)
{
    debug("\n-------------------------------------------------------------------------------\n");
    debug("\nSystem start.\n\n");

    show_device_info();
    nvs_init();

    char fw_version[MAX_LENGTH_VERSION+1];
    Global->get_firmware_version(fw_version);
    debug("Firmware version: %s\n", fw_version);

    Btn_OTA_Update->mode(NORMAL);
    debug("Button OTA initialized.\n");

    LED_RGB->init();
    debug("RGB LED initialized.\n");
}

// Initialize Non-Volatile-Storage
void nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

// FreeRTOS Tasks creation
void task_creation(Globals* Global, Buttons* Btn_OTA_Update, RGBLEDs* LED_RGB)
{
    // Prepare parameters to pass
    static tasks_argv task_argv;
    task_argv.Global = Global;
    task_argv.Btn_OTA_Update = Btn_OTA_Update;
    task_argv.LED_RGB = LED_RGB;
    
    // Create WiFi Status Task
    if(xTaskCreate(&task_wifi_status, "task_wifi_status", TASK_WIFI_STATUS_STACK, 
                   (void*)&task_argv, tskIDLE_PRIORITY+5, NULL) != pdPASS)
    {
        debug("\nError - Can't create WiFi status task (not enough memory?)\n");
        debug("Rebooting the system...\n\n");
        esp_restart();
    }

    // Create Internet Status Task
    if(xTaskCreate(&task_internet_status, "task_internet_status", TASK_INTERNET_STACK, 
                   (void*)&task_argv, tskIDLE_PRIORITY+5, NULL) != pdPASS)
    {
        debug("\nError - Can't create Internet status task (not enough memory?)\n");
        debug("Rebooting the system...\n\n");
        esp_restart();
    }

    // Create OTA Task
    if(xTaskCreate(&task_ota, "task_ota", TASK_OTA_STACK, (void*)&task_argv, 
                   tskIDLE_PRIORITY+5, NULL) != pdPASS)
    {
        debug("\nError - Can't create OTA task (not enough memory?)\n");
        debug("Rebooting the system...\n\n");
        esp_restart();
    }
}
