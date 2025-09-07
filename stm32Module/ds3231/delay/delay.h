#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"                  // Device header

void Delay_Init(void);
void Delay_ms(uint32_t ms);
void Delay_us(uint32_t us);
uint32_t millis(void);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
