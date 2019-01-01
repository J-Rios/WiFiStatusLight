/**************************************************************************************************/
// Project: WiFiStatusLight
// File: provision.h
// Description: System provisioning functinality based in WiFi AP and WebServer
// Created on: 31 dec. 2018
// Last modified date: 01 jan. 2019
// Version: 0.0.1
/**************************************************************************************************/

/* Libraries */

#include "provision.h"

/**************************************************************************************************/

/* Data Types */


/**************************************************************************************************/

/* Provision Function */

// Launch device provisioning functionality elements (WiFi AP and WebServer)
void launch_provision(Globals* Global)
{
    char ap_ssid[MAX_LENGTH_WIFI_SSID+1];

    // Create SSID based on device MAC and start WiFi AP
    char* device_mac = esp_get_base_mac();
    snprintf(ap_ssid, MAX_LENGTH_WIFI_SSID+1, "%s-%s", DEFAULT_WIFI_AP_SSID, (char*)device_mac);
    wifi_start_ap(ap_ssid, DEFAULT_WIFI_AP_PASS);

    Global->set_first_boot_provision(false);
}

/**************************************************************************************************/

/* WiFi AP Events handler */
static esp_err_t ap_event_handler(void *ctx, system_event_t* e)
{
    switch(e->event_id)
    {
        case SYSTEM_EVENT_AP_START:
            debug("WiFi AP Mode started.\n");
            break;
        
        case SYSTEM_EVENT_AP_STOP:
            debug("WiFi AP Mode stopped.\n");
            break;

        case SYSTEM_EVENT_AP_STACONNECTED:
            debug("Client connected to AP: MAC=" MACSTR ", AID=%d\n", 
                  MAC2STR(e->event_info.sta_connected.mac), e->event_info.sta_connected.aid);
            break;
        
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            debug("Client disconnected from AP: MAC=" MACSTR ", AID=%d\n", 
                  MAC2STR(e->event_info.sta_disconnected.mac), e->event_info.sta_disconnected.aid);
            break;
        
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            break;

        case SYSTEM_EVENT_MAX:
            break;
        
        default:
            break;
    }

    return ESP_OK;
}

/**************************************************************************************************/

/* Functions */

// Create AP SSID based on device MAC
void get_ap_ssid_mac(Globals* Global, const char* prefix, char* ssid)
{
    
}

// Start WiFi Soft-AP
void wifi_start_ap(const char* ssid, const char* pass)
{
    static wifi_config_t wifi_config;

    debug("Creating WiFi AP...\n");
    
    // Set TCP-IP event handler callback
    ESP_ERROR_CHECK(esp_event_loop_init(ap_event_handler, NULL));

    // Configure WiFi AP properties
    memcpy(wifi_config.ap.ssid, ssid, MAX_LENGTH_WIFI_SSID+1);
    memcpy(wifi_config.ap.password, pass, MAX_LENGTH_WIFI_PASS+1);
    wifi_config.ap.ssid_len = strlen(ssid);
    wifi_config.ap.max_connection = 3;
    if (strlen(pass) == 0)
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    else
        wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    // Create and launch WiFi AP
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    debug("WiFi AP \"%s\" succesfully created.\n", ssid);
}
