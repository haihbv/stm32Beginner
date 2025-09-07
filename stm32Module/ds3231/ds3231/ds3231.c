#include "ds3231.h"

void DS3231_Write_Checked(I2C_TypeDef *I2Cx, uint8_t data)
{
    if (DS3231_I2C_Write_Data(I2Cx, data))
    {
        DS3231_I2C_Stop(I2Cx);
        return; 
    }
}

void DS3231_I2C_Init(I2C_TypeDef *I2Cx)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	if (I2Cx == I2C1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if (I2Cx == I2C2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else
	{
		return;
	}
	
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2Cx, &I2C_InitStruct);
	I2C_Cmd(I2Cx, ENABLE);
}

void DS3231_I2C_Start(I2C_TypeDef *I2Cx)
{
	I2C_GenerateSTOP(I2Cx, DISABLE);
	
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) == RESET); // wait SB is set
}

void DS3231_I2C_Stop(I2C_TypeDef *I2Cx)
{
	I2C_GenerateSTOP(I2Cx, ENABLE);
}
void DS3231_I2C_Write_Addr(I2C_TypeDef *I2Cx, uint8_t address, uint8_t is_read)
{
	I2C_Send7bitAddress(I2Cx, address, (is_read) ? I2C_Direction_Receiver : I2C_Direction_Transmitter);
	
	if (is_read)
	{
		while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == RESET);
	}
	else 
	{
		while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == RESET);
	}
}
uint8_t DS3231_I2C_Write_Data(I2C_TypeDef *I2Cx, uint8_t data)
{
	I2C_SendData(I2Cx, data);
	
	while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == RESET);
	
	if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET)
	{
		I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
		DS3231_I2C_Stop(I2Cx); // fail
		return 1;
	}
	return 0; // success
}

uint8_t DS3231_I2C_Read_Data(I2C_TypeDef *I2Cx, uint8_t _ack)
{
	if (_ack)
	{
		// enable ACK
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
	}
	else
	{
		// disable ACK
		I2C_AcknowledgeConfig(I2Cx, DISABLE);
	}
	
	// wait RxNE = 1 (data register is new data)
	while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == RESET);
	
	return I2C_ReceiveData(I2Cx);
}

void DS3231_Set_Time(I2C_TypeDef *I2Cx ,uint8_t hour, uint8_t minute, uint8_t second)
{
	DS3231_I2C_Start(I2Cx);
	DS3231_I2C_Write_Addr(I2Cx, DS3231_ADDRESS, 0);
	
	DS3231_Write_Checked(I2Cx, DS3231_REG_SECONDS);
	DS3231_Write_Checked(I2Cx, Dec_to_Bcd(second));
	DS3231_Write_Checked(I2Cx, Dec_to_Bcd(minute));
	DS3231_Write_Checked(I2Cx, Dec_to_Bcd(hour));
	
	DS3231_I2C_Stop(I2Cx);
}

void DS3231_Get_Time(I2C_TypeDef *I2Cx ,uint8_t *hour, uint8_t *minute, uint8_t *second)
{
	DS3231_I2C_Start(I2Cx);
	DS3231_I2C_Write_Addr(I2Cx, DS3231_ADDRESS, Bit_RESET);
	DS3231_I2C_Write_Data(I2Cx, DS3231_REG_SECONDS);
	
	DS3231_I2C_Start(I2Cx);
	DS3231_I2C_Write_Addr(I2Cx, DS3231_ADDRESS, 1);
	
	*second = Bcd_To_Dec(DS3231_I2C_Read_Data(I2Cx, 1));
	*minute = Bcd_To_Dec(DS3231_I2C_Read_Data(I2Cx, 1));
	*hour = Bcd_To_Dec(DS3231_I2C_Read_Data(I2Cx, 0));
	
	DS3231_I2C_Stop(I2Cx);
}

