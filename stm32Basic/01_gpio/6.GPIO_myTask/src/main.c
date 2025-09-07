#include "assignment.h"

int main(void)
{
    GPIO_Config();

    uint32_t sysTickCounter = 0;

    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, (led3_state) ? Bit_RESET : Bit_SET);

    while (1)
    {
        Button_Debounced();
        BTN_StateHandler();
        if (sysTickCounter % 300 == 0)
        {
            GPIOC->ODR ^= GPIO_Pin_13;
        }

        if (sysTickCounter % 600 == 0)
        {
            GPIOB->ODR ^= GPIO_Pin_12;
        }
        delay_ms(10);
        sysTickCounter++;
        if (sysTickCounter >= 600)
        {
            sysTickCounter = 0;
        }
    }
}
