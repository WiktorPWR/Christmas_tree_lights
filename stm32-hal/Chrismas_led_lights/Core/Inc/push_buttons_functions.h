#ifndef PUSH_BUTTONS_FUNCTIONS_H
#define PUSH_BUTTONS_FUNCTIONS_H

#include "main.h"

void Interrupts_GPIO_enable();

struct Button_pressed{
    GPIO_TypeDef * GPIO_Port;
    uint16_t GPIO_Pin;
}

extern volatile struct Button_pressed actual_button_pressed;

#endif
