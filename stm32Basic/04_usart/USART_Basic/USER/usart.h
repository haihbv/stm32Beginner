#ifndef USART_H
#define USART_H

#include "stm32f10x.h"                  // Device header

void USARTx_Init(USART_TypeDef *USARTx, uint32_t baudrate);
void USART_SendChar(USART_TypeDef *USARTx, char c);
void USART_SendStr(USART_TypeDef *USARTx, const char *str);
uint8_t USART_ReceiveChar(USART_TypeDef *USARTx);
void USART_ReceiveStr(USART_TypeDef *USARTx, char *buffer, uint16_t length);

#endif // USART_H
