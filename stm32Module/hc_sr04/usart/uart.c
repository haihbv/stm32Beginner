#include "uart.h"
#include <stdio.h>
#include <stdarg.h>

UART_TypedefStruct UART1;
UART_TypedefStruct UART2;
UART_TypedefStruct UART3;

typedef enum
{
	U1 = 0x00,
	U2,
	U3

} UARTx_e;

static volatile uint8_t UARTx_dataReceived[3][130] = {"0\0", "0\0", "0\0"};
static volatile uint8_t UARTx_rx_flag[3] = {0, 0, 0};
static volatile uint8_t UARTx_idx[3] = {0, 0, 0};
static char number[50];

void UARTx_Init(USART_TypeDef *UART, uint32_t baudrate, uint8_t remap);
void UARTx_SendData(USART_TypeDef *UART, const char *str, va_list args);
uint8_t UARTx_ReceiveData(UARTx_e Ux, uint8_t *data);

void UART1_Init(uint32_t baudrate, uint8_t remap);
void UART2_Init(uint32_t baudrate, uint8_t remap);
void UART3_Init(uint32_t baudrate, uint8_t remap);
void UART1_Print(const char *str, ...);
void UART2_Print(const char *str, ...);
void UART3_Print(const char *str, ...);
uint8_t UART1_Scan(uint8_t *data);
uint8_t UART2_Scan(uint8_t *data);
uint8_t UART3_Scan(uint8_t *data);

void UART_FirstInit(void) __attribute__((constructor));

void UART_FirstInit(void)
{
	UART1.Init = UART1_Init;
	UART1.Scan = UART1_Scan;
	UART1.Print = UART1_Print;

	UART2.Init = UART2_Init;
	UART2.Scan = UART2_Scan;
	UART2.Print = UART2_Print;

	UART3.Init = UART3_Init;
	UART3.Scan = UART3_Scan;
	UART3.Print = UART3_Print;
}

void UARTx_Init(USART_TypeDef *UART, uint32_t baudrate, uint8_t remap)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef UART_InitStruct;

	if (UART == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		if (remap)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

			GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
			GPIO_Init(GPIOB, &GPIO_InitStruct);

			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
			GPIO_Init(GPIOB, &GPIO_InitStruct);
		}
		else
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
			GPIO_Init(GPIOA, &GPIO_InitStruct);

			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
		}

		NVIC_SetPriority(USART1_IRQn, 0);
		NVIC_EnableIRQ(USART1_IRQn);
	}

	else if (UART == USART2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		NVIC_SetPriority(USART2_IRQn, 1);
		NVIC_EnableIRQ(USART2_IRQn);
	}

	else if (UART == USART3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		NVIC_SetPriority(USART3_IRQn, 2);
		NVIC_EnableIRQ(USART3_IRQn);
	}

	UART_InitStruct.USART_BaudRate = baudrate; /* toc do truyen: baud_rates */
	UART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	UART_InitStruct.USART_Parity = USART_Parity_No;
	UART_InitStruct.USART_StopBits = USART_StopBits_1;
	UART_InitStruct.USART_WordLength = USART_WordLength_8b;
	UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(UART, &UART_InitStruct);

	USART_Cmd(UART, ENABLE); /* cho phep USART hoat dong */

	USART_ITConfig(UART, USART_IT_RXNE, ENABLE); /* ngat nhan USART					*/
}

/**
 *************************************************************************
 * @brief 	UARTx Initialization
 * @param  baudrate
 * @param  remap: remap or not
 * @retval None
 *************************************************************************
 */

void UART1_Init(uint32_t baudrate, uint8_t remap)
{
	UARTx_Init(USART1, baudrate, remap);
}

void UART2_Init(uint32_t baudrate, uint8_t remap)
{
	UARTx_Init(USART2, baudrate, remap);
}

void UART3_Init(uint32_t baudrate, uint8_t remap)
{
	UARTx_Init(USART3, baudrate, remap);
}

void UART1_Print(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	UARTx_SendData(USART1, str, args);
	va_end(args);
}

void UART2_Print(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	UARTx_SendData(USART2, str, args);
	va_end(args);
}

void UART3_Print(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	UARTx_SendData(USART3, str, args);
	va_end(args);
}

uint8_t UART1_Scan(uint8_t *data)
{
	return UARTx_ReceiveData(U1, data);
}

uint8_t UART2_Scan(uint8_t *data)
{
	return UARTx_ReceiveData(U2, data);
}

uint8_t UART3_Scan(uint8_t *data)
{
	return UARTx_ReceiveData(U3, data);
}

static __inline void UART_SendChar(USART_TypeDef *UART, const char str)
{
	while (USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(UART, str);
}

static __inline void UART_SendStr(USART_TypeDef *UART, const char *str)
{
	do
	{
		UART_SendChar(UART, *str);

	} while (*(++str));
}

void UARTx_SendData(USART_TypeDef *UART, const char *str, va_list args)
{
	do
	{
		if (*str == '%')
		{
			switch (*(++str))
			{
			case 's':
			{
				char *temp_str = va_arg(args, char *);
				UART_SendStr(UART, temp_str);
				continue;
			}

			case 'd':
			{
				int temp_num = va_arg(args, int);
				sprintf(number, "%d", temp_num);
				UART_SendStr(UART, number);
				continue;
			}

			case 'h':
			{
				int temp_num = va_arg(args, int);
				sprintf(number, "%02X", temp_num);
				UART_SendStr(UART, number);
				continue;
			}

			case 'f':
			{
				double temp_num = va_arg(args, double);
				sprintf(number, "%f", temp_num);
				UART_SendStr(UART, number);
				continue;
			}
			}
		}

		UART_SendChar(UART, *str);
	} while (*(++str));
}

uint8_t UARTx_ReceiveData(UARTx_e Ux, uint8_t *data)
{
	uint8_t new_data = 0;
	if (UARTx_rx_flag[Ux])
	{
		uint8_t index = 0;
		for (volatile int i = 0; i < 0xfffff; i++)
		{
			if (UARTx_rx_flag[Ux] == 2)
			{
				break; // delay 1 l?c
			}
		}

		UARTx_dataReceived[Ux][UARTx_idx[Ux]++] = '\0';
		UARTx_idx[Ux] = 0;
		UARTx_rx_flag[Ux] = 0;
		new_data = 1;
		do
		{
			data[index] = UARTx_dataReceived[Ux][index];
		} while (UARTx_dataReceived[Ux][index++]);
	}
	return new_data;
}

/**
 *************************************************************************
 * @brief 	USARTxIRQn
 * @param 	None
 * @retval None
 *************************************************************************
 */

void USART1_IRQHandler(void)
{
	UARTx_rx_flag[U1] = 1;
	UARTx_dataReceived[U1][(UARTx_idx[U1]++) & 0x7f] = (uint8_t)USART1->DR;
	if (UARTx_dataReceived[U1][UARTx_idx[U1] - 1] == '\n')
		UARTx_rx_flag[U1] = 2;
}
void USART2_IRQHandler(void)
{
	UARTx_rx_flag[U2] = 1;
	UARTx_dataReceived[U2][(UARTx_idx[U2]++) & 0x7f] = (uint8_t)USART2->DR;
	if (UARTx_dataReceived[U2][UARTx_idx[U2] - 1] == '\n')
		UARTx_rx_flag[U2] = 2;
}
void USART3_IRQHandler(void)
{
	UARTx_rx_flag[U3] = 1;
	UARTx_dataReceived[U3][(UARTx_idx[U3]++) & 0x7f] = (uint8_t)USART3->DR;
	if (UARTx_dataReceived[U3][UARTx_idx[U3] - 1] == '\n')
		UARTx_rx_flag[U3] = 2;
}
