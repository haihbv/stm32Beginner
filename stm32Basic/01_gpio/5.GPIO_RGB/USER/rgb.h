#ifndef RGB_H
#define RGB_H

#include "stm32f10x.h" // Device header

#define RED_PIN         GPIO_Pin_0
#define GREEN_PIN       GPIO_Pin_1
#define BLUE_PIN        GPIO_Pin_2

#define RGB_PIN_MASK    RED_PIN | GREEN_PIN | BLUE_PIN

extern volatile uint32_t tick_ms;

void delay_ms(uint32_t ms);
void SysTick_Handler(void);

void GPIO_Config(void);
void LED_RGB_Set(uint8_t red, uint8_t green, uint8_t blue);
void LED_RGB_SetCommand(void);

#endif // RGB_H
