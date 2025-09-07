#include "stm32f10x.h"	   // Device header
#include "stm32f10x_tim.h" // Keil::Device:StdPeriph Drivers:TIM

void GPIO_Config(void);
void TIM1_Config(void);
void TIM1_UP_IRQHandler(void);

int main()
{
	GPIO_Config();
	TIM1_Config();
	while (1)
	{
	}
}

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &gpio);
}
void TIM1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef tim;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Period = 10000 - 1;
	tim.TIM_Prescaler = 7200 - 1;
	tim.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &tim);

	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); // UIE: update interrupt enable

	TIM_Cmd(TIM1, ENABLE);

	TIM_CtrlPWMOutputs(TIM1, ENABLE); // bat buoc voi TIM1, ko dung PWM van phai bat de timer hoat dong dung

	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = TIM1_UP_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET) // UIF: update interrupt flag & UIE
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update); // clear UIF
		GPIOC->ODR ^= GPIO_Pin_13;
	}
}
