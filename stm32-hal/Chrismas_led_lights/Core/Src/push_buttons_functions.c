//#include "stm32f4xx_hal_gpio.h"
#include "push_buttons_functions.h"

extern TIM_HandleTypeDef htim3;

volatile struct Button_pressed actual_button_pressed;

void Interrupts_GPIO_disable(){
    NVIC_DisableIRQ(EXTI9_5_IRQn);
    NVIC_DisableIRQ(EXTI15_10_IRQn);
}

void Interrupts_GPIO_enable(){
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

    actual_button_pressed = (struct Button_pressed){.GPIO_Port = NULL, .GPIO_Pin = 0, .pressed_confirmation = 0};

    switch (GPIO_Pin) {
        case Button_up_Pin:
            // Handle Button Up press
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_up_GPIO_Port, .GPIO_Pin = Button_up_Pin};
            break;
        case Button_down_Pin:
            // Handle Button Down press
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_down_GPIO_Port, .GPIO_Pin = Button_down_Pin};
            break;
        case Button_left_Pin:
            // Handle Button Left press
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_left_GPIO_Port, .GPIO_Pin = Button_left_Pin};
            break;
        case Button_rigth_Pin:
            // Handle Button Right press
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_rigth_GPIO_Port, .GPIO_Pin = Button_rigth_Pin};
            break;
        case Button_middle_Pin:
            // Handle Button Middle press
            actual_button_pressed = (struct Button_pressed){.GPIO_Port = Button_middle_GPIO_Port, .GPIO_Pin = Button_middle_Pin};
            break;
        default:
            break;
    }

    //WE disable interrupts to avoid multiple interrupts while we are processing the button press, and we start a timer to re-enable the interrupts after a short delay (debouncing)
    Interrupts_GPIO_disable();
    //Then we activate timer 
    HAL_TIM_Base_Start_IT(&htim3);
    
}

void Button_function_selector(struct Button_pressed button){
    if(button.GPIO_Pin == Button_up_Pin){
        // Handle Button Up action
    }
    else if(button.GPIO_Pin == Button_down_Pin){
        // Handle Button Down action
    }
    else if(button.GPIO_Pin == Button_left_Pin){
        // Handle Button Left action
    }
    else if(button.GPIO_Pin == Button_rigth_Pin){
        // Handle Button Right action
    }
    else if(button.GPIO_Pin == Button_middle_Pin){
        // Handle Button Middle action
    }

}