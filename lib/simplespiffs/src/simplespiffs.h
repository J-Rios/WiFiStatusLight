/**************************************************************************************************/
// Project: WiFiStatusLight
// File: simplespiffs.cpp
// Description: HAL library to ease SPIFFS usage.
// Created on: 22 dec. 2018
// Last modified date: 22 dec. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef SIMPLESPIFFS_H_
#define SIMPLESPIFFS_H_

/**************************************************************************************************/

/* Defines & Macros */

// Set to true or false to enable/disable FreeRTOS safe use of the input pin through multiples Tasks
#define FREERTOS_MUTEX true

/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#if FREERTOS_MUTEX
    // Include SDK and Freertos
    #include <freertos/FreeRTOS.h>
    #include <freertos/semphr.h>
#endif

// Device libraries (ESP-IDF)
#include "esp_spiffs.h"

/**************************************************************************************************/

/* Defines & Constants */

// Enable/Disable Serial debug messages
#define DEBUG true

// Debug macro
#define debug(...) do { if(DEBUG) printf(__VA_ARGS__); } while (0)

// Maximum SPIFFS file text line lenght
#define MAX_LENGHT_SPIFFS_LINE 64

/**************************************************************************************************/

class SimpleSPIFFS
{
    public:
        SimpleSPIFFS(void);
        bool mount(void);
        bool unmount(void);
        bool file_exists(const char* path);
        int32_t file_count_lines(const char* path);
        bool file_read_line(const char* path, const uint16_t line_num, char* read_line);
        bool file_write_line(const char* path, char* text_line);
        bool file_append_line(const char* path, char* text_line);
        bool file_move_rename(const char* path1, const char* path2);
        bool file_remove(const char* path);

    private:
        esp_vfs_spiffs_conf_t spiffs_conf;
        #if FREERTOS_MUTEX
            SemaphoreHandle_t this_mutex;
        #endif
};

/**************************************************************************************************/

#endif
