#include "main.h"

int main(void)
{
	ssd1306_Init(I2C2);

	ssd1306_Clear();
	ssd1306_Refresh();

	while (1)
	{
		ssd1306_DrawBitMap(0, 0, 19, 39, walkmen_0_19x39);
		ssd1306_Refresh();

		ssd1306_DrawBitMap(0, 0, 19, 39, walkmen_1_19x39);
		ssd1306_Refresh();

		ssd1306_DrawBitMap(0, 0, 19, 39, walkmen_2_19x39);
		ssd1306_Refresh();

		ssd1306_DrawBitMap(0, 0, 19, 39, walkmen_3_19x39);
		ssd1306_Refresh();
	}
}
