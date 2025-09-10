#include "main.h"

uint8_t whoami;

int main(void)
{
	SystemInit();
	Delay_Init();
	
	I2Cx_Init(I2C1, I2C_ClockSpeed_100kHz);
	
	while (1)
	{
		I2C_Start(I2C1);
		I2C_WriteAddress(I2C1, 0x68 << 1, 0);
		I2C_WriteData(I2C1, 0x00);
		
		I2C_Start(I2C1);
		I2C_WriteAddress(I2C1, 0x68 << 1, 1);
		whoami = I2C_ReadData(I2C1, 0);
		I2C_Stop(I2C1);
	}
}
