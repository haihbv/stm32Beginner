#include "main.h"

int main(void)
{
	ssd1306_Init(I2C2);

	ssd1306_Clear();
	ssd1306_Refresh();

	ssd1306_DrawBitMap(0, 0, 84, 48, weew_stack_84x48);
	ssd1306_Refresh();

	while (1)
	{
	}
}
