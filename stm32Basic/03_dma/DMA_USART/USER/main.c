#include "dma_usart.h"
#include <string.h>

int main()
{
	DMA_USART1_Init(9600);

	uint8_t *msg = (uint8_t *)"DMA USART1 Demo\r\n";
	while (1)
	{
		DMA_USART1_Send(msg, (uint16_t)strlen((char *)msg));
		delay_ms(1000);
	}
}
