#include "main.h"

int main(void)
{
	Delay_Init();
	
	USART_InitHandle(&UART1, USART1, 9600);
	UART1.sendString(&UART1, "Haizz...!\n");
	printf("Hello World\n");
	while (1)
	{
		if (UART1.available(&UART1))
		{
			char c = UART1.getChar(&UART1);
			UART1.sendByte(&UART1, c);
		}
	}
}
