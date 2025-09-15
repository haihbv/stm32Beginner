#include "main.h"

int main(void)
{
  ssd1306_Init(I2C2);

  ssd1306_Clear();
	ssd1306_Refresh();
	
  ssd1306_DrawImage(0, 0, 128, 64, capoo_bitmap);
	ssd1306_Refresh();
	
	while (1)
	{
		
	}
}
