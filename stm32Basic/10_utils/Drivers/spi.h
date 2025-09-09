#ifndef __SPI_H
#define __SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"

void SPI_Start(SPI_TypeDef *SPIx);
void SPI_Stop(SPI_TypeDef *SPIx);
void SPI_CS_Low(GPIO_TypeDef *GPIOx, uint16_t GPIO_PinCS);
void SPI_CS_High(GPIO_TypeDef *GPIOx, uint16_t GPIO_PinCS);
void SPIx_Init(SPI_TypeDef *SPIx, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_DataSize);
uint8_t SPI_TR_Data(SPI_TypeDef *SPIx, uint8_t Data);

#ifdef __cplusplus
}
#endif

#endif // __SPI_H
