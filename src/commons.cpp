/**************************************************************************************************/
// Project: WiFiStatusLight
// File: commons.cpp
// Description: Commons functions file
// Created on: 17 nov. 2018
// Last modified date: 18 nov. 2018
// Version: 1.0.0
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
    esp_chip_info(&chip_info);

    debug("Device Info:\n");
    debug("---------------\n");
    if(chip_info.model == CHIP_ESP32)
        debug("Chip Model: ESP32\n");
    else
        debug("Chip Model: Unknown\n");
    debug("Chip Revision: %d\n", chip_info.revision);
    debug("CPU Cores: %d\n", chip_info.cores);
    debug("Flash Memory: %dMB\n", spi_flash_get_chip_size()/(1024*1024));
    if(chip_info.features & CHIP_FEATURE_WIFI_BGN)
    {
        char* mac = esp_get_base_mac();
        debug("Base MAC: %s\n", mac);
    }
    debug("Firmware ESP-IDF version: %s\n", esp_get_idf_version());
    if((chip_info.features & CHIP_FEATURE_WIFI_BGN) || (chip_info.features & CHIP_FEATURE_BT) ||
       (chip_info.features & CHIP_FEATURE_BLE) || (chip_info.features & CHIP_FEATURE_EMB_FLASH))
    {
        debug("Characteristics:\n");
        if(chip_info.features & CHIP_FEATURE_WIFI_BGN)
            debug("    WiFi 2.4GHz\n");
        if(chip_info.features & CHIP_FEATURE_BT)
            debug("    Bluetooth Classic\n");
        if(chip_info.features & CHIP_FEATURE_BLE)
            debug("    Bluetooth Low Energy\n");
        if(chip_info.features & CHIP_FEATURE_EMB_FLASH)
            debug("    Embedded Flash memory\n");
        else
            debug("    External Flash memory\n");
    }
    debug("\n\n");
}

// Read device actual configuration and show it through serial
void show_device_config(Globals* Global)
{
    char wifi_ssid[MAX_LENGTH_WIFI_SSID+1];
    char wifi_pass[MAX_LENGTH_WIFI_PASS+1];
    char internet_check_url[MAX_LENGTH_IPV4+1];
    char firmware_version[MAX_LENGTH_VERSION+1];

    Global->get_wifi_ssid(wifi_ssid);
    Global->get_wifi_pass(wifi_pass);
    Global->get_internet_check_url(internet_check_url);
    Global->get_firmware_version(firmware_version);

    debug("Actual Device Configuration Parameters:\n");
    debug("---------------------------------------\n");
    debug("WiFi SSID: %s\n", wifi_ssid);
    //debug("WiFi PASS: %s\n", wifi_pass); // Dont show pass through Serial
    debug("Check Internet pinging to: %s\n", internet_check_url);
    debug("Firmware Version: %s\n", firmware_version);
    debug("\n\n");
}

// Get ESP Base MAC Address
char* esp_get_base_mac(void)
{
    static char mac[18];
    uint8_t u8_mac[6];

	esp_read_mac(u8_mac, ESP_MAC_WIFI_STA);
    snprintf(mac, 18, "%02X%02X%02X%02X%02X%02X", u8_mac[0], u8_mac[1], u8_mac[2], u8_mac[3], 
             u8_mac[4], u8_mac[5]);
    
    return mac;
}
