#include "ssd1306.h"
#include "i2c.h"

void ssd1306_CMD_1byte(I2C_TypeDef *I2Cx, uint8_t data)
{
	i2c_Start(I2Cx);
	
	i2c_Address(I2Cx, SSD1306_ADDRESS, 0);
	
	i2c_Data(I2Cx, 0x00);
	i2c_Data(I2Cx, data);
	
	i2c_Stop(I2Cx);
}

void ssd1306_CMD_2byte(I2C_TypeDef *I2Cx, uint8_t *data)
{
	int i;
	i2c_Start(I2Cx);
	i2c_Address(I2Cx, SSD1306_ADDRESS, 0);
	i2c_Data(I2Cx, 0x00);
	for (i = 0; i < 2; i++)
	{
		i2c_Data(I2Cx, data[i]);
	}
	i2c_Stop(I2Cx);
}

void ssd1306_Init(I2C_TypeDef *I2Cx)
{
	i2c_Init(I2Cx);
	
	ssd1306_CMD_1byte(I2Cx, 0xAE); // Display OFF
	
	uint8_t cmd1[] = {0xA8, 0x3F};
	ssd1306_CMD_2byte(I2Cx, cmd1); // Set MUX Ratio
	
	uint8_t cmd2[] = {0xD3, 0x00};
	ssd1306_CMD_2byte(I2Cx, cmd2); // Set Display Offset
	
	ssd1306_CMD_1byte(I2Cx, 0x40); // Set Display Start Line
	ssd1306_CMD_1byte(I2Cx, 0xA1); // Set Segment re-map
	ssd1306_CMD_1byte(I2Cx, 0xC8); // COM scan direction
	
	uint8_t cmd3[] = {0xDA, 0x12};
	ssd1306_CMD_2byte(I2Cx, cmd3); // Set COM Pins hardware config
	
	uint8_t cmd4[] = {0x81, 0x7F};
	ssd1306_CMD_2byte(I2Cx, cmd4); // Set Contrast Control
	
	ssd1306_CMD_1byte(I2Cx, 0xA4); // Disable Entire Display On
	ssd1306_CMD_1byte(I2Cx, 0xA6); // Set Normal Display
	
	uint8_t cmd5[] = {0xD5, 0x80};
	ssd1306_CMD_2byte(I2Cx, cmd5); // Set Osc Frequency
	
	uint8_t cmd6[] = {0xD9, 0xF1};
	ssd1306_CMD_2byte(I2Cx, cmd6); // Pre-charge period
	
	uint8_t cmd7[] = {0xDB, 0x40};
	ssd1306_CMD_2byte(I2Cx, cmd7); // VCOM detect level
	
	uint8_t cmd8[] = {0x8D, 0x14};
	ssd1306_CMD_2byte(I2Cx, cmd8); // Enable charge pump
	
	ssd1306_CMD_1byte(I2Cx, 0xAF); // Display On
	
	uint8_t cmd9[] = {0x20, 0x10};
	ssd1306_CMD_2byte(I2Cx, cmd9);
}

void ssd1306_Data(I2C_TypeDef *I2Cx, uint8_t data)
{
	i2c_Start(I2Cx);
	
	i2c_Address(I2Cx, SSD1306_ADDRESS, 0);
	
	i2c_Data(I2Cx, 0x40);
	i2c_Data(I2Cx, data);
	
	i2c_Stop(I2Cx);
}

void ssd1306_Pos(I2C_TypeDef *I2Cx,uint8_t yPos, uint8_t xPos)
{
	/*
	* yPos = page (0 -> 7), each page high 8 pixel
	* xPos = column (0 -> 127)
	*/
	ssd1306_CMD_1byte(I2Cx, 0xB0 + yPos); // chon page
	
	ssd1306_CMD_1byte(I2Cx, (xPos & 0x0F)); // cot thap
	ssd1306_CMD_1byte(I2Cx, 0x10 | (xPos >> 4)); // cot cao
}

void ssd1306_Blank(I2C_TypeDef *I2Cx)
{
	int i, j;
	ssd1306_Pos(I2Cx, 0, 0);
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 128; j++)
		{
			ssd1306_Data(I2Cx, 0x00);
		}
	}
	ssd1306_Pos(I2Cx, 0, 0);
}

void ssd1306_Print(I2C_TypeDef *I2Cx, const char *str)
{
	int i, j;
	i = 0;
	while (str[i])
	{
		for (j = 0; j < 5; j++)
		{
			ssd1306_Data(I2Cx, ASCII[(str[i] - 32)][j]);
		}
		i++;
	}
}

void ssd1306_Goto(I2C_TypeDef *I2Cx, uint8_t yPos, uint8_t xPos, const char *str)
{
	ssd1306_Pos(I2Cx, yPos, xPos);
	ssd1306_Print(I2Cx, str);
}

