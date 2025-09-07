#include "delay.h"
#include "uart.h"
#include "hc_sr04.h"

int main()
{
	SystemInit();
	Delay_Init();
	UART1.Init(9600, NO_REMAP);
	HCSR04_Init();

	while (1)
	{
		uint32_t distance = HCSR04_GetDistance();
		UART1.Print("Distance: %d cm\r\n", distance);
		Delay_Ms(1000);
	}
}
