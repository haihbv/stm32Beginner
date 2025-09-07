#ifndef __DELAY_H_
#define __DELAY_H_

#include "stm32f10x.h"

#define SYSTEM_CORE_CLOCK 72000000UL

void SysTick_Init(void);
void delay_ms(uint32_t ms);

#endif // __DELAY_H_
