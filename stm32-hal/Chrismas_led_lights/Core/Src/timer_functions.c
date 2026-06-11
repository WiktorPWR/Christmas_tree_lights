#include "main.h"
#include "push_buttons_functions.h"
#include "led_functions.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM3){
        if(HAL_GPIO_ReadPin(actual_button_pressed.GPIO_Port, actual_button_pressed.GPIO_Pin))
        {
            // Button is still pressed
            actual_button_pressed.pressed_confirmation = 1;
            if(actual_button_pressed.GPIO_Pin == Button_up_Pin){
                LED_blinking_speed_changer_by_const_value();
            }
            else if(actual_button_pressed.GPIO_Pin == Button_down_Pin){
                LED_blinking_speed_decrement_by_const_value();
            }
            else if(actual_button_pressed.GPIO_Pin == Button_left_Pin){
                LED_brightness_increment_by_const_value();
            }
            else if(actual_button_pressed.GPIO_Pin == Button_rigth_Pin){
                LED_brightness_decrement_by_const_value();
            }
            else if(actual_button_pressed.GPIO_Pin == Button_middle_Pin){
                //This will on and off the LEDs
                if(htim4.Init.Period == 0){
                    htim4.Init.Period = 1000;
                }
                else{
                    htim4.Init.Period = 0;
                }
                // Handle Button Middle action
            }
        }
        else
        {
            // Button is not pressed anymore soo that was probalby some noise
            Interrupts_GPIO_enable();
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = NULL, .GPIO_Pin = 0};
        }
    }
}