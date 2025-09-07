#include "stm32f10x.h"                  // Device header
#include "lcd_i2c.h"
#include "lcd_printf.h"
#include "button.h"

int main(void)
{
	SystemInit();
	Delay_Init();
	LCD_I2C_Init(I2C1);
	LCD_I2C_Clear();
	
	LCD_I2C_Goto(0, 0);
	// LCD_I2C_SendString("Hello, STM32!");
	LCD_I2C_Printf("Temp: %d", 25);
	LCD_I2C_SendData(0xDF);
	LCD_I2C_Printf("C");
	
	LCD_I2C_Goto(1, 0);
	// LCD_I2C_SendString("I2C LCD Ready");
	LCD_I2C_Printf("Hum: %d %%", 60);
	
	while (1)
	{
		
	}
}
