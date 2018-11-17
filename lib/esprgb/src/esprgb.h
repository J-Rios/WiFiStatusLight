/**************************************************************************************************/
// Project: WiFiStatusLight
// File: esprgb.h
// Description: Library to ease RGB LEDs control.
// Created on: 16 nov. 2018
// Last modified date: 16 nov. 2018
// Version: 1.0.0
/**************************************************************************************************/

/* Include Guard */

#ifndef ESPRGB_H_
#define ESPRGB_H_

/**************************************************************************************************/

/* Libraries */

// Device libraries (ESP-IDF)
#include <driver/gpio.h>

/**************************************************************************************************/

/* Library Data Types */

// RGB LED component data type
typedef enum
{
    RGB_RED   = 0,
    RGB_GREEN = 1,
    RGB_BLUE  = 2,
} esprgb_led;

/**************************************************************************************************/

class ESPRGB
{
    public:
        ESPRGB(const uint8_t pin_r, const uint8_t pin_g, const uint8_t pin_b);
        void init(void);
        void on(void);
        void off(void);
        void on(const esprgb_led led, const bool shutdown_others=true);
        void off(const esprgb_led led);
        void toggle(const esprgb_led led, const bool toggle_others=false);

    private:
        uint8_t this_pin_r;
        uint8_t this_pin_g;
        uint8_t this_pin_b;

        void gpio_as_output(const uint8_t gpio);
        void gpio_low(const uint8_t gpio);
        void gpio_high(const uint8_t gpio);
        void gpio_toggle(const uint8_t gpio);
};

/**************************************************************************************************/

#endif
