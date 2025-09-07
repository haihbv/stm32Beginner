#ifndef __DS3231_H
#define __DS3231_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"                  // Device header

#define DS3231_ADDRESS					0x68 << 1
#define DS3231_REG_SECONDS			0x00

static inline uint8_t Bcd_To_Dec(uint8_t bcd_val)
{
    return ((bcd_val >> 4) * 10 + (bcd_val & 0x0F));
}

static inline uint8_t Dec_to_Bcd(uint8_t dec_val)
{
    return (uint8_t)(((dec_val / 10) << 4) | (dec_val % 10));
}


// DS3231 + I2C : Config
void DS3231_I2C_Start(I2C_TypeDef *I2Cx);
void DS3231_I2C_Stop(I2C_TypeDef *I2Cx);
void DS3231_I2C_Write_Addr(I2C_TypeDef *I2Cx, uint8_t address, uint8_t is_read);
uint8_t DS3231_I2C_Write_Data(I2C_TypeDef *I2Cx, uint8_t data);
uint8_t DS3231_I2C_Read_Data(I2C_TypeDef *I2Cx, uint8_t _ack);

// DS3231
void DS3231_Write_Checked(I2C_TypeDef *I2Cx, uint8_t data);
void DS3231_I2C_Init(I2C_TypeDef *I2Cx);
void DS3231_Set_Time(I2C_TypeDef *I2Cx ,uint8_t hour, uint8_t minute, uint8_t second);
void DS3231_Get_Time(I2C_TypeDef *I2Cx ,uint8_t *hour, uint8_t *minute, uint8_t *second);

#ifdef __plusplus
}
#endif

#endif //__DS3231_H
