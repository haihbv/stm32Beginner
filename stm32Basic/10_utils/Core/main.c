#include "main.h"

static void TIM_PWMConfig(void)
{
	TIM_PWMConfigTypeDef TIM_PWMConfigStruct;
	TIM_PWMConfigStruct.Prescaler = 72 - 1;
	TIM_PWMConfigStruct.Period = 1000 - 1;
	TIM_PWMConfigStruct.Pulse = 0;
	TIM_PWMConfigStruct.Channel = 1;
	TIMER_PWM_Init(TIM1, &TIM_PWMConfigStruct);
	TIMER_Start(TIM1);
}

int main(void)
{
	SystemInit();
	Delay_Init();
	GPIOx_Init(GPIOA, GPIO_Pin_8, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	TIM_PWMConfig();
	while (1)
	{
		TIMER_PWMLed(TIM1);
		DelayMs(25);
	}
}
