#ifndef __DMA_USART_H
#define __DMA_USART_H

#include "stm32f10x.h"       // Device header
#include "stm32f10x_dma.h"   // Keil::Device:StdPeriph Drivers:DMA
#include "stm32f10x_rcc.h"   // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"  // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_usart.h" // Keil::Device:StdPeriph Drivers:USART

#define RX_BUFFER_SIZE 32

extern uint8_t rx_buffer[RX_BUFFER_SIZE];

void DMA_USART1_Init(uint32_t baudrate);
void DMA_USART1_Send(uint8_t *data, uint16_t len);

void delay_ms(uint32_t ms);
#endif
