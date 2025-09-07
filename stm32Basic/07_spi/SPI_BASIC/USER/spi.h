#ifndef SPI_H
#define SPI_H

#include "delay.h"

#define CS_PIN          GPIO_Pin_4
#define CS_HIGH         GPIOA->BSRR = CS_PIN
#define CS_LOW          GPIOA->BRR = CS_PIN

void SPI_Config(SPI_TypeDef *SPIx);
uint8_t SPI_Transmit_Receive_Data(SPI_TypeDef *SPIx, uint8_t data);
void SPI_CS_Enable(SPI_TypeDef *SPIx);
void SPI_CS_Disable(SPI_TypeDef *SPIx);
void SPI_SendCommand(SPI_TypeDef *SPIx, uint8_t data);
#endif // SPI_H
