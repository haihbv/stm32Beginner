#include "uart.h"

static uint8_t data[128];

int main(void)
{
	uart1_init(115200);

	uart1_printf("UART Demo - STM32F103\r\n");
	uart1_printf("Char: %c, String: %s, Dec: %d, Hex: %x, HEX: %X, Float: %f, Percent: %%\n",
             'A', "hello", 123, 0xAB, 0xAB, 3.14159);
	uart1_printf("Float 3 digits: %.3f\n", 3.14159);
	while (1)
	{
		if (uart1_scanf(data))
		{
			uart1_printf("\nChuoi nhan duoc la: %s", data);
		}
	}
}
