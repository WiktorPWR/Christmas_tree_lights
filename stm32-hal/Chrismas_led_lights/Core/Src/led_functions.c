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

#define TIMER_INPUT_CLOCK_HZ 84000000

/* --- EXTERNAL PERIPHERALS --- */

/** @brief Hardware Timer 4 handle defined in main.c */
extern TIM_HandleTypeDef htim4;


/* --- STATE VARIABLES --- */

/** @brief Current brightness duty cycle, represented as a percentage (0-100%). */
union LED_Brightness{
    uint8_t leds_brightness[4];
    uint32_t led_brightness_all;

};

volatile static union LED_Brightness brightness = {0};

// volatile static uint16_t current_brightness_led_1;
// volatile static uint16_t current_brightness_led_2;
// volatile static uint16_t current_brightness_led_3;
// volatile static uint16_t current_brightness_led_4; 

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
    uint16_t new_arr_value = ( TIMER_INPUT_CLOCK_HZ / (current_frequency * (htim4.Init.Prescaler + 1))) - 1;
    __HAL_TIM_SET_AUTORELOAD(&htim4, new_arr_value);

    /* 2. Recalculate and set the pulse width (CCR) keeping a constant brightness percentage */
    uint16_t new_pulse=0;
    
    for(uint8_t i=0; i<4; i++){
        if(brightness.leds_brightness[i] == MAX_BRIGHTNESS){
            new_pulse = new_arr_value;
        }
        else if(brightness.leds_brightness[i] == MIN_BRIGHTNESS){
            new_pulse = 0;
        }
        else {
            new_pulse = ((uint32_t)brightness.leds_brightness[i] * (new_arr_value + 1)) / 100;
        }

        switch(i){
            case 0:
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, new_pulse);
                break;
            case 1:
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, new_pulse);
                break;
            case 2:
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, new_pulse);
                break;
            case 3:
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, new_pulse);
                break;
        }
    }


    htim4.Instance->EGR = TIM_EGR_UG; // Generate an update event to apply changes immediately
}


/* --- PUBLIC FUNCTIONS (FREQUENCY CONTROL) --- */


void LED_increase_frequency(void) {
    if (current_frequency + CHANGE_FREQUENCY_VALUE <= MAX_FREQUENCY) {
        current_frequency += CHANGE_FREQUENCY_VALUE;
        LED_Hardware_Update();
    }
}

void LED_decrease_frequency(void) {
    if (current_frequency - CHANGE_FREQUENCY_VALUE >= MIN_FREQUENCY) {
        current_frequency -= CHANGE_FREQUENCY_VALUE;
        LED_Hardware_Update();
    }
}

void LED_set_frequency(uint16_t frequency) {
    if (frequency >= MIN_FREQUENCY && frequency <= MAX_FREQUENCY) {
        current_frequency = frequency;
        LED_Hardware_Update();
    }
}

/* --- PUBLIC FUNCTIONS (BRIGHTNESS CONTROL) --- */

void LED_increase_brightness(uint8_t led_index) {
    if (led_index < 4 && brightness.leds_brightness[led_index] + CHANGE_BRIGHTNESS_VALUE <= MAX_BRIGHTNESS) {
        brightness.leds_brightness[led_index] += CHANGE_BRIGHTNESS_VALUE;
        LED_Hardware_Update();
    }else if (led_index < 4 && brightness.leds_brightness[led_index] + CHANGE_BRIGHTNESS_VALUE > MAX_BRIGHTNESS) {
        brightness.leds_brightness[led_index] = MAX_BRIGHTNESS;
        LED_Hardware_Update();
    }
}

void LED_decrease_brightness(uint8_t led_index) {
    if (led_index < 4 && brightness.leds_brightness[led_index] >= CHANGE_BRIGHTNESS_VALUE) {
        brightness.leds_brightness[led_index] -= CHANGE_BRIGHTNESS_VALUE;
        LED_Hardware_Update();
    }else if (led_index < 4 && brightness.leds_brightness[led_index] < CHANGE_BRIGHTNESS_VALUE) {
        brightness.leds_brightness[led_index] = MIN_BRIGHTNESS;
        LED_Hardware_Update();
    }
}

void LED_set_brightness(uint8_t led_index, uint8_t brightness_value) {
    if (led_index < 4 && brightness_value >= MIN_BRIGHTNESS && brightness_value <= MAX_BRIGHTNESS) {
        brightness.leds_brightness[led_index] = brightness_value;
        LED_Hardware_Update();
    }
}

void LED_set_all_brightness(uint8_t brightness_value) {
    if (brightness_value >= MIN_BRIGHTNESS && brightness_value <= MAX_BRIGHTNESS) {
        for (uint8_t i = 0; i < 4; i++) {
            brightness.leds_brightness[i] = brightness_value;
        }
        LED_Hardware_Update();
    }
}

void LED_increase_all_brightness(void) {
    for (uint8_t i = 0; i < 4; i++) {
        if (brightness.leds_brightness[i] + CHANGE_BRIGHTNESS_VALUE <= MAX_BRIGHTNESS) {
            brightness.leds_brightness[i] += CHANGE_BRIGHTNESS_VALUE;
        } else {
            brightness.leds_brightness[i] = MAX_BRIGHTNESS;
        }
    }
    LED_Hardware_Update();
}


void LED_decrease_all_brightness(void) {
    for (uint8_t i = 0; i < 4; i++) {
        if (brightness.leds_brightness[i] >= CHANGE_BRIGHTNESS_VALUE) {
            brightness.leds_brightness[i] -= CHANGE_BRIGHTNESS_VALUE;
        } else {
            brightness.leds_brightness[i] = MIN_BRIGHTNESS;
        }
    }
    LED_Hardware_Update();
}

void LED_power_ON(void){
    current_frequency = START_FREQUENCY; 
    brightness.led_brightness_all = 0xFFFFFFFF; // Set all LEDs to maximum brightness
    LED_Hardware_Update();
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void LED_power_OFF(void){ 
    brightness.led_brightness_all = 0x00000000; // Set all LEDs to minimum brightness
    LED_Hardware_Update();
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
}

