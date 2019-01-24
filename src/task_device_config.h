/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_device_config.h
// Description: Device provision-configuration through AP and WebServer, FreeRTOS task file
// Created on: 19 jan. 2019
// Last modified date: 19 jan. 2019
// Version: 1.0.0
/**************************************************************************************************/

/* Include Guard */

#ifndef DEVICECONFIG_H
#define DEVICECONFIG_H

/**************************************************************************************************/

/* C++ compiler compatibility */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <string.h>

// Device libraries (ESP-IDF)
#include <esp_log.h>
#include <nvs_flash.h>
#include <openssl/ssl.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>

// Custom libraries
#include "constants.h"
#include "globals.h"
#include "commons.h"
#include "rgbleds.h"

/**************************************************************************************************/

/* HTTPS Certificates locates in internal Blob memory */

extern const uint8_t server_cert_start[] asm("_binary_httpsserver_certs_esp_cert_pem_start");
extern const uint8_t server_cert_end[] asm("_binary_httpsserver_certs_esp_cert_pem_end");
extern const uint8_t server_key_start[] asm("_binary_httpsserver_certs_esp_key_pem_start");
extern const uint8_t server_key_end[] asm("_binary_httpsserver_certs_esp_key_pem_end");


/**************************************************************************************************/

/* Functions */

extern void task_device_config(void *pvParameter);

extern void launch_config_mode(Globals* Global);
extern void wifi_start_ap(const char* ap_ssid, const char* ap_pass);
extern void start_https_web_server(Globals* Global);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
