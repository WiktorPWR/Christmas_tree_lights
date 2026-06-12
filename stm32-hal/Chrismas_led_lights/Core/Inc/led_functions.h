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

/**
 * @brief  Initializes the LED control state variables and applies them to the hardware.
 * @note   Sets the initial brightness to 50% and frequency to 1 Hz, then updates
 * the timer configuration immediately.
 * @retval None
 */
void LED_set_init_state(void);

/**
 * @brief  Increments the LED blinking frequency by a predefined constant value.
 * @details Automatically clamps the frequency to the maximum allowed limit (100 Hz).
 * The hardware update is forced immediately without waiting for the current cycle to finish.
 * @retval None
 */
void LED_blinking_speed_increment_by_const_value(void);

/**
 * @brief  Decrements the LED blinking frequency by a predefined constant value.
 * @details Automatically clamps the frequency to the minimum allowed limit (1 Hz).
 * The hardware update is forced immediately without waiting for the current cycle to finish.
 * @retval None
 */
void LED_blinking_speed_decrement_by_const_value(void);

/**
 * @brief  Increments the LED brightness duty cycle by a predefined constant value.
 * @details Automatically clamps the brightness percentage to the maximum allowed limit (100%).
 * The hardware update is forced immediately without waiting for the current cycle to finish.
 * @retval None
 */
void LED_brightness_increment_by_const_value(void);

/**
 * @brief  Decrements the LED brightness duty cycle by a predefined constant value.
 * @details Automatically clamps the brightness percentage to the minimum allowed limit (0%).
 * The hardware update is forced immediately without waiting for the current cycle to finish.
 * @retval None
 */
void LED_brightness_decrement_by_const_value(void);

#endif /* LED_FUNCTIONS_H */