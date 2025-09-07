#include "usart.h"

static void USART_GPIO_Init(USART_TypeDef *USARTx)
{
	GPIO_InitTypeDef USART_Gpio;
	
	if (USARTx == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		
		// PA9
		USART_Gpio.GPIO_Pin = GPIO_Pin_9;
		USART_Gpio.GPIO_Speed = GPIO_Speed_50MHz;
		USART_Gpio.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &USART_Gpio);
		
		// PA10
		USART_Gpio.GPIO_Pin = GPIO_Pin_10;
		USART_Gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &USART_Gpio);
	}
	else if (USARTx == USART2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		// PA2
		USART_Gpio.GPIO_Pin = GPIO_Pin_2;
		USART_Gpio.GPIO_Speed = GPIO_Speed_50MHz;
		USART_Gpio.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &USART_Gpio);
		
		// PA3
		USART_Gpio.GPIO_Pin = GPIO_Pin_3;
		USART_Gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &USART_Gpio);
	}
	else if (USARTx == USART3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
		// PB10 - TX
		USART_Gpio.GPIO_Pin = GPIO_Pin_10;
		USART_Gpio.GPIO_Speed = GPIO_Speed_50MHz;
		USART_Gpio.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &USART_Gpio);
		
		// PB11 - RX
		USART_Gpio.GPIO_Pin = GPIO_Pin_11;
		USART_Gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &USART_Gpio);
	}
	else
	{
		return;
	}
	
}
void USARTx_Init(USART_TypeDef *USARTx, uint32_t baudrate)
{
	USART_GPIO_Init(USARTx);
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USARTx, &USART_InitStruct);
	
	USART_Cmd(USARTx, ENABLE);
}

void USART_SendChar(USART_TypeDef *USARTx, char c)
{
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USARTx->DR = (uint16_t)c;
}

void USART_SendStr(USART_TypeDef *USARTx, const char *str)
{
	while (*str)
	{
		USART_SendChar(USARTx, *str++);
	}
}

uint8_t USART_ReceiveChar(USART_TypeDef *USARTx)
{
	while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	return (uint8_t)USARTx->DR;
}

void USART_ReceiveStr(USART_TypeDef *USARTx, char *buffer, uint16_t length)
{
	uint16_t i = 0;
	char c;
	
	while (1)
	{
		c = (char)USART_ReceiveChar(USARTx);
		
		if (c == '\n' || c == '\r' || i >= (length - 1))
		{
			buffer[i] = '\0';
			break;
		}
		else
		{
			buffer[i++] = c;
		}
	}
}

