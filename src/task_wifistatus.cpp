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

/* FreeRTOS event group to signal when we are connected*/
//static EventGroupHandle_t s_wifi_event_group;

/**************************************************************************************************/

/* Task */

// Check for WiFi network changes (connection/disconnection/get IP/ lost IP) and show that status 
// through RGB LED colors
void task_wifi_status(void *pvParameter)
{
    char wifi_ssid[MAX_LENGTH_WIFI_SSID+1];
    char wifi_pass[MAX_LENGTH_WIFI_PASS+1];

    debug("\nWiFi status task initialized.\n");

    // Get provided parameters
    tasks_argv task_argv = (tasks_argv*)pvParameter;
    Globals* Global = task_argv.Global;
    EspRGB* this_LED_RGB = task_argv.LED_RGB;

    // Get actual WiFi SSID and Password config
    Global->get_wifi_ssid(wifi_ssid);
    Global->get_wifi_pass(wifi_pass);

    // Initialize WiFi SoftAP interface
    wifi_init_stat(wifi_ssid, wifi_pass);

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

/* WiFi Change Event Handler */

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id)
    {
        case SYSTEM_EVENT_STA_START:
            printf("WiFi Station interface Up.\n");
            printf("Connecting...\n");
            esp_wifi_connect();
            break;

        case SYSTEM_EVENT_STA_CONNECTED:
			printf("WiFi connected.\n");
            printf("Waiting for IP...\n");
			break;

        case SYSTEM_EVENT_STA_GOT_IP:
            //get_IP();
			//set_host_name();
			printf("WiFi IP received.\nConnection success:\n");
            //printf("  WiFi MAC - %s\n", Global.get_device_mac());
            //printf("  WiFi IPv4 - %s\n", Global.get_device_ip());
            break;

        case SYSTEM_EVENT_STA_LOST_IP:
			printf("WiFi IP lost.\n");
            break;

        case SYSTEM_EVENT_STA_DISCONNECTED:
            printf("WiFi disconnected\n");
            break;

        case SYSTEM_EVENT_STA_STOP:
			printf("WiFi interface stopped\n");
			//Global.set_eth_ip(false);
			//Global.set_eth_connected(false);
			break;

		default:
			break;
    }
    
    return ESP_OK;
}

/**************************************************************************************************/

/* Functions */

void wifi_init_stat(char* ssid, char* pass)
{
    wifi_config_t wifi_config;
    wifi_init_config_t cfg;

    //s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    memcpy(wifi_config.sta.ssid, ssid, MAX_LENGTH_WIFI_SSID);
    memcpy(wifi_config.sta.password, pass, MAX_LENGTH_WIFI_PASS);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());
}
