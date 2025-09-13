#include "stm32f10x.h"
#include "ssd1306.h"
#include "delay.h"

int main(void)
{
	Delay_Init();
	ssd1306_Init(I2C1);
	
	DelayMs(1000);
	ssd1306_Blank(I2C1);
	ssd1306_Goto(I2C1, 3, 40, "Lieu Ngoc");
	while (1)
	{
	}
}
