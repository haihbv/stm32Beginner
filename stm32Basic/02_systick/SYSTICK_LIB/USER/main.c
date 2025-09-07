#include "systick.h"

int main(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_13;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &gpio);

    SysTick_Init(1);

    delay_ms_non_blocking(1000);

    while (1)
    {
        if (is_delay_done())
        {
            GPIOC->ODR ^= GPIO_Pin_13;
            delay_ms_non_blocking(1000);
        }

        // Xu ly khac
    }
}
