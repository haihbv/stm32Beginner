#include "lcd_16x2.h"

#define LCD_RS      GPIO_Pin_13
#define LCD_RW      GPIO_Pin_14
#define LCD_EN      GPIO_Pin_15

#define LCD_D4      GPIO_Pin_8
#define LCD_D5      GPIO_Pin_9
#define LCD_D6      GPIO_Pin_10
#define LCD_D7      GPIO_Pin_11

uint8_t heart[8] = {
	0x0A, // 01010
  0x1F, // 11111
  0x1F, // 11111
  0x1F, // 11111
  0x0E, // 01110
  0x04, // 00100
  0x00, // 00000
  0x00  // 00000
};

uint8_t degree[8] = {
  0x06, // 00000110
  0x09, // 00001001
  0x09, // 00001001
  0x06, // 00000110
  0x00, // 00000000
  0x00, // 00000000
  0x00, // 00000000
  0x00  // 00000000
};

void LCD_CreateChar(uint8_t location, uint8_t charmap[]) 
{
  location &= 0x07; 
  LCD_SendCommand((uint8_t)(0x40 | (location << 3))); 
  for (int i = 0; i < 8; i++) 
	{
    LCD_SendData(charmap[i]);
  }
}
void LCD_PutCustom(uint8_t location) 
{
  LCD_SendData(location);
}

void GPIO_LCD_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Data pins D4-D7 (PA8 - PA11)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Control pins RS, RW, EN (PC13 - PC15)
	GPIO_InitStruct.GPIO_Pin = LCD_RS | LCD_RW | LCD_EN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// Reset pin
	GPIO_ResetBits(GPIOA, LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
	GPIO_ResetBits(GPIOC, LCD_RS | LCD_RW | LCD_EN);
}

// enable pulse
void LCD_Enable(void)
{
	GPIO_SetBits(GPIOC, LCD_EN);
	delay_us(1);
	GPIO_ResetBits(GPIOC, LCD_EN);
	delay_us(50);
}

// send 4 bit of data
void LCD_Send4Bit(uint8_t data)
{
	GPIO_WriteBit(GPIOA, LCD_D4, data & 0x01);
	GPIO_WriteBit(GPIOA, LCD_D5, (data >> 1) & 0x01);
	GPIO_WriteBit(GPIOA, LCD_D6, (data >> 2) & 0x01);
	GPIO_WriteBit(GPIOA, LCD_D7, (data >> 3) & 0x01);
}

// send command (RS = 0)
void LCD_SendCommand(uint8_t command)
{
	GPIO_ResetBits(GPIOC, LCD_RS); // command
	GPIO_ResetBits(GPIOC, LCD_RW); // write
	
	LCD_Send4Bit(command >> 4);
	LCD_Enable();
	LCD_Send4Bit(command & 0x0F);
	LCD_Enable();
	
	if (command == 0x01 || command == 0x02)
	{
		delay_ms(2);
	}
}

// send data (RS = 1)
void LCD_SendData(uint8_t data)
{
	GPIO_SetBits(GPIOC, LCD_RS); // data
	GPIO_ResetBits(GPIOC, LCD_RW); // write
	
	LCD_Send4Bit(data >> 4);
	LCD_Enable();
	LCD_Send4Bit(data & 0x0F);
	LCD_Enable();
}

// clear screen
void LCD_Clear(void)
{
	LCD_SendCommand(0x01);
}
void LCD_Init(void)
{
	GPIO_LCD_Config();
	delay_ms(20); // wait LCD to boot
	
	LCD_Send4Bit(0x03);
	LCD_Enable();
	delay_ms(5);
	LCD_Send4Bit(0x03);
	LCD_Enable();
	delay_us(150);
	LCD_Send4Bit(0x03);
	LCD_Enable();
	delay_us(100);
	LCD_Send4Bit(0x02);
	LCD_Enable();
	
	LCD_SendCommand(0x28);
	LCD_SendCommand(0x0C);
	LCD_SendCommand(0x06);
	LCD_Clear();
}
void LCD_Goto(uint8_t x, uint8_t y)
{
	uint8_t address;
	if (y == 0)
	{
		address = (0x80 + x);
	}
	else if (y == 1)
	{
		address = (0xC0 + x);
	}
	else 
	{
		return;
	}
	
	LCD_SendCommand(address);
}
void LCD_PutChar(uint8_t data)
{
  LCD_SendData(data);
}
void LCD_PutStr(char *str)
{
  while (*str) {
		LCD_PutChar(*str++);
  }
}
void LCD_Printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void LCD_Printf(const char *fmt, ...)
{
	char buffer[32];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	LCD_PutStr(buffer);
}


