#include "main.h"

int main(void)
{
	SystemInit();
	Delay_Init();
	
	hUSART1.Init(115200);
	printf("USART1 Echo Test\r\n");
	while (1)
	{
		if (hUSART1.Available())
		{
			char c = hUSART1.GetChar();
			USART_SendChar(USART1, c);
			
			if (c == '\r')
			{
				USART_SendChar(USART1, '\n');
			}
		}
	}
}
