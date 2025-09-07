#include "delay.h"

static volatile uint32_t _millis = 0;

void Delay_Init(void)
{
	SysTick->LOAD = 72000 - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;				
}
uint32_t millis(void)
{
	return _millis;
}
void Delay_ms(uint32_t ms)
{
	uint32_t start = millis();
	while ((millis() - start) < ms);
}
void Delay_us(uint32_t us)
{
	uint32_t start = SysTick->VAL;
	uint32_t ticks = (uint32_t)us * 72;
	uint32_t reload = SysTick->LOAD + 1;
	
	while (ticks > 0)
	{
		uint32_t now = SysTick->VAL;
		uint32_t elapsed = (start >= now) ? (start - now) : (reload + start - now);
		
		if (elapsed >= ticks)
		{
			break;
		}
		
		ticks -= elapsed;
		start = now;
	}
}

void SysTick_Handler(void)
{
	_millis++;
}
