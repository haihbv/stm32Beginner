#include "delay.h"

static inline void SysTick_Stop(void)
{
    SysTick->CTRL = 0;
}

static inline void SysTick_Start(uint32_t ticks)
{
    SysTick->LOAD = ticks - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Init(void) {}

void delay_ms(uint32_t ms)
{
    SysTick_Start(SystemCoreClock / 1000); // 1ms ticks
    for (uint32_t i = 0; i < ms; i++)
    {
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
            ;
    }
    SysTick_Stop();
}
