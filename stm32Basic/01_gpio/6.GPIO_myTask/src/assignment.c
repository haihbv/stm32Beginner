#include "assignment.h"

static ButtonState btn_state = BTN_RELEASED;
static uint8_t btn_pressed_flag = 0;
static uint8_t btn_press_count = 0;
uint8_t led3_state = 0;

void delay_ms(uint32_t ms)
{
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 0x2aff; j++)
        {
            __NOP();
        }
    }
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                               RCC_APB2Periph_GPIOB |
                               RCC_APB2Periph_GPIOC,
                           ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Button_Debounced(void)
{
    static uint8_t debounce_count = 0;
    uint8_t raw_input = (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET);

    switch (btn_state)
    {
    case BTN_RELEASED:
        if (raw_input)
        {
            debounce_count = 0;
            btn_state = BTN_PRESS_DEBOUNCE;
        }
        break;
    case BTN_PRESS_DEBOUNCE:
        if (raw_input)
        {
            if (++debounce_count >= 3)
            {
                btn_state = BTN_PRESSED;
            }
        }
        else
        {
            btn_state = BTN_RELEASED;
        }
        break;
    case BTN_PRESSED:
        if (!raw_input)
        {
            debounce_count = 0;
            btn_state = BTN_RELEASE_DEBOUNCE;
        }
        break;
    case BTN_RELEASE_DEBOUNCE:
        if (!raw_input)
        {
            if (++debounce_count >= 3)
            {
                btn_state = BTN_RELEASED;
                btn_pressed_flag = 1;
            }
        }
        else
        {
            btn_state = BTN_PRESSED;
        }
        break;
    }
}
void BTN_StateHandler(void)
{
    if (btn_pressed_flag)
    {
        btn_pressed_flag = 0;
        btn_press_count++;

        if (btn_press_count == 1)
        {
            led3_state = 1;
        }
        else if (btn_press_count == 2)
        {
            led3_state = 0;
        }
        else
        {
            led3_state = !led3_state;
        }

        if (led3_state)
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_13);
        }
        else
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_13);
        }
    }
}
