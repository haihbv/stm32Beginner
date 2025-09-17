#include "main.h"
#include "test_ssd1306.h"

int main(void)
{
	Delay_Init();
	ssd1306_Init(I2C2);
	
	ssd1306_Clear();
	ssd1306_SetCursor(20, 23);
	ssd1306_PutString("Uyen Nhi", Font_11x18, COLOR_WHITE);
	ssd1306_Refresh();
	DelayMs(2000);
	
	ssd1306_Clear();
	ssd1306_DrawBitMap(0, 0, 84, 48, weew_stack_84x48);
	ssd1306_Refresh();
	DelayMs(2000);

	while (1)
	{
		ssd1306_Clear();
		ssd1306_DrawBitMap(0, 0, 19, 39, walkmen_0_19x39);
		ssd1306_Refresh();

		ssd1306_Clear();
		ssd1306_DrawBitMap(0, 0, 19, 39, walkmen_1_19x39);
		ssd1306_Refresh();

		ssd1306_Clear();
		ssd1306_DrawBitMap(0, 0, 19, 39, walkmen_2_19x39);
		ssd1306_Refresh();

		ssd1306_Clear();
		ssd1306_DrawBitMap(0, 0, 19, 39, walkmen_3_19x39);
		ssd1306_Refresh();
	}
}
