#include "main.h"

static void TIM_PWMConfig(void)
{
	TIM_PWMConfigTypeDef TIM_PWMConfigStruct;
	TIM_PWMConfigStruct.Prescaler = 72 - 1;
	TIM_PWMConfigStruct.Period = 20000 - 1;
	TIM_PWMConfigStruct.Pulse = 1500;
	TIM_PWMConfigStruct.Channel = 1;
	TIMER_PWM_Init(TIM2, &TIM_PWMConfigStruct);
	TIMER_Start(TIM2);
}

int main(void)
{
	SystemInit();
	Delay_Init();
	GPIOx_Init(GPIOA, GPIO_Pin_0, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	TIM_PWMConfig();
	while (1)
	{
		TIM_SetCompare1(TIM2, 500);
		DelayMs(1000);
		
		TIM_SetCompare1(TIM2, 1500);
		DelayMs(1000);
		
		TIM_SetCompare1(TIM2, 2500);
		DelayMs(1000);
	}
}
