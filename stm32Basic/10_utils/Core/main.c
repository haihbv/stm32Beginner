#include "main.h"

int main(void)
{
	SystemInit();
	Delay_Init();
	
	SPIx_Init(SPI1, SPI_BaudRatePrescaler_128, SPI_DataSize_8b);
	
	while (1)
	{
		SPI_CS_Low(GPIOC, GPIO_Pin_15);
		SPI_TR_Data(SPI1, 0xA5);
		SPI_CS_High(GPIOC, GPIO_Pin_15);
		DelayMs(1000);
	}
}
