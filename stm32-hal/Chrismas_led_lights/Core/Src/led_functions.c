#include "led_functions.h"
#include "main.h"

#define MAX_BRIGHTNESS 100
#define MIN_BRIGHTNESS 0
#define MAX_FREQUENCY 100
#define MIN_FREQUENCY 1

#define CHANGE_FREQUENCY_VALUE 10
#define CHANGE_BRIGHTNESS_VALUE 10

extern TIM_HandleTypeDef htim4;

#define START_BRIGHTNESS_PCT 50
#define START_FREQUENCY 1

// Globalna zmienna przechowująca stan jasności w procentach (0-100)
volatile static uint16_t current_brightness_pct; 
volatile static uint16_t current_frequency; // Przykładowa początkowa częstotliwość w Hz

static void LED_Hardware_Update(void) {
    // 1. Przeliczenie i ustawienie nowej częstotliwości (ARR)
    uint16_t new_arr_value = ((SystemCoreClock * 4) / (current_frequency * (htim4.Init.Prescaler + 1))) - 1;
    __HAL_TIM_SET_AUTORELOAD(&htim4, new_arr_value);
    htim4.Init.Period = new_arr_value; // Aktualizacja struktury do obliczeń poniżej

    // 2. Przeliczenie i ustawienie impulsu (CCR) z zachowaniem stałego procentu jasności
    uint16_t new_pulse = (current_brightness_pct * new_arr_value) / 100;
    
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, new_pulse);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, new_pulse);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, new_pulse);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, new_pulse);
}

// --- OBSŁUGA CZĘSTOTLIWOŚCI ---

void LED_set_init_state() {
    current_brightness_pct = START_BRIGHTNESS_PCT; // Ustawienie początkowej jasności na 50%
    current_frequency = START_FREQUENCY; // Ustawienie początkowej częstotliwości na 10 Hz
    LED_Hardware_Update(); // Aktualizacja sprzętu z nowymi wartościami
}

void LED_blinking_speed_increment_by_const_value(){
    current_frequency += CHANGE_FREQUENCY_VALUE;
    if(current_frequency > MAX_FREQUENCY) {
        current_frequency = MAX_FREQUENCY;
    }
    LED_Hardware_Update(); // Aktualizujemy sprzęt
}

void LED_blinking_speed_decrement_by_const_value(){
    if (current_frequency >= (MIN_FREQUENCY + CHANGE_FREQUENCY_VALUE)) {
        current_frequency -= CHANGE_FREQUENCY_VALUE;
    } else {
        current_frequency = MIN_FREQUENCY;
    }
    LED_Hardware_Update(); // Aktualizujemy sprzęt
}

// --- OBSŁUGA JASNOŚCI ---

void LED_brightness_increment_by_const_value(){
    current_brightness_pct += CHANGE_BRIGHTNESS_VALUE;
    if(current_brightness_pct > MAX_BRIGHTNESS) {
        current_brightness_pct = MAX_BRIGHTNESS;
    }
    LED_Hardware_Update(); // Aktualizujemy sprzęt
}

void LED_brightness_decrement_by_const_value(){
    if (current_brightness_pct >= CHANGE_BRIGHTNESS_VALUE) {
        current_brightness_pct -= CHANGE_BRIGHTNESS_VALUE;
    } else {
        current_brightness_pct = MIN_BRIGHTNESS;
    }
    LED_Hardware_Update(); // Aktualizujemy sprzęt
}