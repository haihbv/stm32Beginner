#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Handler(void);
void SysTick_Init(uint32_t ms);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
uint32_t millis(void);
uint32_t micros(void);

// Non-blocking delay
void delay_ms_non_blocking(uint32_t ms);
uint8_t is_delay_done(void);

#endif
