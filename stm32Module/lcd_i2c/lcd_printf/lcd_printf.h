#ifndef __LCD_PRINTF_H
#define __LCD_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lcd_i2c.h"
#include <stdint.h>
#include <stdarg.h>

void LCD_I2C_Printf(const char *format, ...);

#ifdef __cpluplus
}
#endif

#endif // __LCD_PRINTF_H
