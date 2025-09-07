#include "uart.h"

static uint8_t data[128];

int main(void)
{
	UART1.Init(9600, NO_REMAP);

	while (1)
	{
		if (UART1.Scan(data))
		{
			UART1.Print("\nChuoi nhan duoc: %s\n", data);
		}
	}
}
