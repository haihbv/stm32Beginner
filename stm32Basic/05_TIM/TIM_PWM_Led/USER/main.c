#include "stm32f10x.h"	   // Device header
#include "stm32f10x_tim.h" // Keil::Device:StdPeriph Drivers:TIM

void GPIO_Config(void);
void TIM2_PWM_Init(uint16_t period);
void delay_ms(uint32_t ms);

int main()
{
	uint16_t duty = 0;
	uint16_t step = 50;

	GPIO_Config();
	TIM2_PWM_Init(1000);

	while (1)
	{
		TIM_SetCompare1(TIM2, duty);
		duty += step;

		if (duty >= 1000)
		{
			duty = 1000;
			step = -step;
		}
		else if (duty <= 0)
		{
			duty = 0;
			step = -step;
		}
		delay_ms(100);
	}
}

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void TIM2_PWM_Init(uint16_t period)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = period;
	TIM_InitStruct.TIM_Prescaler = 72 - 1;
	TIM_InitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);

	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);

	TIM_OC1PreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint32_t ms)
{
	volatile uint32_t i, j;
	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 0x1995; j++)
			;
	}
}
