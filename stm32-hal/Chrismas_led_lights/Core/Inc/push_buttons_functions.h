#ifndef PUSH_BUTTONS_FUNCTIONS_H
#define PUSH_BUTTONS_FUNCTIONS_H

#include "main.h"

void Interrupts_GPIO_enable();

struct Button_pressed{
    GPIO_TypeDef * GPIO_Port;
    uint16_t GPIO_Pin;
    //if this is one then we know that the button is pressed correctly
    uint16_t pressed_confirmation;
};

extern volatile struct Button_pressed actual_button_pressed;

#endif
