#ifndef SERVO_H
#define SERVO_H

#include "stm32f10x.h"     // Device header
#include "stm32f10x_tim.h" // Keil::Device:StdPeriph Drivers:TIM

void delay_ms(uint32_t ms);

void GPIO_Config(void);
void TIM1_PWM_Init(uint16_t period);
void Servo_Write(uint16_t pulse_us);
void Servo_Write_Angle(uint8_t angle);
void TIM2_PWM_Init(void);
void delay_ms(uint32_t ms);
#endif // SERVO_H
