#include "stm32f10x.h" // Device header

static volatile uint32_t systick_counter = 0;

void SysTick_Init(void);
void SysTick_Handler(void);
void delay_ms(uint32_t ms);

void GPIO_Config(void);
int main()
{
	SysTick_Init();
	GPIO_Config();
	while (1)
	{
		GPIOC->ODR ^= GPIO_Pin_13;
		delay_ms(1000);
	}
}

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &gpio);
}
void SysTick_Init(void)
{
	// SysTick_Config(72000);
	// SysTick_Config(SystemCoreClock);
	SysTick->LOAD = 72000 - 1; // 1ms
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | // use HCLK
					SysTick_CTRL_TICKINT_Msk |	 // enable tick
					SysTick_CTRL_ENABLE_Msk;	 // enable SysTick
}

void SysTick_Handler()
{
	systick_counter++;
}
void delay_ms(uint32_t ms)
{
	volatile uint32_t start = systick_counter;
	while ((systick_counter - start) < ms)
		;
}
