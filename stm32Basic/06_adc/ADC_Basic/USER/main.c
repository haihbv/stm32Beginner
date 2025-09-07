#include "stm32f10x.h" // Device header

void GPIO_Config(void);
void ADC1_Config(void);
uint16_t Read_ADC(void);
void TIM2_PWM_Config(void);
void delay_ms(uint32_t ms);

int main()
{
    GPIO_Config();
    ADC1_Config();
    TIM2_PWM_Config();

    while (1)
    {
        TIM2->CCR2 = Read_ADC();
    }
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // PA0 - analog input
    GPIO_InitTypeDef gpioA0;
    gpioA0.GPIO_Pin = GPIO_Pin_0;
    gpioA0.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &gpioA0);

    // PA1 - output alternate push-pull (PWM)
    GPIO_InitTypeDef gpioA1;
    gpioA1.GPIO_Pin = GPIO_Pin_1;
    gpioA1.GPIO_Speed = GPIO_Speed_50MHz;
    gpioA1.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpioA1);
}
void ADC1_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    ADC_InitTypeDef adcInit;
    adcInit.ADC_Mode = ADC_Mode_Independent;
    adcInit.ADC_ScanConvMode = DISABLE;
    adcInit.ADC_ContinuousConvMode = ENABLE;
    adcInit.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    adcInit.ADC_DataAlign = ADC_DataAlign_Right;
    adcInit.ADC_NbrOfChannel = 1;

    ADC_Init(ADC1, &adcInit);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1))
        ;

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void delay_ms(uint32_t ms)
{
    SysTick->LOAD = 72000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (volatile uint32_t i = 0; i < ms; i++)
    {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
            ;
    }
    SysTick->CTRL = 0;
}
uint16_t Read_ADC(void)
{
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        ;
    return ADC_GetConversionValue(ADC1);
}
void TIM2_PWM_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef tim;
    tim.TIM_Prescaler = 0;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period = 4095;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &tim);

    TIM_OCInitTypeDef oc;
    oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_Pulse = 0;
    oc.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM2, &oc);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}
