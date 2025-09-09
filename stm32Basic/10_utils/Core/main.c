#include "main.h"

int main(void)
{
	SystemInit();
	Delay_Init();
	
	hSPI1.Init(SPI_BaudRatePrescaler_128, SPI_DataSize_8b);
	
	while (1)
	{
		hSPI1.CS_Low();
		hSPI1.Transfer(0xA5);
		hSPI1.CS_High();
		DelayMs(1000);
	}
}
