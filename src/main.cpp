/**************************************************************************************************/
// Project: WiFiStatusLight
// File: main.cpp
// Description: Project main file
// Created on: 16 nov. 2018
// Last modified date: 01 jan. 2019
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
#include "task_device_config.h"
#include "task_wifistatus.h"
#include "task_internetstatus.h"
#include "task_ota.h"

// Custom libraries
#include "constants.h"
#include "globals.h"
#include "commons.h"
#include "configuration.h"
#include "simplespiffs.h"
#include "buttons.h"
#include "rgbleds.h"

/**************************************************************************************************/

/* Functions Prototypes */

extern "C" { void app_main(void); }
void system_start(Globals* Global, SimpleSPIFFS* SPIFFS, Buttons* Btn_OTA_Update, 
                  Buttons* Btn_AP_Conf, RGBLEDs* LED_RGB);
void nvs_init(void);
void wifi_init(void);
void task_creation(Globals* Global, Buttons* Btn_AP_Conf, Buttons* Btn_OTA_Update, 
                   RGBLEDs* LED_RGB);

/**************************************************************************************************/

/* Main Function */

void app_main(void)
{
    // Elements that will exists during all system live time
    Globals Global;
    SimpleSPIFFS SPIFFS;
    Buttons Btn_OTA_Update(P_I_BTN_OTA);
    Buttons Btn_AP_Conf(P_I_BTN_AP_CONF);
    RGBLEDs LED_RGB(P_O_RGBLED_R, P_O_RGBLED_G, P_O_RGBLED_B);

    // System start and FreeRTOS task creation functions
    system_start(&Global, &SPIFFS, &Btn_OTA_Update, &Btn_AP_Conf, &LED_RGB);
    task_creation(&Global, &Btn_AP_Conf, &Btn_OTA_Update, &LED_RGB);
    
    // Keep Main "Task" running to avoid lost local scope data that has been passed to Tasks
    while(1)
        delay(1000); // vTaskDelay() Macro (See commons.h)
}

/**************************************************************************************************/

/* Functions */

// Initial system start
void system_start(Globals* Global, SimpleSPIFFS* SPIFFS, Buttons* Btn_OTA_Update, 
                  Buttons* Btn_AP_Conf, RGBLEDs* LED_RGB)
{
    debug("\n-------------------------------------------------------------------------------\n");
    debug("\nSystem start.\n\n");

    // Non-Volatile-Storage and WiFi interface initialization
    nvs_init();
    wifi_init();

    // Mount SPIFFS and create/load persistent config file
    debug("Mounting SPIFFS FileSystem...\n");
    if(SPIFFS->mount())
    {
        debug("[OK] SPIFFS successfully mounted.\n");
        device_config_init(SPIFFS, Global);
    }

    show_device_info();
    show_device_config(Global);

    Btn_OTA_Update->mode(NORMAL);
    debug("Button OTA initialized.\n");

    Btn_AP_Conf->mode(NORMAL);
    debug("Button AP Configuration provisioning initialized.\n");

    LED_RGB->init();
    debug("RGB LED initialized.\n");
}

// Init WiFi interface
void wifi_init(void)
{
    static wifi_init_config_t cfg;

    debug("Initializing TCP-IP adapter...\n");

    tcpip_adapter_init();

    cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    esp_wifi_set_mode(WIFI_MODE_APSTA);

    debug("TCP-IP adapter successfuly initialized.\n");
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
void task_creation(Globals* Global, Buttons* Btn_AP_Conf, Buttons* Btn_OTA_Update, RGBLEDs* LED_RGB)
{
    // Prepare parameters to pass
    static tasks_argv task_argv;
    task_argv.Global = Global;
    task_argv.Btn_AP_Conf = Btn_AP_Conf;
    task_argv.Btn_OTA_Update = Btn_OTA_Update;
    task_argv.LED_RGB = LED_RGB;

    // Create Device Provision-Configuration Task
    /*if(xTaskCreate(&task_device_config, "task_device_config", TASK_DEVICE_CONF_STACK, 
                   (void*)&task_argv, tskIDLE_PRIORITY+5, NULL) != pdPASS)
    {
        debug("\nError - Can't create device configuration task (not enough memory?)\n");
        debug("Rebooting the system...\n\n");
        esp_restart();
    }
    delay(1000);*/
    
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
