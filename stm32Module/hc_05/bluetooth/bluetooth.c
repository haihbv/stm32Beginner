#include "bluetooth.h"
#include "stm32f10x_usart.h"

USART_TypeDef *pADCx = USART1;

void BT_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef 	GPIO_InitStruct = {0};
	USART_InitTypeDef USART_InitStruct = {0};
	if (pADCx == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
		
		// PA9 - TX
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		// PA10 - RX
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else if (pADCx == USART2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		// PA2 - TX
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		// PA3 - RX
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else
	{
		return;
	}
	
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(pADCx, &USART_InitStruct);
	USART_Cmd(pADCx, ENABLE);
}
void BT_SendChar(char c)
{
	while (!(pADCx->SR & USART_FLAG_TXE));
	USART_SendData(pADCx, c);
}
void BT_SendStr(const char *pStr)
{
	while (*pStr)
	{
		BT_SendChar(*pStr++);
	}
}
char BT_GetChar(void)
{
	while (!(pADCx->SR & USART_FLAG_RXNE));
	return (uint8_t)USART_ReceiveData(pADCx);
}



