#include "stm32f10x.h"
#include "bluetooth.h"

static char c;

static void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);
}

static void Setup(void)
{
	GPIO_Config();
	BT_Init(115200);
	BT_SendStr("Hello Bluetooth HC-05 from STM32!\r\n");
}
static void Loop(void)
{
	c = BT_GetChar();
	BT_SendChar(c);
	
	if (c == 'h')
	{
		GPIOC->ODR ^= GPIO_Pin_13;
	}
}

int main()
{
	Setup();
	while (1)
	{
		Loop();
	}
}
