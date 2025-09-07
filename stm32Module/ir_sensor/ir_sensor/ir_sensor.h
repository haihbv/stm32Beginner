#ifndef __IR_SENSOR_H
#define __IR_SENSOR_H

#include "stm32f10x.h"

void IR_ADC_Init(void);
uint16_t IR_Read(void);

#endif
