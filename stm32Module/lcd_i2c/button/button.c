#include "button.h"

void Button_Init(void)
{
	RCC_APB2PeriphClockCmd(BUTTON_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = BUTTON_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
}
uint8_t Button_Read(void)
{
	return (GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN) == Bit_RESET);
}
