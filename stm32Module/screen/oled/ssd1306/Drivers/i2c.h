#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"

void i2c_Init(I2C_TypeDef *I2Cx);
void i2c_Address(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t is_read);
void i2c_Data(I2C_TypeDef *I2Cx, uint8_t data);
void i2c_Start(I2C_TypeDef *I2Cx);
void i2c_Stop(I2C_TypeDef *I2Cx);
void i2c_Master_Transmit(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H */
