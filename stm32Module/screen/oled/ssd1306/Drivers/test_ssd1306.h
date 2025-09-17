#ifndef TEST_SSD1306_H
#define TEST_SSD1306_H

#include "ssd1306.h"
#include "fonts.h"
#include "delay.h"

/**********************************************************
 * TEST_API ssd1306
 **********************************************************/
void test_DisplayOnOff(void);
void test_DrawPixel(void);
void test_PutCharAndString(void);
void test_DrawLine(void);
void test_FillRectangle(void);
void test_DrawBitMap(void);
void test_Contrast(void);
void test_FillBuffer(void);
void test_All(void);

#endif /* TEST_SSD1306_H */
