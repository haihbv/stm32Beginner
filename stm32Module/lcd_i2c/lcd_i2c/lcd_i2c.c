#include "lcd_i2c.h"
#include <stdarg.h>

#define LCD_I2C_ADDR		(0x27 << 1)
#define LCD_BACKLIGHT		0x08
#define LCD_ENABLE			0x04
#define LCD_RW					0x02
#define LCD_RS					0x01

static I2C_TypeDef* _i2c_port;

static void LCD_I2C_Write(uint8_t data)
{
	while (I2C_GetFlagStatus(_i2c_port, I2C_FLAG_BUSY)); // wait bus I2C free 
	I2C_GenerateSTART(_i2c_port, ENABLE);
	while (!I2C_CheckEvent(_i2c_port, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(_i2c_port, LCD_I2C_ADDR, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(_i2c_port, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(_i2c_port, data);
	while (!I2C_CheckEvent(_i2c_port, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(_i2c_port, ENABLE);
}

static void LCD_I2C_SendHalf(uint8_t data)
{
	LCD_I2C_Write(data | LCD_BACKLIGHT | LCD_ENABLE);
	Delay_us(50);
	LCD_I2C_Write((data & ~LCD_ENABLE) | LCD_BACKLIGHT);
	Delay_us(50);
}

static void LCD_I2C_Send(uint8_t value, uint8_t mode)
{
	uint8_t high_nib = value & 0xF0;
	uint8_t low_nib = (value << 4) & 0xF0;
	
	LCD_I2C_SendHalf(high_nib | mode);
	LCD_I2C_SendHalf(low_nib | mode);
}

void LCD_I2C_SendCmd(uint8_t cmd)
{
	LCD_I2C_Send(cmd, 0);
	if (cmd == 0x01 || cmd == 0x02)
	{
		Delay_ms(2);
	}
	else
	{
		Delay_us(50);
	}
}

void LCD_I2C_SendData(uint8_t data)
{
	LCD_I2C_Send(data, LCD_RS);
}

void LCD_I2C_Clear(void)
{
	LCD_I2C_SendCmd(0x01);
	Delay_ms(2);
}

void LCD_I2C_Goto(uint8_t row, uint8_t col)
{
	uint8_t _addr = (row == 0) ? 0x80 : 0xC0;
	LCD_I2C_SendCmd(_addr + col);
}

void LCD_I2C_SendString(const char *str)
{
	while (*str)
	{
		LCD_I2C_SendData(*str++);
	}
}

void LCD_I2C_GPIO_Init(I2C_TypeDef *I2Cx)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	if (I2Cx == I2C1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if (I2Cx == I2C2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else
	{
		return;
	}
	
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2Cx, &I2C_InitStruct);
	I2C_Cmd(I2Cx, ENABLE);
}

void LCD_I2C_Init(I2C_TypeDef *I2Cx)
{
	_i2c_port = I2Cx;
	LCD_I2C_GPIO_Init(I2Cx);
	
	Delay_ms(100);
	
	LCD_I2C_SendHalf(0x30);
	Delay_ms(5);
	LCD_I2C_SendHalf(0x30);
	Delay_us(150);
	LCD_I2C_SendHalf(0x30);
	Delay_us(150);
	
	LCD_I2C_SendHalf(0x20);
	Delay_us(150);
	
	LCD_I2C_SendCmd(0x28);
	LCD_I2C_SendCmd(0x0C);
	LCD_I2C_SendCmd(0x06);
	LCD_I2C_Clear();
}

// button + LCD
void LCD_I2C_NextLine(void)
{
	static uint8_t current_now = 0;
	current_now = (current_now + 1) % 2;
	LCD_I2C_Goto(current_now, 0);
}

