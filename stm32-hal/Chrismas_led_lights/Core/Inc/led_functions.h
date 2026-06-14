/**
 * @file led_functions.h
 * @brief Header file for the LED control module via TIM4 PWM.
 * @details This module provides an interface to initialize and dynamically adjust
 * the brightness and blinking frequency of LEDs. All adjustments include
 * underflow and overflow protection.
 */

#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include "main.h"

void LED_power_ON(void);

void LED_power_OFF(void);

void LED_set_brightness(uint8_t led_index, uint8_t brightness_value);

void LED_set_all_brightness(uint8_t brightness_value);

void LED_decrease_brightness(uint8_t led_index);

void LED_increase_brightness(uint8_t led_index);

void LED_set_frequency(uint16_t frequency);

void LED_decrease_all_brightness(void);

void LED_increase_all_brightness(void);

void LED_decrease_frequency(void);

void LED_increase_frequency(void);


#endif /* LED_FUNCTIONS_H */