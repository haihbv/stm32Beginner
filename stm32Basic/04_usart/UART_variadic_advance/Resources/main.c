#include "uart.h"

uint8_t data[100];

int main(void)
{
	UART1.Init(115200, NO_REMAP);

	UART1.Print("Char: %c, String: %s, Dec: %d, Hex: %h, Float: %f, Percent: %%\n", 'A', "hello", 123, 0xAB, 3.14);

	while (1)
	{
		if (UART1.Scan(data))
		{
			UART1.Print("\nChuoi nhan duoc: %s\n", data);
		}
	}
}
