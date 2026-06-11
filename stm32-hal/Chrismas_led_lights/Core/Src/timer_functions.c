#include "main.h"
#include "push_buttons_functions.h"
#include "led_functions.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM3){
        if(HAL_GPIO_ReadPin(actual_button_pressed.GPIO_Port, actual_button_pressed.GPIO_Pin))
        {
            // Button is still pressed
            actual_button_pressed.pressed_confirmation = 1;
            
        }
        else
        {
            // Button is not pressed anymore soo that was probalby some noise
            Interrupts_GPIO_enable();
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = NULL, .GPIO_Pin = 0};
        }
    }
}