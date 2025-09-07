#include "ir_sensor.h"
#include "uart.h"
#include "delay.h"

static uint16_t ir_val = 0;

int main()
{
	SystemInit();
	Delay_Init();
	UART1.Init(115200, NO_REMAP);
	IR_ADC_Init();
	
	while (1)
	{
		ir_val = IR_Read();
		UART1.Print("IR ADC value: %d\r\n", ir_val);
		if (ir_val < 300)
		{
			UART1.Print("Line White\r\n");
		}
		else 
		{
			UART1.Print("Line Black\r\n");
		}
		Delay_Ms(1000);
	}
}
