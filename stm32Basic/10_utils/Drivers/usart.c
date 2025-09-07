#include "usart.h"
#include <stdio.h>

USART_Handle_t UART1;
USART_Handle_t UART2;

static void RingBuffer_Put(RingBuffer_t *rb, char c)
{
	uint16_t next = (rb->head + 1) % RX_BUFFER_SIZE;
	if (next != rb->tail)
	{
		rb->buffer[rb->head] = c;
		rb->head = next;
	}
}

static char RingBuffer_Get(RingBuffer_t *rb)
{
	if (rb->head == rb->tail)
	{
		return 0;
	}
	char c = rb->buffer[rb->tail];
	rb->tail = (rb->tail + 1) % RX_BUFFER_SIZE;
	return c;
}

static void USART_SendByte(USART_Handle_t *huart, uint8_t data)
{
	while (USART_GetFlagStatus(huart->Instance, USART_FLAG_TXE) == RESET);
	USART_SendData(huart->Instance, data);
}

static void USART_SendString(USART_Handle_t *huart, const char *str)
{
	while (*str)
	{
		huart->sendByte(huart, *str++);
	}
}

static int USART_Available(USART_Handle_t *huart)
{
	return (huart->rxBuffer.head != huart->rxBuffer.tail);
}

static char USART_GetChar(USART_Handle_t *huart)
{
	return RingBuffer_Get(&huart->rxBuffer);
}

void USART_InitHandle(USART_Handle_t *huart, USART_TypeDef *USARTx, uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	huart->Instance = USARTx;
  huart->rxBuffer.head = 0;
  huart->rxBuffer.tail = 0;
	
	huart->sendByte = USART_SendByte;
	huart->sendString = USART_SendString;
	huart->available = USART_Available;
	huart->getChar = USART_GetChar;
	
	if (USARTx == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
		
		/* PA9 = TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* PA10 = RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	}
	else if (USARTx == USART2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		/* PA2 = TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* PA3 = RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USARTx, &USART_InitStructure);
  USART_Cmd(USARTx, ENABLE);
	
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		char c = (char)USART_ReceiveData(USART1);
		RingBuffer_Put(&UART1.rxBuffer, c);
	}
}

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		char c = (char)USART_ReceiveData(USART2);
		RingBuffer_Put(&UART2.rxBuffer, c);
	}
}

__attribute__((noreturn, unused)) static void _sys_exit(int x)
{
  (void)x;
  while (1);
}

static USART_Handle_t *StdOut = &UART1;
static USART_Handle_t *StdIn  = &UART1;

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

FILE __stdout;

PUTCHAR_PROTOTYPE
{
	(void)f;
	if (ch == '\n')
	{
		StdOut->sendByte(StdOut, '\r');
	}
	StdOut->sendByte(StdOut, (uint8_t)ch);
	return ch;
}

#define GETCHAR_PROTOTYPE int fgetc(FILE *f)

GETCHAR_PROTOTYPE
{
	(void)f;
	char c = 0;
	while (!StdIn->available(StdOut));
	
	c = StdIn->getChar(StdIn);
	StdOut->sendByte(StdOut, c);
	return c;
}
