#include "main.h"
#include "test_ssd1306.h"

int main(void)
{
	ssd1306_Init(I2C2);
	
	test_DisplayOnOff();
	
	test_FillBuffer();
	
	test_DrawPixel();
	
	test_PutCharAndString();
	
	test_DrawLine();
	
	test_FillRectangle();
	
	test_DrawBitMap();
	
	// test_Contrast();
	while (1)
	{
	}
}