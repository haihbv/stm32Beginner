#include "spi.h"

void SPI_Start(SPI_TypeDef *SPIx)
{
	SPI_Cmd(SPIx, ENABLE);
}
void SPI_Stop(SPI_TypeDef *SPIx)
{
	SPI_Cmd(SPIx, DISABLE);
}
void SPIx_Init(SPI_TypeDef *SPIx, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_DataSize)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	SPI_InitTypeDef		SPI_InitStruct;
	if (SPIx == SPI1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOC, ENABLE);
		
		// MOSI - SCK
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		// MISO
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		// CS
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStruct);
		GPIO_SetBits(GPIOC, GPIO_Pin_15);
	}
	else if (SPIx == SPI2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		
		// MOSI - SCK
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		// MISO
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		// CS
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}
	else 
	{
		return;
	}
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPIx, &SPI_InitStruct);
	SPI_Start(SPIx);
}

void SPI_CS_Low(GPIO_TypeDef *GPIOx, uint16_t GPIO_PinCS)
{
	GPIOx->BRR = GPIO_PinCS;
}
void SPI_CS_High(GPIO_TypeDef *GPIOx, uint16_t GPIO_PinCS)
{
	GPIOx->BSRR = GPIO_PinCS;
}

uint8_t SPI_TR_Data(SPI_TypeDef *SPIx, uint8_t Data)
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIx, Data);
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	return (uint8_t)SPI_I2S_ReceiveData(SPIx);
}

