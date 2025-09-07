#include "hc_sr04.h"
#include "stm32f10x.h" // Device header
#include "delay.h"

// Bi?n to�n c?c
static volatile uint32_t capture_rising = 0;
static volatile uint32_t capture_falling = 0;
static volatile uint8_t is_captured = 0;

void HCSR04_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // Enable clock
    RCC_APB2PeriphClockCmd(HCSR04_TRIG_RCC | HCSR04_ECHO_RCC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(HCSR04_TIMER_RCC, ENABLE);

    // TRIG output
    GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HCSR04_TRIG_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN);

    // ECHO input
    GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HCSR04_ECHO_PORT, &GPIO_InitStructure);

    // Timer base
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 1 MHz -> 1us
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(HCSR04_TIMER, &TIM_TimeBaseStructure);

    // Input Capture CH1
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(HCSR04_TIMER, &TIM_ICInitStructure);

    TIM_ITConfig(HCSR04_TIMER, TIM_IT_CC1, ENABLE);
    TIM_Cmd(HCSR04_TIMER, ENABLE);

    // NVIC TIM2
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// Phat xung TRIG 10us
static void HCSR04_Trigger(void)
{
    GPIO_SetBits(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN);
    Delay_Us(10);
    GPIO_ResetBits(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN);
}

uint32_t HCSR04_GetDistance(void)
{
    uint32_t sum = 0;
    for (int i = 0; i < 3; i++)
    {
        capture_rising = 0;
        capture_falling = 0;
        is_captured = 0;
        TIM_SetCounter(HCSR04_TIMER, 0);

        HCSR04_Trigger();

        // cho capture xong
        uint32_t timeout = 30000; // ~30 ms
        while (!is_captured && timeout--)
            Delay_Us(1);
        if (!is_captured)
        {
            return 0;
        }

        uint32_t pulse_width = (capture_falling >= capture_rising) ? (capture_falling - capture_rising) : (0xFFFF - capture_rising + capture_falling + 1);

        sum += pulse_width / 58; // cm
        Delay_Ms(50);
    }
    return sum / 3;
}

// Ng?t TIM2
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(HCSR04_TIMER, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(HCSR04_TIMER, TIM_IT_CC1);

        if ((TIM2->CCER & TIM_CCER_CC1P) == 0)
        {
            capture_rising = TIM_GetCapture1(HCSR04_TIMER);
            TIM2->CCER |= TIM_CCER_CC1P;
        }
        else
        {
            capture_falling = TIM_GetCapture1(HCSR04_TIMER);
            is_captured = 1;
            TIM2->CCER &= ~TIM_CCER_CC1P;
        }
    }
}
