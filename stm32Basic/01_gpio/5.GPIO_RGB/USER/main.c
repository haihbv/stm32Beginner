#include "rgb.h"

int main(void)
{
	SysTick_Config(SystemCoreClock / 1000);
	GPIO_Config();
	while (1)
	{
		LED_RGB_SetCommand();
	}
}
