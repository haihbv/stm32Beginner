#include "main.h"

uint16_t val;

int main(void)
{
	SystemInit();
	Delay_Init();
	
	hADC1.Init(ADC_Channel_0);
	
	while (1)
	{
		val = hADC1.Read(ADC_Channel_0);
		DelayMs(1000);
	}
}
