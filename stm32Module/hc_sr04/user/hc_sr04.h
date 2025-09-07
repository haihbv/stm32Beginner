#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "stm32f10x.h"

// Ch?n ch�n TRIG / ECHO
#define HCSR04_TRIG_PORT    GPIOA
#define HCSR04_TRIG_PIN     GPIO_Pin_1
#define HCSR04_TRIG_RCC     RCC_APB2Periph_GPIOA

#define HCSR04_ECHO_PORT    GPIOA
#define HCSR04_ECHO_PIN     GPIO_Pin_0
#define HCSR04_ECHO_RCC     RCC_APB2Periph_GPIOA

#define HCSR04_TIMER        TIM2
#define HCSR04_TIMER_RCC    RCC_APB1Periph_TIM2

void HCSR04_Init(void);
uint32_t HCSR04_GetDistance(void);      
void TIM2_IRQHandler(void);

#endif
