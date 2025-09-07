#include "rgb.h"

volatile uint32_t tick_ms = 0;

void SysTick_Handler(void)
{
	tick_ms++;
}
void delay_ms(uint32_t ms)
{
	uint32_t start = tick_ms;
	while ((tick_ms - start) < ms)
		;
}

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = RGB_PIN_MASK;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void LED_RGB_Set(uint8_t red, uint8_t green, uint8_t blue)
{
	GPIO_WriteBit(GPIOA, RED_PIN, red ? Bit_RESET : Bit_SET);
	GPIO_WriteBit(GPIOA, GREEN_PIN, green ? Bit_RESET : Bit_SET);
	GPIO_WriteBit(GPIOA, BLUE_PIN, blue ? Bit_RESET : Bit_SET);
}
void LED_RGB_SetCommand(void)
{
	LED_RGB_Set(1, 0, 0); // do
	delay_ms(1000);

	LED_RGB_Set(0, 1, 0); // xanh la
	delay_ms(1000);

	LED_RGB_Set(0, 0, 1); // xanh duong
	delay_ms(1000);

	LED_RGB_Set(1, 1, 0); // vang = do + xanh la
	delay_ms(1000);

	LED_RGB_Set(1, 0, 1); // hong = do + xanh duong
	delay_ms(1000);

	LED_RGB_Set(0, 1, 1); // cyan = xanh la + xanh duong
	delay_ms(1000);

	LED_RGB_Set(1, 1, 1); // trang
	delay_ms(1000);

	LED_RGB_Set(0, 0, 0); // tat
	delay_ms(1000);
}
