/**************************************************************************************************/
// Project: WiFiStatusLight
// File: provision.h
// Description: System provisioning functinality based in WiFi AP and WebServer
// Created on: 31 dec. 2018
// Last modified date: 01 jan. 2019
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef PROVISION_H
#define PROVISION_H

/**************************************************************************************************/

/* C++ compiler compatibility */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/

/* Libraries */

// FreeRTOS libraries


// Device libraries (ESP-IDF)
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <openssl/ssl.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>

// Custom libraries
#include "constants.h"
#include "globals.h"
#include "commons.h"

/**************************************************************************************************/

/* HTTPS Certificates locates in internal Blob memory */

extern const uint8_t server_cert_start[] asm("_binary_httpsserver_certs_esp_cert_pem_start");
extern const uint8_t server_cert_end[] asm("_binary_httpsserver_certs_esp_cert_pem_end");
extern const uint8_t server_key_start[] asm("_binary_httpsserver_certs_esp_key_pem_start");
extern const uint8_t server_key_end[] asm("_binary_httpsserver_certs_esp_key_pem_end");

/**************************************************************************************************/

/* Functions */

extern void launch_provision(Globals* Global);

extern void wifi_start_ap(const char* ap_ssid, const char* ap_pass);
extern void start_https_web_server(Globals* Global);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
