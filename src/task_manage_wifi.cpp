/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_manage_wifi.cpp
// Description: Device network (WiFi) management FreeRTOS task file
// Created on: 24 jan. 2019
// Last modified date: 24 jan. 2019
// Version: 1.0.0
/**************************************************************************************************/

/* Libraries */

#include "task_manage_wifi.h"

/**************************************************************************************************/

/* Data Types */
volatile bool ap_started = false;
volatile bool sta_started = false;
wifi_config_t wifi_cfg;

/**************************************************************************************************/

/* Task */

// Note: If AP is initialized and STA becomes initialized, AP will restart with same STA connection 
// channel (clients connected to AP will be drop)

// Check for device state to setup and configure WiFi network (create station, access point or both)
void task_manage_wifi(void *pvParameter)
{
    char ap_ssid[MAX_LENGTH_WIFI_SSID+1];
    bool start_ap = false;
    bool start_sta = false;

    // Get provided parameters
    tasks_argv* task_argv = (tasks_argv*)pvParameter;
    Globals* Global = task_argv->Global;

    debug("\nManage network WiFi task initialized.\n");

    // Create SSID based on device MAC and start WiFi AP
    char* device_mac = esp_get_base_mac();
    snprintf(ap_ssid, MAX_LENGTH_WIFI_SSID+1, "%s-%s", DEFAULT_WIFI_AP_SSID, (char*)device_mac);

    // WiFi interface initialization (needs NVS to be initialized before)
    wifi_init(Global);

    // Start Station interface by default
    Global->set_start_sta(true);

    while(1)
    {
        Global->get_start_ap(start_ap);
        Global->get_start_sta(start_sta);

        // Initialize WiFi Station interface
        if(start_sta && !sta_started)
        {
            wifi_start_stat(Global);
            sta_started = true;
        }

        // Initialize WiFi SoftAP interface if station interface was initialized and try to 
        // connect once
        if(!start_sta)
        {
            if(start_ap && !ap_started)
            {
                wifi_start_ap(ap_ssid, DEFAULT_WIFI_AP_PASS);
                ap_started = true;
            }
        }

        // Task CPU release
        delay(100);
    }
}

/**************************************************************************************************/

/* WiFi Change Event Handler */

static esp_err_t network_event_handler(void *ctx, system_event_t *e)
{
    static uint8_t conn_fail_retries = 0;
    char ip[MAX_LENGTH_IPV4+1];

    Globals* Global = (Globals*)ctx;

    switch(e->event_id)
    {
        case SYSTEM_EVENT_AP_START:
            debug("WiFi AP Mode started.\n");
            Global->set_start_ap(false);
            break;

        case SYSTEM_EVENT_AP_STOP:
            debug("WiFi AP Mode stopped.\n");
            ap_started = false;
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

        case SYSTEM_EVENT_STA_START:
            debug("WiFi Station interface Up.\n");
            debug("Connecting...\n");
            esp_wifi_connect();
            Global->set_start_sta(false);
            break;

        case SYSTEM_EVENT_STA_CONNECTED:
            debug("WiFi connected.\n");
            debug("Waiting for IP...\n");
            Global->set_wifi_connected(true);
            break;

        case SYSTEM_EVENT_STA_GOT_IP:
            Global->set_wifi_ip(ip4addr_ntoa(&e->event_info.got_ip.ip_info.ip));
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
            sta_started = false;
            break;

        default:
            break;
    }
    
    return ESP_OK;
}

/**************************************************************************************************/

/* Functions */

// Init WiFi interface
void wifi_init(Globals* Global)
{
    static wifi_init_config_t wifi_init_cfg;

    debug("Initializing TCP-IP adapter...\n");

    tcpip_adapter_init();

    wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_cfg));
    esp_wifi_set_mode(WIFI_MODE_APSTA);

    // Set TCP-IP event handler callback for Access Point interface
    ESP_ERROR_CHECK(esp_event_loop_init(network_event_handler, Global));

    debug("TCP-IP adapter successfuly initialized.\n");
}

// Start WiFi Soft-AP
void wifi_start_ap(const char* ssid, const char* pass)
{
    debug("Creating WiFi AP...\n");

    // Configure WiFi AP properties
    memcpy(wifi_cfg.ap.ssid, ssid, MAX_LENGTH_WIFI_SSID+1);
    memcpy(wifi_cfg.ap.password, pass, MAX_LENGTH_WIFI_PASS+1);
    wifi_cfg.ap.ssid_len = strlen(ssid);
    wifi_cfg.ap.max_connection = 3;
    if (strlen(pass) == 0)
        wifi_cfg.ap.authmode = WIFI_AUTH_OPEN;
    else
        wifi_cfg.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    // Create and launch WiFi AP
    //ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());

    debug("WiFi AP \"%s\" succesfully created.\n", ssid);
}

// Start WiFi Station Client
void wifi_start_stat(Globals* Global)
{
    // Configure WiFi AP properties
    char wifi_ssid[MAX_LENGTH_WIFI_SSID+1];
    char wifi_pass[MAX_LENGTH_WIFI_PASS+1];
    Global->get_wifi_ssid(wifi_ssid);
    Global->get_wifi_pass(wifi_pass);
    memcpy(wifi_cfg.sta.ssid, wifi_ssid, MAX_LENGTH_WIFI_SSID+1);
    memcpy(wifi_cfg.sta.password, wifi_pass, MAX_LENGTH_WIFI_PASS+1);

    // Create and launch WiFi Station
    //ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());
}
