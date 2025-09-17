#include "test_ssd1306.h"
#include "image.h"

/**
 * @brief   On/Off man hinh
 */
void test_DisplayOnOff(void)
{
    ssd1306_DisplayOn();
    DelayMs(500);
    ssd1306_DisplayOff();
    DelayMs(500);
}

/**
 * @brief   Ve pixel tai cac goc
 */
void test_DrawPixel(void)
{
    ssd1306_Clear();
    ssd1306_DrawPixel(0, 0, COLOR_WHITE);
    ssd1306_DrawPixel(MAX_COL - 1, 0, COLOR_WHITE);
    ssd1306_DrawPixel(0, MAX_ROW - 1, COLOR_WHITE);
    ssd1306_DrawPixel(MAX_COL - 1, MAX_ROW - 1, COLOR_WHITE);
    ssd1306_Refresh();
}

/**
 * @brief   In ky tu va chuoi
 */
void test_PutCharAndString(void)
{
    ssd1306_Clear();
    ssd1306_SetCursor(0, 0);
    ssd1306_PutChar('A', Font_6x8, COLOR_WHITE);
    ssd1306_SetCursor(0, 40);
    ssd1306_PutString("Daisuki Dayo", Font_6x8, COLOR_WHITE);
    ssd1306_Refresh();
}

/**
 * @brief   Ve duong thang
 */
void test_DrawLine(void)
{
    ssd1306_Clear();
    ssd1306_DrawLine(0, 0, MAX_COL - 1, MAX_ROW - 1, COLOR_WHITE);
    ssd1306_DrawLine(MAX_COL - 1, 0, 0, MAX_ROW - 1, COLOR_WHITE);
    ssd1306_Refresh();
}

/**
 * @brief   Ve hinh hoc
 */
void test_FillRectangle(void)
{
    ssd1306_Clear();
    ssd1306_FillRectangle(0, 0, 30, 20, COLOR_WHITE);    
    ssd1306_FillRectangle(97, 0, 127, 20, COLOR_WHITE);  
    ssd1306_FillRectangle(0, 44, 30, 63, COLOR_WHITE);   
    ssd1306_FillRectangle(97, 44, 127, 63, COLOR_WHITE); 
    ssd1306_Refresh();
}

/**
 * @brief   Ve hinh bat ky
 */
void test_DrawBitMap(void)
{
    ssd1306_Clear();
    ssd1306_DrawBitMap(0, 0, 69, 64, saitama_69x64);
    ssd1306_Refresh();
}

/**
 * @brief   Contrast
 */
void test_Contrast(void)
{
    for (uint8_t c = 0; c < 0xFF; c += 20)
    {
        ssd1306_SetContrast(c);
        for (volatile uint32_t i = 0; i < 300000; i++)
            ;
    }
}
void test_FillBuffer(void)
{
    ssd1306_FillBuffer(0xFF); // Full white
    ssd1306_Refresh();
    for (volatile uint32_t i = 0; i < 500000; i++)
        ;
    ssd1306_FillBuffer(0x00); // Full black
    ssd1306_Refresh();
}

void test_All(void)
{
    test_DisplayOnOff();
    test_FillBuffer();
    test_DrawPixel();
    test_PutCharAndString();
    test_DrawLine();
    test_FillRectangle();
    test_DrawBitMap();
    test_Contrast();
}
