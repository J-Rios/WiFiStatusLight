/**************************************************************************************************/
// Project: WiFiStatusLight
// File: constants.h
// Description: Project constants file
// Created on: 17 nov. 2018
// Last modified date: 17 nov. 2018
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

// Arrays and strings maximum length
#define MAX_LENGTH_MAC_ADDR  18

/**************************************************************************************************/

/* Macros */

// Debug macro
#define debug(...) do { if(DEBUG) printf(__VA_ARGS__); } while (0)

// FreeRTOS to Arduino like delay() macro
#define delay(x) do { vTaskDelay(x/portTICK_PERIOD_MS); } while(0)

/**************************************************************************************************/

#endif
