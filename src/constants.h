/**************************************************************************************************/
// Project: WiFiStatusLight
// File: constants.h
// Description: Project constants file
// Created on: 17 nov. 2018
// Last modified date: 18 nov. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef CONSTANTS_H
#define CONSTANTS_H

/**************************************************************************************************/

/* Libraries */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/**************************************************************************************************/

/* Defines & Constants */

// Set DEBUG to true or false to enable/disable printf() debugging statement
#define DEBUG true

// I/O pins defines
#define P_O_RGBLED_R GPIO_NUM_12
#define P_O_RGBLED_G GPIO_NUM_13
#define P_O_RGBLED_B GPIO_NUM_14

// Tasks Stack size
#define TASK_WIFI_STATUS_STACK configMINIMAL_STACK_SIZE+2048
#define TASK_OTA_STACK         configMINIMAL_STACK_SIZE+7424

// Arrays and strings maximum length
#define MAX_LENGTH_WIFI_SSID 31
#define MAX_LENGTH_WIFI_PASS 63
#define MAX_LENGTH_MAC_ADDR  17
#define MAX_LENGTH_IPV4      15

// Default device parameters values
#define DEFAULT_WIFI_SSID     "MyNet"
#define DEFAULT_WIFI_PASS     "testpassword1423"
#define DEFAULT_DEVICE_MAC    "00:00:00:00:00:00"
#define DEFAULT_DEVICE_IPV4   "0.0.0.0"
#define MAX_WIFI_CONN_RETRIES 50
#define OTA_HTTPS_SERVER      "https://192.168.0.50:8070/new_firmware.bin"

/**************************************************************************************************/

#endif
