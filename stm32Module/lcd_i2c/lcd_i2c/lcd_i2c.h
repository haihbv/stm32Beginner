#ifndef __LCD_I2C_H
#define __LCD_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include "delay.h"

void LCD_I2C_GPIO_Init(I2C_TypeDef *I2Cx);
void LCD_I2C_Init(I2C_TypeDef *I2Cx);
void LCD_I2C_SendCmd(uint8_t cmd);
void LCD_I2C_SendData(uint8_t data);
void LCD_I2C_Clear(void);
void LCD_I2C_Goto(uint8_t row, uint8_t col);
void LCD_I2C_SendString(const char *str);

// button + LCD
void LCD_I2C_NextLine(void);

#ifdef __cplusplus
}
#endif


#endif //__LCD_I2C_H
