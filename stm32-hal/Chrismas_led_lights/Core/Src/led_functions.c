#include "led_functions.h"
#include "main.h"

#define MAX_BRIGHTNESS 100
#define MIN_BRIGHTNESS 0
#define MAX_FREQUENCY 20000
#define MIN_FREQUENCY 1

#define CHANGE_FREQUENCY_VALUE 100
#define CHANGE_BRIGHTNESS_VALUE 10

extern TIM_HandleTypeDef htim4;


void LED_blinking_speed_changer(uint16_t new_frequency){
    if(new_frequency > MAX_FREQUENCY){
        new_frequency = MAX_FREQUENCY;
    }
    else if(new_frequency < MIN_FREQUENCY){
        new_frequency = MIN_FREQUENCY;
    }
    //we change the period of the timer to change the blinking speed of the LEDs
    htim4.Init.Period = new_frequency;
    if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
    {
        Error_Handler();
    }
}

void LED_blinking_speed_changer_by_const_value(){
    uint16_t new_frequency = htim4.Init.Period + CHANGE_FREQUENCY_VALUE;
    LED_blinking_speed_changer(new_frequency);
}

void LED_blinking_speed_decrement_by_const_value(){
    uint16_t new_frequency = htim4.Init.Period - CHANGE_FREQUENCY_VALUE;
    LED_blinking_speed_changer(new_frequency);
}

//new brigthness is a value from 0 to 100
void LED_brightness_changer(uint16_t new_brightness){
    if(new_brightness > MAX_BRIGHTNESS){
        new_brightness = MAX_BRIGHTNESS;
    }
    else if(new_brightness < MIN_BRIGHTNESS){
        new_brightness = MIN_BRIGHTNESS;
    }
    //we change the pulse of the timer to change the brightness of the LEDs
    uint16_t new_pulse = (new_brightness / 100) * htim4.Init.Period;
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,new_pulse);
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,new_pulse);
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,new_pulse);
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,new_pulse);
}

void LED_brightness_increment_by_const_value(){
    uint16_t new_brightness = ((htim4.Init.Period / 100) * htim4.Init.Period) + CHANGE_BRIGHTNESS_VALUE;
    LED_brightness_changer(new_brightness);
}

void LED_brightness_decrement_by_const_value(){
    uint16_t new_brightness = ((htim4.Init.Period / 100) * htim4.Init.Period) - CHANGE_BRIGHTNESS_VALUE;
    LED_brightness_changer(new_brightness);
}