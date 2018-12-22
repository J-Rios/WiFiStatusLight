/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_wifistatus.cpp
// Description: WiFi status FreeRTOS task file
// Created on: 17 nov. 2018
// Last modified date: 22 dec. 2018
// Version: 1.0.0
/**************************************************************************************************/

/* Libraries */

#include "task_wifistatus.h"

/**************************************************************************************************/

/* Task */

// Check for WiFi network changes (connection/disconnection/get IP/ lost IP) and show that status 
// through RGB LED colors
void task_wifi_status(void *pvParameter)
{
    bool wifi_connected = false;
    bool wifi_has_ip = false;
    bool internet_conn = false;

    // Get provided parameters
    tasks_argv* task_argv = (tasks_argv*)pvParameter;
    Globals* Global = task_argv->Global;
    RGBLEDs* LED_RGB = task_argv->LED_RGB;

    debug("\nWiFi status task initialized.\n");

    // Tur Red the RGB LED
    LED_RGB->on(RGB_RED);

    // Initialize WiFi SoftAP interface
    wifi_init_stat(Global);

    while(1)
    {
        // Check for actual WiFi status
        Global->get_wifi_connected(wifi_connected);
        Global->get_wifi_has_ip(wifi_has_ip);
        Global->get_internet_connection(internet_conn);

        // Show the actual WiFi status using the RGB LED
        if(!wifi_connected && !wifi_has_ip)
            LED_RGB->on(RGB_RED);
        else if(wifi_connected && !wifi_has_ip)
        {
            LED_RGB->toggle(RGB_RED);
            delay(400);
        }
        else if(wifi_connected && wifi_has_ip)
        {
            if(!internet_conn)
                LED_RGB->on(RGB_GREEN);
            else
                LED_RGB->on(RGB_BLUE);
        }

        // Task CPU release
        delay(100);
    }
}

/**************************************************************************************************/

/* WiFi Change Event Handler */

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    static uint8_t conn_fail_retries = 0;
    uint8_t mac[MAX_LENGTH_MAC_ADDR+1];
    char ip[MAX_LENGTH_IPV4+1];

    Globals* Global = (Globals*)ctx;

    switch(event->event_id)
    {
        case SYSTEM_EVENT_STA_START:
            ESP_ERROR_CHECK(esp_wifi_get_mac(WIFI_IF_STA, mac));
            Global->set_device_mac(mac);
            debug("WiFi Station interface Up (%s).\n", mac);
            debug("Connecting...\n");
            esp_wifi_connect();
            break;

        case SYSTEM_EVENT_STA_CONNECTED:
            debug("WiFi connected.\n");
            debug("Waiting for IP...\n");
            Global->set_wifi_connected(true);
            break;

        case SYSTEM_EVENT_STA_GOT_IP:
            Global->set_wifi_ip(ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            Global->get_wifi_ip(ip);
            debug("WiFi IPv4 received: %s\n", ip);
            Global->set_wifi_has_ip(true);
            break;

        case SYSTEM_EVENT_STA_LOST_IP:
            debug("WiFi IP lost.\n");
            Global->set_wifi_ip(DEFAULT_DEVICE_IPV4);
            Global->set_wifi_has_ip(false);
            break;

        case SYSTEM_EVENT_STA_DISCONNECTED:
            bool wifi_connected;
            Global->get_wifi_connected(wifi_connected);
            if(wifi_connected)
            {
                debug("WiFi disconnected\n");
                conn_fail_retries = 0;
            }
            else
            {
                debug("Can't connect to AP, trying again...\n");
                conn_fail_retries = conn_fail_retries + 1;
            }
            Global->set_wifi_ip(DEFAULT_DEVICE_IPV4);
            Global->set_wifi_has_ip(false);
            Global->set_wifi_connected(false);
            if(conn_fail_retries < MAX_WIFI_CONN_RETRIES)
                esp_wifi_connect();
            else
            {
                debug("WiFi connection fail %d times.\n", MAX_WIFI_CONN_RETRIES);
                debug("Rebooting the system...\n\n");
                esp_restart();
            }
            break;

        case SYSTEM_EVENT_STA_STOP:
            debug("WiFi interface stopped\n");
            conn_fail_retries = 0;
            Global->set_wifi_ip(DEFAULT_DEVICE_IPV4);
            Global->set_wifi_has_ip(false);
            Global->set_wifi_connected(false);
            break;

        default:
            break;
    }
    
    return ESP_OK;
}

/**************************************************************************************************/

/* Functions */

void wifi_init_stat(Globals* Global)
{
    static wifi_config_t wifi_config;
    static wifi_init_config_t cfg;
    char wifi_ssid[MAX_LENGTH_WIFI_SSID+1];
    char wifi_pass[MAX_LENGTH_WIFI_PASS+1];

    Global->get_wifi_ssid(wifi_ssid);
    Global->get_wifi_pass(wifi_pass);

    memcpy(wifi_config.sta.ssid, wifi_ssid, MAX_LENGTH_WIFI_SSID+1);
    memcpy(wifi_config.sta.password, wifi_pass, MAX_LENGTH_WIFI_PASS+1);

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, Global));

    cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}
