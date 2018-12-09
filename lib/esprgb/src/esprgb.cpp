/**************************************************************************************************/
// Project: WiFiStatusLight
// File: esprgb.cpp
// Description: Library to ease RGB LEDs control.
// Created on: 16 nov. 2018
// Last modified date: 16 nov. 2018
// Version: 1.0.0
/**************************************************************************************************/

/* Libraries */

#include "esprgb.h"

/**************************************************************************************************/

/* Resource Safe Access Mutex Macro */

#if !FREERTOS_MUTEX
    #define MUTEX_INIT() do { } while (0)
    #define MUTEX_SAFE(x) do { x; } while (0)
#else
    #define MUTEX_INIT() do { this_mutex = xSemaphoreCreateMutex(); } while (0)
    #define MUTEX_SAFE(x) do \
    { \
        if(xSemaphoreTake(this_mutex, (portTickType)10)==pdTRUE) \
        { \
            x; \
            xSemaphoreGive(this_mutex); \
        } \
    } while (0)
#endif

/**************************************************************************************************/

/* Constructor */

// EspRGB constructor, get the GPIO pins numbers that going to be used for RGB
EspRGB::EspRGB(const uint8_t pin_r, const uint8_t pin_g, const uint8_t pin_b)
{
    this_pin_r = pin_r;
    this_pin_g = pin_g;
    this_pin_b = pin_b;

    MUTEX_INIT();
}

/**************************************************************************************************/

/* Public Methods */

// Initialize R, G and B GPIOs as outputs and set them to HIGH (LED off)
void EspRGB::init(void)
{
    MUTEX_SAFE
    (
        gpio_as_digital_output(this_pin_r);
        gpio_as_digital_output(this_pin_g);
        gpio_as_digital_output(this_pin_b);
        gpio_high(this_pin_r);
        gpio_high(this_pin_g);
        gpio_high(this_pin_b);
    );
}

// Turn ON all RGB LED colors
void EspRGB::on(void)
{
    MUTEX_SAFE
    (
        gpio_low(this_pin_r);
        gpio_low(this_pin_g);
        gpio_low(this_pin_b);
    );
}

// Turn OFF all RGB LED colors
void EspRGB::off(void)
{
    MUTEX_SAFE
    (
        gpio_high(this_pin_r);
        gpio_high(this_pin_g);
        gpio_high(this_pin_b);
    );
}

// Turn ON the selected LED color (R, G or B)
void EspRGB::on(const esprgb_led led, const bool shutdown_others)
{
    MUTEX_SAFE
    (
        if(led == RGB_RED)
            gpio_low(this_pin_r);
        else if(led == RGB_GREEN)
            gpio_low(this_pin_g);
        else if(led == RGB_BLUE)
            gpio_low(this_pin_b);
        
        if(shutdown_others)
        {
            if(led == RGB_RED)
            {
                gpio_low(this_pin_r);
                gpio_high(this_pin_g);
                gpio_high(this_pin_b);
            }
            else if(led == RGB_GREEN)
            {
                gpio_high(this_pin_r);
                gpio_low(this_pin_g);
                gpio_high(this_pin_b);
            }
            else if(led == RGB_BLUE)
            {
                gpio_high(this_pin_r);
                gpio_high(this_pin_g);
                gpio_low(this_pin_b);
            }
        }
    );
}

// Turn OFF the selected LED color (R, G or B)
void EspRGB::off(const esprgb_led led)
{
    MUTEX_SAFE
    (
        if(led == RGB_RED)
            gpio_high(this_pin_r);
        else if(led == RGB_GREEN)
            gpio_high(this_pin_g);
        else if(led == RGB_BLUE)
            gpio_high(this_pin_b);
    );
}

// Toggle the selected LED color (R, G or B) or toggle all
void EspRGB::toggle(const esprgb_led led, const bool toggle_others)
{
    MUTEX_SAFE
    (
        if(!toggle_others)
        {
            if(led == RGB_RED)
                gpio_toggle(this_pin_r);
            else if(led == RGB_GREEN)
                gpio_toggle(this_pin_g);
            else if(led == RGB_BLUE)
                gpio_toggle(this_pin_b);
        }
        else
        {
            gpio_toggle(this_pin_r);
            gpio_toggle(this_pin_g);
            gpio_toggle(this_pin_b);
        }
    );
}

/**************************************************************************************************/

/* Private Methods (Specific device HAL functions) */

// Set the provided GPIO as Output
void EspRGB::gpio_as_digital_output(const uint8_t gpio)
{
    gpio_pad_select_gpio((gpio_num_t)gpio);
    gpio_set_direction((gpio_num_t)gpio, GPIO_MODE_OUTPUT);
}

// Set the provided output GPIO to LOW
void EspRGB::gpio_low(const uint8_t gpio)
{
    gpio_set_level((gpio_num_t)gpio, 0);
}

// Set the provided output GPIO to LOW
void EspRGB::gpio_high(const uint8_t gpio)
{
    gpio_set_level((gpio_num_t)gpio, 1);
}

// Toggle the provided output GPIO
void EspRGB::gpio_toggle(const uint8_t gpio)
{
    gpio_set_level((gpio_num_t)gpio, 1 - (GPIO.out >> gpio & 0x1));
}
