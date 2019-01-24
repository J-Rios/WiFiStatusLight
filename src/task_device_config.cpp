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
    bool creating_ap = true;
    Global->set_start_ap(true);

    // Wait until manage_wifi task create AP
    while(creating_ap)
    {
        Global->get_start_ap(creating_ap);
        delay(100);
    }
    
    // Launch Web Server
    start_https_web_server(Global);

    Global->set_first_boot_provision(false);
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
