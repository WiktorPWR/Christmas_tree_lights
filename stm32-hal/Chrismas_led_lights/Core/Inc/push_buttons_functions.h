/**
 * @file push_buttons_functions.h
 * @brief Header file for the push-button interrupt and debounce management module.
 * @details This file defines the structures, external variables, and functions 
 * used to handle button presses with a software/hardware hybrid debouncing logic.
 */

#ifndef PUSH_BUTTONS_FUNCTIONS_H
#define PUSH_BUTTONS_FUNCTIONS_H

#include "main.h"

/* --- STRUCTURES --- */

/**
 * @struct Button_pressed
 * @brief  Structure to store the context and validation state of a pressed button.
 */
struct Button_pressed {
    GPIO_TypeDef * GPIO_Port;       /**< Pointer to the GPIO Port register bank (e.g., GPIOA, GPIOB). */
    uint16_t GPIO_Pin;              /**< The specific GPIO Pin number (e.g., GPIO_PIN_0). */
    uint16_t pressed_confirmation;  /**< Status flag: set to 1 if the button press is validated after the debounce period. */
};


/* --- EXTERNAL VARIABLES --- */

/**
 * @brief Volatile structure instance tracking the currently active or pending button press.
 * @note  Shared globally across EXTI and Timer interrupt service routines.
 */
extern volatile struct Button_pressed actual_button_pressed;


/* --- PUBLIC FUNCTION PROTOTYPES --- */

/**
 * @brief  Enables GPIO external interrupts in the NVIC controller.
 * @details Re-enables the EXTI9_5_IRQn and EXTI15_10_IRQn interrupt vectors to 
 * listen for new button state transitions.
 * @retval None
 */
void Interrupts_GPIO_enable(void);

/**
 * @brief  Disables GPIO external interrupts in the NVIC controller.
 * @details Blocks the EXTI9_5_IRQn and EXTI15_10_IRQn interrupt vectors to prevent 
 * contact-bounce noise from re-triggering the system during processing.
 * @retval None
 */
void Interrupts_GPIO_disable(void);

#endif /* PUSH_BUTTONS_FUNCTIONS_H */