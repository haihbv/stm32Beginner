#include "spi.h"

#define TARGET_ADDRESS  	0xA3
int main()
{
	SystemInit();
	SysTick_Init();
	SPI_Config(SPI1);
	while (1)
	{
		SPI_Transmit_Receive_Data(SPI1, TARGET_ADDRESS);
		delay_ms(1000);
	}
}



