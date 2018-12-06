/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_ota.cpp
// Description: System Over-The-Air (OTA) update through secure HTTPS FreeRTOS task file
// Created on: 20 nov. 2018
// Last modified date: 06 dec. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Libraries */

#include "task_ota.h"

/**************************************************************************************************/

/* OTA Events Handler Prototype */

static esp_err_t _http_event_handler(esp_http_client_event_t *evt);

/**************************************************************************************************/

/* Task */

// Check for WiFi network changes (connection/disconnection/get IP/ lost IP) and show that status 
// through RGB LED colors
void task_ota(void *pvParameter)
{
    bool wifi_connected = false;
    bool wifi_has_ip = false;
    bool ota_update = false;

    // Get provided parameters
    tasks_argv* task_argv = (tasks_argv*)pvParameter;
    Globals* Global = task_argv->Global;
    //EspRGB* this_LED_RGB = task_argv->LED_RGB;

    debug("\nOTA task initialized.\n");

    // Initialize and set Set OTA http client config
    esp_http_client_config_t config;
    config.cert_pem = (char*)server_cert_pem_start;
    config.event_handler = _http_event_handler;
    config.url = OTA_HTTPS_SERVER;

    while(1)
    {
        // Check for actual WiFi status
        Global->get_wifi_connected(wifi_connected);
        Global->get_wifi_has_ip(wifi_has_ip);
        Global->get_ota_update(ota_update);

        if(ota_update)
        {
            printf("An OTA update request has been set.\n");

            ota_update = false;
            Global->set_ota_update(ota_update);
            
            if(wifi_connected && wifi_has_ip)
            {
                printf("Trying to update through OTA...\n");

                // Connect to the Server that has the OTA firmware and download-flash it
                esp_err_t ret = esp_https_ota(&config);
                if (ret == ESP_OK)
                {
                    debug("Firmware successfully upgraded.\n");
                    debug("Rebooting the system...\n");
                    esp_restart();
                }
                else
                {
                    debug("Firmware Upgrades Failed.\n");
                    if(ret == ESP_ERR_NO_MEM)
                        debug("Not enough memory available for OTA download.");
                    else
                        ESP_ERROR_CHECK(ret);
                }
            }
            else
                printf("The update can't be done cause the system has not Internet access.\n");
        }

        // Task CPU release
        delay(100);
    }
}

/**************************************************************************************************/

/* OTA Events Handler */

static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id)
    {
        case HTTP_EVENT_ERROR:
            debug("HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            debug("HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            debug("HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            debug("HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            debug("HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            break;
        case HTTP_EVENT_ON_FINISH:
            debug("HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            debug("HTTP_EVENT_DISCONNECTED");
            break;
    }

    return ESP_OK;
}

/**************************************************************************************************/

/* Functions */

