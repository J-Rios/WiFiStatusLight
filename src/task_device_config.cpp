/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_device_config.cpp
// Description: Device provision-configuration through AP and WebServer, FreeRTOS task file
// Created on: 19 jan. 2019
// Last modified date: 19 jan. 2019
// Version: 1.0.0
/**************************************************************************************************/

/* Libraries */

#include "task_device_config.h"

/**************************************************************************************************/

/* HTML Builtin */

#define TAG "HTTPS WebServer"
#define WEBSERVER_RX_BUFFER_LEN 1024

#define WEBSERVER_ROOT_HTML \
    "HTTP/1.1 200 OK\r\n" \
    "Content-Type: text/html\r\n" \
    "Content-Length: 98\r\n\r\n" \
    "<html>\r\n" \
    "<head>\r\n" \
    "<title>ESP32 Server</title></head><body>\r\n" \
    "Web Content...\r\n" \
    "</body>\r\n" \
    "</html>\r\n" \
    "\r\n"

/**************************************************************************************************/

/* Data Types */
volatile bool ap_start = false;

/**************************************************************************************************/

/* Task */

// Check for device configuration from user through an WiFi AP and secure WebServer
void task_device_config(void *pvParameter)
{
    bool first_boot = true;
    bool is_config_on = false;
    uint32_t btn_press_time = 0;

    // Get provided parameters
    tasks_argv* task_argv = (tasks_argv*)pvParameter;
    Globals* Global = task_argv->Global;
    RGBLEDs* LED_RGB = task_argv->LED_RGB;
    Buttons* Btn_AP_Conf = task_argv->Btn_AP_Conf;

    // Check if it is first boot
    Global->get_first_boot_provision(first_boot);

    debug("\nDevice configuration task initialized.\n");

    while(1)
    {
        // Check if Config Button is pressed during 5s
        if(!is_config_on)
        {
            if(Btn_AP_Conf->read() == 0)
            {
                btn_press_time = btn_press_time + 200;
                delay(100);
            }
            else
                btn_press_time = 0;
        }

        // Launch AP and WebServer
        if((first_boot == true) || (btn_press_time >= 5000))
        {
            if(!is_config_on)
            {
                is_config_on = true;
                first_boot = false;
                Global->set_first_boot_provision(first_boot);

                // Tur Red the RGB LED
                LED_RGB->on(RGB_RED);
                
                launch_config_mode(Global);
            }
            else
            {
                // TODO - Close Web Server and WiFi AP
            }
        }

        // Task CPU release
        delay(100);
    }
}

/**************************************************************************************************/

/* Provision Function */

// Launch device configuration mode (WiFi AP and WebServer)
void launch_config_mode(Globals* Global)
{
    char ap_ssid[MAX_LENGTH_WIFI_SSID+1];

    // Create SSID based on device MAC and start WiFi AP
    char* device_mac = esp_get_base_mac();
    snprintf(ap_ssid, MAX_LENGTH_WIFI_SSID+1, "%s-%s", DEFAULT_WIFI_AP_SSID, (char*)device_mac);
    wifi_start_ap(ap_ssid, DEFAULT_WIFI_AP_PASS);

    // Wait until AP start
    while(!ap_start)
        delay(100);
    
    // Create Web Server
    start_https_web_server(Global);

    Global->set_first_boot_provision(false);
}

/**************************************************************************************************/

/* WiFi AP Events handler */

// Note - For some reason, set twice wifi events handlers fail (AP+STAT)
/*static esp_err_t ap_event_handler(void *ctx, system_event_t* e)
{
    switch(e->event_id)
    {
        case SYSTEM_EVENT_AP_START:
            debug("WiFi AP Mode started.\n");
            ap_start = true;
            break;
        
        case SYSTEM_EVENT_AP_STOP:
            debug("WiFi AP Mode stopped.\n");
            ap_start = false;
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
}*/

/**************************************************************************************************/

/* SoftAP Functions */

// Start WiFi Soft-AP
void wifi_start_ap(const char* ssid, const char* pass)
{
    static wifi_config_t wifi_config;

    debug("Creating WiFi AP...\n");
    
    // Set TCP-IP event handler callback
    // Note - For some reason, set twice wifi events handlers fail (AP+STAT)
    //ESP_ERROR_CHECK(esp_event_loop_init(ap_event_handler, NULL));

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
    //ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    debug("WiFi AP \"%s\" succesfully created.\n", ssid);
}

/**************************************************************************************************/

/* HTTPS Web Server Functions */

// Start HTTPS Web Server
void start_https_web_server(Globals* Global)
{
    int ret;

    SSL_CTX *ctx;
    SSL *ssl;

    int sockfd, new_sockfd;
    socklen_t addr_len;
    struct sockaddr_in sock_addr;

    char recv_buf[WEBSERVER_RX_BUFFER_LEN];

    const char send_data[] = WEBSERVER_ROOT_HTML;
    const int send_bytes = sizeof(send_data); 

    ESP_LOGI(TAG, "SSL server context create ......");
    /* For security reasons, it is best if you can use
       TLSv1_2_server_method() here instead of TLS_server_method().
       However some old browsers may not support TLS v1.2.
    */
    ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        ESP_LOGI(TAG, "failed");
        goto failed1;
    }
    ESP_LOGI(TAG, "OK");

    ESP_LOGI(TAG, "SSL server context set own certification......");
    ret = SSL_CTX_use_certificate_ASN1(ctx, server_cert_end-server_cert_start, server_cert_start);
    if (!ret) {
        ESP_LOGI(TAG, "failed");
        goto failed2;
    }
    ESP_LOGI(TAG, "OK");

    ESP_LOGI(TAG, "SSL server context set private key......");
    ret = SSL_CTX_use_PrivateKey_ASN1(0, ctx, server_key_start, server_key_end-server_key_start);
    if (!ret) {
        ESP_LOGI(TAG, "failed");
        goto failed2;
    }
    ESP_LOGI(TAG, "OK");

    ESP_LOGI(TAG, "SSL server create socket ......");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        ESP_LOGI(TAG, "failed");
        goto failed2;
    }
    ESP_LOGI(TAG, "OK");

    ESP_LOGI(TAG, "SSL server socket bind ......");
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = 0;
    sock_addr.sin_port = htons(HTTPS_PORT);
    ret = bind(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
    if (ret) {
        ESP_LOGI(TAG, "failed");
        goto failed3;
    }
    ESP_LOGI(TAG, "OK");

    ESP_LOGI(TAG, "SSL server socket listen ......");
    ret = listen(sockfd, 32);
    if (ret) {
        ESP_LOGI(TAG, "failed");
        goto failed3;
    }
    ESP_LOGI(TAG, "OK");

reconnect:
    ESP_LOGI(TAG, "SSL server create ......");
    ssl = SSL_new(ctx);
    if (!ssl) {
        ESP_LOGI(TAG, "failed");
        goto failed3;
    }
    ESP_LOGI(TAG, "OK");

    ESP_LOGI(TAG, "SSL server socket accept client ......");
    new_sockfd = accept(sockfd, (struct sockaddr *)&sock_addr, &addr_len);
    if (new_sockfd < 0) {
        ESP_LOGI(TAG, "failed" );
        goto failed4;
    }
    ESP_LOGI(TAG, "OK");

    SSL_set_fd(ssl, new_sockfd);

    ESP_LOGI(TAG, "SSL server accept client ......");
    ret = SSL_accept(ssl);
    if (!ret) {
        ESP_LOGI(TAG, "failed");
        goto failed5;
    }
    ESP_LOGI(TAG, "OK");

    ESP_LOGI(TAG, "SSL server read message ......");
    do {
        memset(recv_buf, 0, WEBSERVER_RX_BUFFER_LEN);
        ret = SSL_read(ssl, recv_buf, WEBSERVER_RX_BUFFER_LEN - 1);
        if (ret <= 0) {
            break;
        }
        ESP_LOGI(TAG, "SSL read: %s", recv_buf);
        if (strstr(recv_buf, "GET ") &&
            strstr(recv_buf, " HTTP/1.1")) {
            ESP_LOGI(TAG, "SSL get matched message");
            ESP_LOGI(TAG, "SSL write message");
            ret = SSL_write(ssl, send_data, send_bytes);
            if (ret > 0) {
                ESP_LOGI(TAG, "OK");
            } else {
                ESP_LOGI(TAG, "error");
            }
            break;
        }
    } while (1);
    
    SSL_shutdown(ssl);
failed5:
    close(new_sockfd);
    new_sockfd = -1;
failed4:
    SSL_free(ssl);
    ssl = NULL;
    goto reconnect;
failed3:
    close(sockfd);
    sockfd = -1;
failed2:
    SSL_CTX_free(ctx);
    ctx = NULL;
failed1:
    vTaskDelete(NULL);
    return;
}
