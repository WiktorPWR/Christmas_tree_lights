#include "main.h"
#include "push_buttons_functions.h"
#include "led_functions.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim3;

volatile uint8_t LED_on_off_flag = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM3){
        HAL_TIM_Base_Stop_IT(&htim3);

        if(!HAL_GPIO_ReadPin(actual_button_pressed.GPIO_Port, actual_button_pressed.GPIO_Pin))
        {
            // Button is still pressed
            actual_button_pressed.pressed_confirmation = 1;
            if(actual_button_pressed.GPIO_Pin == Button_up_Pin){
                LED_blinking_speed_increment_by_const_value();
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
                if(LED_on_off_flag == 0){
                    LED_brightness_increment_by_const_value();
                    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
                    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
                    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
                    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
                    LED_on_off_flag = 1;
                }
                else{
                    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
                    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
                    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
                    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
                    LED_on_off_flag = 0;
                }
                // Handle Button Middle action
            }
        }

        // Button is not pressed anymore soo that was probalby some noise
        Interrupts_GPIO_enable();
        actual_button_pressed = (struct Button_pressed){.GPIO_Port = NULL, .GPIO_Pin = 0};
    }
}