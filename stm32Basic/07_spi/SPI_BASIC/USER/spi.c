#include "spi.h"

void SPI_Config(SPI_TypeDef *SPIx)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	if (SPIx == SPI1)
	{
		// clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

		// gpio
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = CS_PIN;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else if (SPIx == SPI2)
	{
		// clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

		// gpio
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}

	// spi config
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPIx, &SPI_InitStruct);
	SPI_Cmd(SPIx, ENABLE);
}
void SPI_CS_Enable(SPI_TypeDef *SPIx)
{
	if (SPIx == SPI1)
	{
		GPIO_ResetBits(GPIOA, CS_PIN);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	}
}

void SPI_CS_Disable(SPI_TypeDef *SPIx)
{
	if (SPIx == SPI1)
	{
		GPIO_SetBits(GPIOA, CS_PIN);
	}
	else
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}
}
uint8_t SPI_Transmit_Receive_Data(SPI_TypeDef *SPIx, uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
		;
	SPIx->DR = data;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
		;
	return (uint8_t)SPIx->DR;
}

void SPI_SendCommand(SPI_TypeDef *SPIx, uint8_t data)
{
	SPI_CS_Disable(SPIx);
	SPI_Transmit_Receive_Data(SPIx, data);
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)
		;
	delay_ms(1);
	SPI_CS_Enable(SPIx);
}
