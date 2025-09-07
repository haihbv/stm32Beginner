#ifndef __BUTTON_H
#define __BUTTON_H

#ifdef __cpluplus
extern "C" {
#endif

#include "stm32f10x.h"

#define BUTTON_PIN 			GPIO_Pin_0
#define BUTTON_PORT 		GPIOA
#define BUTTON_RCC			RCC_APB2Periph_GPIOA

void Button_Init(void);
uint8_t Button_Read(void);

#ifdef __cplusplus
}
#endif

#endif // __BUTTON_H
