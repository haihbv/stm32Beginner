#include "main.h"

int main(void)
{
	SystemInit();
	Delay_Init();
	
	USART_InitHandle(&UART1, USART1, 115200);
	hADC1.Init(ADC_Channel_4);
	hADC2.Init(ADC_Channel_3);
	
	uint16_t adc1Value, adc2Value;
	while (1)
	{
		adc1Value = hADC1.Read(ADC_Channel_4); adc2Value = hADC2.Read(ADC_Channel_3);
		printf("ADC Value: %d %d\n", adc1Value, adc2Value);
		DelayMs(1000);
	}
}
