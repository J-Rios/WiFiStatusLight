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

/* Commons Defines & Constants */

// Set DEBUG to true or false to enable/disable printf() debugging statement
#define DEBUG true

// I/O pins defines
#define P_O_RGBLED_R             GPIO_NUM_25
#define P_O_RGBLED_G             GPIO_NUM_32
#define P_O_RGBLED_B             GPIO_NUM_33
#define P_I_BTN_OTA              GPIO_NUM_34

// Tasks Stack size
#define TASK_WIFI_STATUS_STACK   configMINIMAL_STACK_SIZE+2048
#define TASK_INTERNET_STACK      configMINIMAL_STACK_SIZE+2048
#define TASK_OTA_STACK           configMINIMAL_STACK_SIZE+7424

// Arrays and strings maximum length
#define MAX_LENGTH_VERSION       11
#define MAX_LENGTH_WIFI_SSID     31
#define MAX_LENGTH_WIFI_PASS     63
#define MAX_LENGTH_MAC_ADDR      17
#define MAX_LENGTH_IPV4          15

// Default device parameters values
#define DEFAULT_WIFI_SSID        "MyNet"
#define DEFAULT_WIFI_PASS        "testpassword1423"
#define DEFAULT_DEVICE_MAC       "00:00:00:00:00:00"
#define DEFAULT_DEVICE_IPV4      "0.0.0.0"
#define DEFAULT_FIRMWARE_VERSION "0.0.1"
#define MAX_WIFI_CONN_RETRIES    50

/**************************************************************************************************/

/* Task OTA Defines & Constants */

// Over The Air Server last version file and firmware, and server connecton port
#define OTA_SERVER_VERSION_FILE  "https://192.168.0.130/version.txt"
#define OTA_SERVER_FIRMWARE_FILE "https://192.168.0.130/firmware.bin"
#define OTA_SERVER_HTTPS_PORT    443


/**************************************************************************************************/

/* Task Internet Status Defines & Constants */

// Internet remote server to ping and time between each Internet status checks (30s)
#define CHECK_INTERNET_URL       "8.8.8.8"
#define T_INTERNET_CHECK         30000

/**************************************************************************************************/

#endif
