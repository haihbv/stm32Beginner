#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"                  // Device header

extern USART_TypeDef *pADCx;

void BT_Init(uint32_t baudrate);
void BT_SendChar(char c);
void BT_SendStr(const char *pStr);
char BT_GetChar(void);

#ifdef __cplusplus
}
#endif

#endif // __BLUETOOTH_H
