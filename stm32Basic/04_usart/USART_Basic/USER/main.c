#include "usart.h"
#include "delay.h"

int main(void)
{
  SystemInit();
  Delay_Init();
  USARTx_Init(USART2, 9600);

  while (1)
	{
		char c = USART_ReceiveChar(USART2);
		USART_SendChar(USART2, c);
	}
}
