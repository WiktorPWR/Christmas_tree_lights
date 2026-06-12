/**
 * @file led_functions.c
 * @brief LED Control Driver utilizing TIM4 PWM generation.
 * @details This module provides functions to dynamically adjust both the 
 * blinking frequency and duty cycle (brightness) of connected LEDs.
 * It ensures safe register modifications with boundary checking.
 */

#include "led_functions.h"
#include "main.h"

/* --- CONSTANTS & CONFIGURATION --- */

/** @brief Maximum allowable brightness percentage. */
#define MAX_BRIGHTNESS 100

/** @brief Minimum allowable brightness percentage. */
#define MIN_BRIGHTNESS 0

/** @brief Maximum allowable blinking frequency in Hz. */
#define MAX_FREQUENCY 100

/** @brief Minimum allowable blinking frequency in Hz. */
#define MIN_FREQUENCY 1

/** @brief Step size for frequency adjustments (Hz). */
#define CHANGE_FREQUENCY_VALUE 1

/** @brief Step size for brightness adjustments (%). */
#define CHANGE_BRIGHTNESS_VALUE 10

/** @brief Default brightness percentage on initialization. */
#define START_BRIGHTNESS_PCT 50

/** @brief Default blinking frequency in Hz on initialization. */
#define START_FREQUENCY 1


/* --- EXTERNAL PERIPHERALS --- */

/** @brief Hardware Timer 4 handle defined in main.c */
extern TIM_HandleTypeDef htim4;


/* --- STATE VARIABLES --- */

/** @brief Current brightness duty cycle, represented as a percentage (0-100%). */
volatile static uint16_t current_brightness_pct; 

/** @brief Current blinking frequency in Hertz (Hz). */
volatile static uint16_t current_frequency; 


/* --- PRIVATE FUNCTIONS --- */

/**
 * @brief  Recalculates and updates the hardware timer registers.
 * @note   Calculates the new Auto-Reload Register (ARR) value based on the target frequency,
 * then scales the Capture/Compare Registers (CCR) to maintain the requested brightness.
 * @retval None
 */
static void LED_Hardware_Update(void) {
    /* 1. Recalculate and set the new frequency (ARR) */
    uint16_t new_arr_value = ((SystemCoreClock * 4) / (current_frequency * (htim4.Init.Prescaler + 1))) - 1;
    __HAL_TIM_SET_AUTORELOAD(&htim4, new_arr_value);
    htim4.Init.Period = new_arr_value; // Update the structure for the calculation below

    /* 2. Recalculate and set the pulse width (CCR) keeping a constant brightness percentage */
    uint16_t new_pulse=0;
    if(current_brightness_pct == MAX_BRIGHTNESS) {
        new_pulse = htim4.Init.Period-1;
    } else {
        new_pulse = (current_brightness_pct * (htim4.Init.Period + 1)) / 100;
    }

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, new_pulse);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, new_pulse);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, new_pulse);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, new_pulse);

    htim4.Instance->EGR = TIM_EGR_UG; // Generate an update event to apply changes immediately
}


/* --- PUBLIC FUNCTIONS (FREQUENCY CONTROL) --- */

/**
 * @brief  Initializes the internal LED state variables and applies them to the hardware.
 * @details Sets the initial brightness to 50% and frequency to 1 Hz.
 * @retval None
 */
void LED_set_init_state(void) {
    current_brightness_pct = START_BRIGHTNESS_PCT; 
    current_frequency = START_FREQUENCY; 
    LED_Hardware_Update(); 
}

/**
 * @brief  Increments the blinking frequency by a predefined constant value.
 * @details Clamps the value to MAX_FREQUENCY if the threshold is exceeded.
 * @retval None
 */
void LED_blinking_speed_increment_by_const_value(void) {
    current_frequency += CHANGE_FREQUENCY_VALUE;
    if(current_frequency > MAX_FREQUENCY) {
        current_frequency = MAX_FREQUENCY;
    }
    LED_Hardware_Update(); 
}

/**
 * @brief  Decrements the blinking frequency by a predefined constant value.
 * @details Prevents underflow by safely clamping the value to MIN_FREQUENCY.
 * @retval None
 */
void LED_blinking_speed_decrement_by_const_value(void) {
    if (current_frequency >= (MIN_FREQUENCY + CHANGE_FREQUENCY_VALUE)) {
        current_frequency -= CHANGE_FREQUENCY_VALUE;
    } else {
        current_frequency = MIN_FREQUENCY;
    }
    LED_Hardware_Update(); 
}


/* --- PUBLIC FUNCTIONS (BRIGHTNESS CONTROL) --- */

/**
 * @brief  Increments the LED brightness duty cycle by a predefined constant value.
 * @details Clamps the value to MAX_BRIGHTNESS if the threshold is exceeded.
 * @retval None
 */
void LED_brightness_increment_by_const_value(void) {
    current_brightness_pct += CHANGE_BRIGHTNESS_VALUE;
    if(current_brightness_pct > MAX_BRIGHTNESS) {
        current_brightness_pct = MAX_BRIGHTNESS;
    }
    LED_Hardware_Update(); 
}

/**
 * @brief  Decrements the LED brightness duty cycle by a predefined constant value.
 * @details Prevents underflow by safely clamping the value to MIN_BRIGHTNESS.
 * @retval None
 */
void LED_brightness_decrement_by_const_value(void) {
    if (current_brightness_pct >= CHANGE_BRIGHTNESS_VALUE) {
        current_brightness_pct -= CHANGE_BRIGHTNESS_VALUE;
    } else {
        current_brightness_pct = MIN_BRIGHTNESS;
    }
    LED_Hardware_Update(); 
}