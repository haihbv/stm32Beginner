#ifndef __LCD_16x2_H
#define __LCD_16x2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "delay.h"
#include <stdio.h>
#include <stdarg.h>
extern uint8_t heart[8];
extern uint8_t degree[8];

void GPIO_LCD_Config(void);
void LCD_Enable(void);
void LCD_Send4Bit(uint8_t data);
void LCD_SendCommand(uint8_t command);
void LCD_SendData(uint8_t data);
void LCD_Clear(void);
void LCD_Init(void);
void LCD_Goto(uint8_t x, uint8_t y);
void LCD_PutChar(uint8_t data);
void LCD_PutStr(char *str);
void LCD_Printf(const char *fmt, ...);
void LCD_CreateChar(uint8_t location, uint8_t charmap[]) ;
void LCD_PutCustom(uint8_t location); 
#ifdef __cplusplus
}
#endif

#endif // __LCD_16x2_H
