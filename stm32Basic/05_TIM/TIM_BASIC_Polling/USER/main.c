#include "stm32f10x.h" // Device header

void GPIO_Config(void);
void TIM2_Init(void);
void Delay_ms(uint32_t ms);

int main()
{
	GPIO_Config();
	TIM2_Init();
	while (1)
	{
		GPIOC->ODR ^= GPIO_Pin_13;
		Delay_ms(1000);
	}
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void TIM2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_RepetitionCounter = 0;
	TIM_InitStruct.TIM_Period = 1000 - 1;
	TIM_InitStruct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2, ENABLE);
}
void Delay_ms(uint32_t ms)
{
	for (volatile uint32_t i = 0; i < ms; i++)
	{
		TIM_SetCounter(TIM2, 0);
		while (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == RESET)
			;
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	}
}