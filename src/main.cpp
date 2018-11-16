/**************************************************************************************************/
// Project: WiFiStatusLight
// File: main.cpp
// Description: Project main file
// Created on: 16 nov. 2018
// Last modified date: 16 nov. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <stdio.h>

// FreeRTOS libraries
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Device libraries (ESP-IDF)
#include "esp_spi_flash.h"
#include "driver/uart.h"
#include "sdkconfig.h"

// Custom libraries
#include "esprgb.h"

/**************************************************************************************************/

/* Defines, Macros, Constants and Types */

// I/O pins defines
#define P_O_RGBLED_R GPIO_NUM_12
#define P_O_RGBLED_G GPIO_NUM_13
#define P_O_RGBLED_B GPIO_NUM_14

// MAC Address length
#define LENGTH_MAC_ADDR 18

// Macros
#define delay(x) do { vTaskDelay(x/portTICK_PERIOD_MS); } while(0)

/**************************************************************************************************/

/* Functions Prototypes */

// Main Function
extern "C" { void app_main(void); }

// Tasks Functions
void task_rgb_test(void *pvParameter);

// Common Functions
void show_device_info(void);
void show_device_macs(void);

/**************************************************************************************************/

/* Main Function */

void app_main(void)
{
    ESPRGB LED_RGB(P_O_RGBLED_R, P_O_RGBLED_G, P_O_RGBLED_B);

    printf("\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("\nSystem start.\n\n");
    show_device_info();

    LED_RGB.init();
    printf("RGB LED initialized\n");
    
    // Create Blink Task
    if(xTaskCreate(&task_rgb_test, "task_rgb_test", configMINIMAL_STACK_SIZE+128, (void*)&LED_RGB, 
                   tskIDLE_PRIORITY+5, NULL) != pdPASS)
    {
		printf("\nError when creating blink task (not enough memory?)\n");
        printf("Rebooting the system...\n\n");
        esp_restart();
    }
    
    // Keep Main "Task" running to avoid lost scope data that has been passed to Tasks
    while(1)
    {
        delay(1000);
    }
}

/**************************************************************************************************/

/* RGB Test Task */

void task_rgb_test(void *pvParameter)
{
    printf("\nRGB task initialized.\n");

    // Get provided parameter
    ESPRGB* this_LED_RGB = (ESPRGB*)pvParameter;

    while(1)
    {
        this_LED_RGB->on(RGB_RED);
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

/* Common Functions */

// Read device info and show it through serial
void show_device_info(void)
{
    esp_chip_info_t chip_info;
    
    printf("Device Info:\n");
    printf("---------------\n");
    
    esp_chip_info(&chip_info);
    if(chip_info.model == CHIP_ESP32)
        printf("Chip Model: ESP32\n");
    else
        printf("Chip Model: Unknown\n");
    printf("Chip Revision: %d\n", chip_info.revision);
    printf("CPU Cores: %d\n", chip_info.cores);
    printf("Flash Memory: %dMB\n", spi_flash_get_chip_size()/(1024*1024));
    printf("Firmware ESP-IDF version: %s\n", esp_get_idf_version());
    if((chip_info.features & CHIP_FEATURE_WIFI_BGN) || (chip_info.features & CHIP_FEATURE_BT) ||
       (chip_info.features & CHIP_FEATURE_BLE) || (chip_info.features & CHIP_FEATURE_EMB_FLASH))
    {
        printf("Characteristics:\n");
        if(chip_info.features & CHIP_FEATURE_WIFI_BGN)
            printf("  - WiFi 2.4GHz\n");
        if(chip_info.features & CHIP_FEATURE_BT)
            printf("  - Bluetooth Classic\n");
        if(chip_info.features & CHIP_FEATURE_BLE)
            printf("  - Bluetooth Low Energy\n");
        if(chip_info.features & CHIP_FEATURE_EMB_FLASH)
            printf("  - Embedded Flash memory\n");
        else
            printf("  - External Flash memory\n");
    }

    printf("\n");
}

// Read device interfaces MAC addresses and show them through serial
void show_device_macs(void)
{
    uint8_t mac_address[LENGTH_MAC_ADDR+1];
    esp_err_t rc;

    // Read device MAC for WiFi Station Interface
    rc = esp_read_mac(mac_address, ESP_MAC_WIFI_STA);
    if(rc != ESP_OK)
        printf("Error when reading device ESP_MAC_WIFI_STA MAC address - %s\n\n", esp_err_to_name(rc));
    else
        printf("WiFi Station MAC: %s\n", mac_address);
    
    // Read device MAC for WiFi Software Access Point Interface
    rc = esp_read_mac(mac_address, ESP_MAC_WIFI_SOFTAP);
    if(rc != ESP_OK)
        printf("Error when reading device ESP_MAC_WIFI_SOFTAP MAC address - %s\n\n", esp_err_to_name(rc));
    else
        printf("WiFi SoftAP MAC: %s\n", mac_address);
    
    // Read device MAC for WiFi Software Access Point Interface
    rc = esp_read_mac(mac_address, ESP_MAC_ETH);
    if(rc != ESP_OK)
        printf("Error when reading device ESP_MAC_ETH MAC address - %s\n\n", esp_err_to_name(rc));
    else
        printf("Ethernet MAC: %s\n", mac_address);
    
    // Read device MAC for WiFi Software Access Point Interface
    rc = esp_read_mac(mac_address, ESP_MAC_BT);
    if(rc != ESP_OK)
        printf("Error when reading device ESP_MAC_BT MAC address - %s\n\n", esp_err_to_name(rc));
    else
        printf("Bluetooth MAC: %s\n", mac_address);
}
