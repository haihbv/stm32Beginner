#include "lcd_printf.h"

static void LCD_I2C_PrintChar(char c)
{
	LCD_I2C_SendData(c);
}

static void LCD_I2C_PrintString(const char *str)
{
	while (*str)
	{
		LCD_I2C_PrintChar(*str++);
	}
}

static void LCD_I2C_PrintInt(int num)
{
	char buffer[12];
	int i = 0;
	int j;
	int neg = 0;
	
	if (num == 0)
	{
		LCD_I2C_PrintChar('0');
		return;
	}
	
	if (num < 0)
	{
		neg = 1;
		num = -num;
	}
	
	while (num > 0)
	{
		buffer[i++] = (num % 10) + '0';
		num /= 10;
	}
	
	if (neg)
	{
		buffer[i++] = '-';
	}
	
	for (j = i - 1; j >= 0; j--)
	{
		LCD_I2C_PrintChar(buffer[j]);
	}
}

static void LCD_I2C_PrintHex(uint32_t num)
{
	char hex[9];
	int i;
	for (i = 0; i < 8; i++)
	{
		uint8_t nibble = (num >> (28 - i * 4)) & 0xF;
		if (nibble < 10)
		{
			hex[i] = '0' + nibble;
		}
		else 
		{
			hex[i] = 'A' + (nibble - 10);
		}
	}
	hex[8] = 0;
	LCD_I2C_PrintString(hex);
}
void LCD_I2C_Printf(const char *format, ...)
{
	const char *ptr_char = format;
	
	va_list args;
	va_start(args, format);
	
	while (*ptr_char)
	{
		if (*ptr_char == '%')
		{
			ptr_char++;
			switch (*ptr_char)
			{
				case 'c':
				{
					LCD_I2C_PrintChar((char)va_arg(args, int));
					break;
				}
				case 's':
				{
					LCD_I2C_PrintString(va_arg(args, char*));
					break;
				}
				case 'd':
				{
					LCD_I2C_PrintInt(va_arg(args, int));
					break;
				}
				case 'x':
				{
					LCD_I2C_PrintHex(va_arg(args, uint32_t));
					break;
				}
				case '%':
				{
					LCD_I2C_PrintChar('%');
					break;
				}
				default:
				{
					LCD_I2C_PrintChar(*ptr_char);
					break;
				}
			}
		}
		else
		{
			LCD_I2C_PrintChar(*ptr_char);
		}
		ptr_char++;
	}
	va_end(args);
}
