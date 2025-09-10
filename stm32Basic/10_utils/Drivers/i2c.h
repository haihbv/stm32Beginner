#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"

#define I2C_ClockSpeed_100kHz 	(100000)
#define I2C_ClockSpeed_400kHz		(400000)

void I2Cx_Init(I2C_TypeDef *I2Cx, uint32_t I2C_ClockSpeed);
void I2C_Start(I2C_TypeDef *I2Cx);
void I2C_Stop(I2C_TypeDef *I2Cx);
void I2C_WriteAddress(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Is_Read);
uint8_t I2C_WriteData(I2C_TypeDef *I2Cx, uint8_t Data);
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx, uint8_t ACK);

#ifdef __cplusplus
}
#endif

#endif // __I2C_H
