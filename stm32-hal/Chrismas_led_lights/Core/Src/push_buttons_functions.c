/**
 * @file push_buttons_functions.c
 * @brief External interrupt handles and control functions for push-buttons.
 * @details This module manages GPIO external interrupt callbacks (EXTI) for button presses.
 * It implements a hardware/software hybrid debouncing mechanism by disabling NVIC 
 * lines immediately upon detecting an edge and starting a debounce timer (TIM3).
 */

#include "push_buttons_functions.h"

/* --- EXTERNAL PERIPHERALS --- */

/** @brief Hardware Timer 3 handle used for button debounce timing. */
extern TIM_HandleTypeDef htim3;


/* --- GLOBAL VARIABLES --- */

/** * @brief Volatile structure tracking the context of the currently pressed button.
 * @details Shared between the EXTI callback context and the Timer 3 period elapsed callback.
 */
volatile struct Button_pressed actual_button_pressed;


/* --- PUBLIC FUNCTIONS (INTERRUPT MANAGEMENT) --- */

/**
 * @brief  Disables GPIO external interrupts in the NVIC controller.
 * @details Blocks further interrupt requests from EXTI lines 5-9 and 10-15 to lock out 
 * bounce noise while a button press is being verified.
 * @retval None
 */
void Interrupts_GPIO_disable(void) {
    NVIC_DisableIRQ(EXTI9_5_IRQn);
    NVIC_DisableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief  Enables GPIO external interrupts in the NVIC controller.
 * @details Re-enables handling of interrupt requests from EXTI lines 5-9 and 10-15 
 * allowing the system to detect new button press events.
 * @retval None
 */
void Interrupts_GPIO_enable(void) {
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}


/* --- EXTI CALLBACK FUNCTIONS --- */

/**
 * @brief  EXTI line detection callback.
 * @param  GPIO_Pin: Specifies the pin connected to the EXTI line.
 * @details Triggered by a hardware edge transition (e.g., falling edge) on any button pin.
 * It identifies which button caused the event, stores its port and pin configuration,
 * disables further external interrupts, and fires TIM3 to start the debouncing delay window.
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    /* Clear and initialize the tracking structure for a new button event */
    actual_button_pressed = (struct Button_pressed){.GPIO_Port = NULL, .GPIO_Pin = 0, .pressed_confirmation = 0};

    /* Identify the active button pin and save its corresponding hardware port */
    switch (GPIO_Pin) {
        case Button_up_Pin:
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_up_GPIO_Port, .GPIO_Pin = Button_up_Pin};
            break;
        case Button_down_Pin:
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_down_GPIO_Port, .GPIO_Pin = Button_down_Pin};
            break;
        case Button_left_Pin:
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_left_GPIO_Port, .GPIO_Pin = Button_left_Pin};
            break;
        case Button_rigth_Pin: // Note: original typo in macro retained for compatibility
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_rigth_GPIO_Port, .GPIO_Pin = Button_rigth_Pin};
            break;
        case Button_middle_Pin:
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_middle_GPIO_Port, .GPIO_Pin = Button_middle_Pin};
            break;
        default:
            /* Ignore interrupts coming from unmapped pins */
            break;
    }

    /**
          * Disable EXTI interrupts to avoid capturing multiple contact-bounce noise signals 
     * during physical switch transitions, then start the debounce validation timer.
     */
    Interrupts_GPIO_disable();
    HAL_TIM_Base_Start_IT(&htim3);
}