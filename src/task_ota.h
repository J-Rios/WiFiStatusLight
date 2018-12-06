/**************************************************************************************************/
// Project: WiFiStatusLight
// File: task_ota.cpp
// Description: System Over-The-Air (OTA) update through secure HTTPS FreeRTOS task file
// Created on: 20 nov. 2018
// Last modified date: 06 dec. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef OTA_H
#define OTA_H

/**************************************************************************************************/

/* C++ compiler compatibility */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <string.h>

// FreeRTOS libraries
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

// Device libraries (ESP-IDF)
#include <esp_system.h>
#include <esp_event_loop.h>
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"

// Custom libraries
#include "constants.h"
#include "globals.h"
#include "commons.h"
#include "esprgb.h"

/**************************************************************************************************/

/* HTTPS Certificates locates in internal Blob memory */

extern const uint8_t server_cert_pem_start[] asm("_binary_certs_ca_cert_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_certs_ca_cert_pem_end");

/**************************************************************************************************/

/* Functions */

extern void task_ota(void *pvParameter);

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
