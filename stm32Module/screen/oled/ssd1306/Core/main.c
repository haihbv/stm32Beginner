#include "main.h"

int main(void)
{
	i2c_Init(I2C1);
	ssd1306_Init(I2C1);
	
	ssd1306_FillBuffer(0x00);
	ssd1306_Refresh();
	
	ssd1306_SetAutoNewline(ENABLE);
	
	/* ve mot pixel trang */
	ssd1306_SetPixel(10, 10);  
  ssd1306_SetPixel(20, 5);
  ssd1306_Refresh();
	
	/* ve mot duong cheo */
	ssd1306_DrawLine(0, 0, 127, 63, COLOR_WHITE);
  ssd1306_Refresh();
	
	/* Hinh Chu Nhat */
	ssd1306_DrawLine(30, 20, 90, 20, COLOR_WHITE); // top
  ssd1306_DrawLine(30, 40, 90, 40, COLOR_WHITE); // bottom
  ssd1306_DrawLine(30, 20, 30, 40, COLOR_WHITE); // left
  ssd1306_DrawLine(90, 20, 90, 40, COLOR_WHITE); // right
  ssd1306_Refresh();
	
	ssd1306_SetCursor(0, 50);
  ssd1306_PutString("haihbv", Font_11x18, COLOR_WHITE);
  ssd1306_Refresh();
	
	while (1)
	{
		
	}
}
