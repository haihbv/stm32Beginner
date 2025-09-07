#include "stm32f10x.h"                  // Device header
#include "ds3231.h"
#include "delay.h"

static uint8_t h, m, s;
void GPIO_Config(void);

int main(void)
{
	SystemInit();
	Delay_Init();
	DS3231_I2C_Init(I2C1);
	
	// DS3231_Set_Time(I2C1, 12, 0, 0);
	GPIO_Config();
	
	while (1)
	{
		DS3231_Get_Time(I2C1, &h, &m, &s);
		if (s > 1 && s < 30)
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
		else
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		Delay_ms(1000);
	}
}

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);
}
