#include "stm32f10x.h" // Device header

void GPIO_Config(void);
void delay_ms(uint32_t timeout);
int main()
{
	GPIO_Config();
	while (1)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
		delay_ms(1000);
		GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
		delay_ms(1000);
	}
}

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void delay_ms(uint32_t timeout)
{
	volatile uint32_t i, j;
	for (i = 0; i < timeout; i++)
	{
		for (j = 0; j < 0x1995; j++)
		{
			__ASM("nop");
		}
	}
}
