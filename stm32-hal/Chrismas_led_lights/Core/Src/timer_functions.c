/**
 * @file push_buttons_callbacks.c
 * @brief Timer peripheral callback implementation for push-button debouncing.
 * @details This file handles the elapsed period interrupt of TIM3, which is used
 * as a software debounce timer for input buttons. Once a press is confirmed, 
 * it triggers the corresponding LED frequency, brightness, or power toggle action.
 */

#include "main.h"
#include "push_buttons_functions.h"
#include "led_functions.h"

/* --- EXTERNAL PERIPHERALS --- */

/** @brief Hardware Timer 4 handle used for LED PWM generation. */
extern TIM_HandleTypeDef htim4;

/** @brief Hardware Timer 3 handle used for button debounce timing. */
extern TIM_HandleTypeDef htim3;


/* --- STATE VARIABLES --- */

/** * @brief Global flag representing the current power state of the LEDs.
 * @details 0 = LEDs are OFF (PWM stopped), 1 = LEDs are ON (PWM active).
 */
volatile uint8_t LED_on_off_flag = 0;


/* --- CALLBACK FUNCTIONS --- */

/**
 * @brief  Period elapsed callback in non-blocking mode.
 * @param  htim: Pointer to a TIM_HandleTypeDef structure that contains
 * the configuration information for the TIM module.
 * @note   This function acts as the debouncing logic filter. It fires when TIM3 finishes 
 * its debounce period, verifies if the button is still pressed, and processes the event.
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* Check if the interrupt came from Timer 3 (Debounce Timer) */
    if(htim->Instance == TIM3) {
        
        /* Stop the debounce timer immediately to make it a one-shot event */
        HAL_TIM_Base_Stop_IT(&htim3);

        /* Verify if the button is still active (low state) after the debounce period */
        if(!HAL_GPIO_ReadPin(actual_button_pressed.GPIO_Port, actual_button_pressed.GPIO_Pin))
        {
            /* Button press successfully confirmed */
            actual_button_pressed.pressed_confirmation = 1;

            /* --- BUTTON ACTION DISPATCHER --- */

            /* UP Button: Increase LED blinking speed */
            if(actual_button_pressed.GPIO_Pin == Button_up_Pin) {
                LED_increase_frequency();
            }
            /* DOWN Button: Decrease LED blinking speed */
            else if(actual_button_pressed.GPIO_Pin == Button_down_Pin) {
                LED_decrease_frequency();
            }
            /* LEFT Button: Increase LED brightness */
            else if(actual_button_pressed.GPIO_Pin == Button_left_Pin) {
                LED_increase_all_brightness();
            }
            /* RIGHT Button: Decrease LED brightness */
            else if(actual_button_pressed.GPIO_Pin == Button_rigth_Pin) { // Note: original typo in macro retained
                LED_decrease_all_brightness();
            }
            /* MIDDLE Button: Toggle LED state (ON/OFF) */
            else if(actual_button_pressed.GPIO_Pin == Button_middle_Pin) {
                
                /* If LEDs are currently OFF, turn them ON with default parameters */
                if(LED_on_off_flag == 0) {
                    LED_power_ON();
                    LED_on_off_flag = 1;
                }
                /* If LEDs are currently ON, turn them OFF by stopping the PWM */
                else {
                    LED_power_OFF();
                    LED_on_off_flag = 0;
                }
            }
        }

        /* Re-enable external GPIO interrupts to listen for new button events */
        Interrupts_GPIO_enable();
        
        /* Reset the active button tracking structure to an idle state */
        actual_button_pressed = (struct Button_pressed){.GPIO_Port = NULL, .GPIO_Pin = 0};
    }
}