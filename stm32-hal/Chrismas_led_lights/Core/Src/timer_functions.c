#include "main.h"
#include "push_buttons_functions.h"



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM3){
        if(HAL_GPIO_ReadPin(actual_button_pressed.GPIO_Port, actual_button_pressed.GPIO_Pin))
        {
            // Button is still pressed
        }
        else
        {
            // Button is not pressed anymore soo that was probalby some noise
        }
    }
}