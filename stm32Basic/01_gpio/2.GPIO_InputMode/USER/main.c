#include "stm32f10x.h"

void GPIO_Config(void);
void delay_ms(uint32_t ms);

int main(void)
{
  GPIO_Config();

  uint8_t button_prev = 1;

  while (1)
  {
    uint8_t button_curr = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);

    if (button_curr == 0 && button_prev == 1)
    {
      delay_ms(20);
      if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
      {
        GPIOC->ODR ^= GPIO_Pin_13;
      }
    }

    button_prev = button_curr;
  }
}

void GPIO_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef gpio;

  gpio.GPIO_Pin = GPIO_Pin_0;
  gpio.GPIO_Mode = GPIO_Mode_IPU;
  gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &gpio);

  gpio.GPIO_Pin = GPIO_Pin_13;
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &gpio);

  GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void delay_ms(uint32_t ms)
{
  volatile uint32_t i, j;
  for (i = 0; i < ms; i++)
  {
    for (j = 0; j < 0x1995; j++)
      ;
  }
}
