#include "lcd_16x2.h"
#include <string.h>

int main()
{
	SysTick_Init();
	LCD_Init();
	
	LCD_CreateChar(0, degree);
	LCD_CreateChar(1, heart);
	
  LCD_Goto(0, 0);
	LCD_Printf("Temp: %.1f", 38.291);
	LCD_PutCustom(0);
	delay_us(40);
	LCD_PutChar('C');
	
	LCD_Goto(0, 1);
	LCD_Printf("By haihbv ");
	LCD_PutCustom(1);
	delay_us(40);
}
