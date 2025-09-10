#include "main.h"

static uint8_t reg = 0x00;
uint8_t val;

int main(void)
{
	SystemInit();
	Delay_Init();
	
	hI2C1.Init();

	while (1)
	{
		hI2C1.Master_Transmit(0x68, &reg, 1);
		hI2C1.Master_Receive(0x68, &val, 1);
		DelayMs(1000);
	}
}
