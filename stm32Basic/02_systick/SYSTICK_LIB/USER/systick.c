#include "systick.h"

static volatile uint32_t sysTickCounter = 0;
static volatile uint32_t nonBlockingStart = 0;
static volatile uint32_t nonBlockingDelay = 0;
static volatile uint8_t isWaiting = 0;

void SysTick_Handler(void)
{
    sysTickCounter++;
}

void SysTick_Init(uint32_t ms)
{
    SysTick_Config(SystemCoreClock / (1000 / ms)); // interrupt m?i ms
}

void delay_ms(uint32_t ms)
{
    uint32_t start = sysTickCounter;
    while ((sysTickCounter - start) < ms)
        ;
}

void delay_us(uint32_t us)
{
    for (uint32_t i = 0; i < us * 9; i++)
    {
        __NOP(); // x?p x? v?i HCLK 72MHz
    }
}

uint32_t millis(void)
{
    return sysTickCounter;
}

uint32_t micros(void)
{
    uint32_t m;
    uint32_t val;
    do
    {
        m = sysTickCounter;
        val = SysTick->VAL;
    } while (m != sysTickCounter);

    uint32_t ticks_per_us = SystemCoreClock / 1000000; // 72
    return (m * 1000) + ((SysTick->LOAD - val) / ticks_per_us);
}

void delay_ms_non_blocking(uint32_t ms)
{
    nonBlockingStart = sysTickCounter;
    nonBlockingDelay = ms;
    isWaiting = 1;
}

uint8_t is_delay_done(void)
{
    if (isWaiting && (sysTickCounter - nonBlockingStart) >= nonBlockingDelay)
    {
        isWaiting = 0;
        return 1;
    }
    return 0;
}
