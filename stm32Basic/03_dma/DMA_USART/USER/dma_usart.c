#include "dma_usart.h"

uint8_t rx_buffer[RX_BUFFER_SIZE];

void DMA_USART1_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;

	// bat clock cho GPIOA, USART1, DMA1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// PA9 - TX (AF_PP)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// PA10 - RX (Input Floating)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Cau Hinh USART1
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);

	// cho phep USART1 dung DMA truyen/ nhan
	USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
	USART_Cmd(USART1, ENABLE);

	// Cau Hinh DMA cho RX (Channel 5)
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)rx_buffer;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // USART -> RAM
	DMA_InitStruct.DMA_BufferSize = RX_BUFFER_SIZE;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStruct);

	DMA_Cmd(DMA1_Channel5, ENABLE);
}
void DMA_USART1_Send(uint8_t *data, uint16_t len)
{
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_DeInit(DMA1_Channel4);

	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)data;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST; // RAM -> USART
	DMA_InitStruct.DMA_BufferSize = len;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; // gui 1 lan
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);

	DMA_Cmd(DMA1_Channel4, ENABLE);

	while (!DMA_GetFlagStatus(DMA1_FLAG_TC4))
		;
	DMA_ClearFlag(DMA1_FLAG_TC4);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
}

void delay_ms(uint32_t ms)
{
	SysTick->LOAD = 72000 - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = 5;
	for (volatile uint32_t i = 0; i < ms; i++)
	{
		while (!(SysTick->CTRL & (1 << 16)))
			;
	}
	SysTick->CTRL = 0;
}
