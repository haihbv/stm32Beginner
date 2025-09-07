#include "delay.h"

volatile uint32_t tick_us = 0;

void SysTick_Init(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000000);
}
void delay_us(uint32_t us)
{
	tick_us = us;
	while (tick_us != 0)
	{
		__NOP();
	}
}
void delay_ms(uint32_t ms)
{
	while (ms)
	{
		delay_us(1000);
		--ms;
	}
}
void SysTick_Handler(void)
{
	if (tick_us > 0)
	{
		tick_us--;
	}
}
