#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"                  // Device header

extern volatile uint32_t tick_us;

void SysTick_Init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif // __DELAY_H
