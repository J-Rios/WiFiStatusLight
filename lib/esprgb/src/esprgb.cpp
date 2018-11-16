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

// ESPRGB constructor, get the GPIO pins numbers that going to be used for RGB
ESPRGB::ESPRGB(const uint8_t pin_r, const uint8_t pin_g, const uint8_t pin_b)
{
    this_pin_r = pin_r;
    this_pin_g = pin_g;
    this_pin_b = pin_b;
}

// Initialize R, G and B GPIOs as outputs and set them to HIGH (LED off)
void ESPRGB::init(void)
{
    gpio_as_output(this_pin_r);
    gpio_as_output(this_pin_g);
    gpio_as_output(this_pin_b);
    gpio_high(this_pin_r);
    gpio_high(this_pin_g);
    gpio_high(this_pin_b);
}

// Turn ON all RGB LED colors
void ESPRGB::on(void)
{
    gpio_low(this_pin_r);
    gpio_low(this_pin_g);
    gpio_low(this_pin_b);
}

// Turn OFF all RGB LED colors
void ESPRGB::off(void)
{
    gpio_high(this_pin_r);
    gpio_high(this_pin_g);
    gpio_high(this_pin_b);
}

// Turn ON the selected LED color (R, G or B)
void ESPRGB::on(const esprgb_led led, const bool shutdown_others)
{
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
}

// Turn OFF the selected LED color (R, G or B)
void ESPRGB::off(const esprgb_led led)
{
    if(led == RGB_RED)
        gpio_high(this_pin_r);
    else if(led == RGB_GREEN)
        gpio_high(this_pin_g);
    else if(led == RGB_BLUE)
        gpio_high(this_pin_b);
}

// Toggle the selected LED color (R, G or B) or toggle all
void ESPRGB::toggle(const esprgb_led led, const bool toggle_others)
{
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
}

/**************************************************************************************************/

/* Private Methods */

// Specific device HAL function: Set the provided GPIO as Output
void ESPRGB::gpio_as_output(const uint8_t gpio)
{
    gpio_pad_select_gpio((gpio_num_t)gpio);
    gpio_set_direction((gpio_num_t)gpio, GPIO_MODE_OUTPUT);
}

// Specific device HAL function: Set the provided output GPIO to LOW
void ESPRGB::gpio_low(const uint8_t gpio)
{
    gpio_set_level((gpio_num_t)gpio, 0);
}

// Specific device HAL function: Set the provided output GPIO to LOW
void ESPRGB::gpio_high(const uint8_t gpio)
{
    gpio_set_level((gpio_num_t)gpio, 1);
}

// Specific device HAL function: Toggle the provided output GPIO
void ESPRGB::gpio_toggle(const uint8_t gpio)
{
   gpio_set_level((gpio_num_t)gpio, 1 - (GPIO.out >> gpio & 0x1));
}
