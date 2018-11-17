/**************************************************************************************************/
// Project: WiFiStatusLight
// File: commons.cpp
// Description: Commons functions file
// Created on: 17 nov. 2018
// Last modified date: 17 nov. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Libraries */

#include "commons.h"

/**************************************************************************************************/

/* Data Types */


/**************************************************************************************************/

/* Functions */

// Read device info and show it through serial
void show_device_info(void)
{
    esp_chip_info_t chip_info;
    
    debug("Device Info:\n");
    debug("---------------\n");
    
    esp_chip_info(&chip_info);
    if(chip_info.model == CHIP_ESP32)
        debug("Chip Model: ESP32\n");
    else
        debug("Chip Model: Unknown\n");
    debug("Chip Revision: %d\n", chip_info.revision);
    debug("CPU Cores: %d\n", chip_info.cores);
    debug("Flash Memory: %dMB\n", spi_flash_get_chip_size()/(1024*1024));
    debug("Firmware ESP-IDF version: %s\n", esp_get_idf_version());
    if((chip_info.features & CHIP_FEATURE_WIFI_BGN) || (chip_info.features & CHIP_FEATURE_BT) ||
       (chip_info.features & CHIP_FEATURE_BLE) || (chip_info.features & CHIP_FEATURE_EMB_FLASH))
    {
        debug("Characteristics:\n");
        if(chip_info.features & CHIP_FEATURE_WIFI_BGN)
            debug("  - WiFi 2.4GHz\n");
        if(chip_info.features & CHIP_FEATURE_BT)
            debug("  - Bluetooth Classic\n");
        if(chip_info.features & CHIP_FEATURE_BLE)
            debug("  - Bluetooth Low Energy\n");
        if(chip_info.features & CHIP_FEATURE_EMB_FLASH)
            debug("  - Embedded Flash memory\n");
        else
            debug("  - External Flash memory\n");
    }

    debug("\n");
}

// Read device interfaces MAC addresses and show them through serial
void show_device_macs(void)
{
    uint8_t mac_address[MAX_LENGTH_MAC_ADDR+1];
    esp_err_t rc;

    // Read device MAC for WiFi Station Interface
    rc = esp_read_mac(mac_address, ESP_MAC_WIFI_STA);
    if(rc != ESP_OK)
        debug("Error when reading ESP_MAC_WIFI_STA MAC address - %s\n\n", esp_err_to_name(rc));
    else
        debug("WiFi Station MAC: %s\n", mac_address);
    
    // Read device MAC for WiFi Software Access Point Interface
    rc = esp_read_mac(mac_address, ESP_MAC_WIFI_SOFTAP);
    if(rc != ESP_OK)
        debug("Error when reading ESP_MAC_WIFI_SOFTAP MAC address - %s\n\n", esp_err_to_name(rc));
    else
        debug("WiFi SoftAP MAC: %s\n", mac_address);
    
    // Read device MAC for Ethernet Interface
    rc = esp_read_mac(mac_address, ESP_MAC_ETH);
    if(rc != ESP_OK)
        debug("Error when reading ESP_MAC_ETH MAC address - %s\n\n", esp_err_to_name(rc));
    else
        debug("Ethernet MAC: %s\n", mac_address);
    
    // Read device MAC for Bluetooth Interface
    rc = esp_read_mac(mac_address, ESP_MAC_BT);
    if(rc != ESP_OK)
        debug("Error when reading ESP_MAC_BT MAC address - %s\n\n", esp_err_to_name(rc));
    else
        debug("Bluetooth MAC: %s\n", mac_address);
}
