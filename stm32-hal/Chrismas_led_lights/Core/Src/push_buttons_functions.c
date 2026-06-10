#include "stm32f4xx_hal_gpio.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    switch (GPIO_Pin) {
        case Button_up_Pin:
        // Handle Button Up press
        break;
        case Button_down_Pin:
        // Handle Button Down press
        break;
        case Button_left_Pin:
        // Handle Button Left press
        break;
        case Button_right_Pin:
        // Handle Button Right press
        break;
        case Button_middle_Pin:
        // Handle Button Middle press
        break;
        default:
        break;
    }
}