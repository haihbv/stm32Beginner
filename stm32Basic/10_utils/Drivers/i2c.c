#include "i2c.h"

void I2Cx_Init(I2C_TypeDef *I2Cx, uint32_t I2C_ClockSpeed)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	I2C_InitTypeDef		I2C_InitStruct;
	if (I2Cx == I2C1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		
		// SCL - SDA
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if (I2Cx == I2C2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		
		// SCL - SDA
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else
	{
		return;
	}
	I2C_InitStruct.I2C_ClockSpeed = I2C_ClockSpeed;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2Cx, &I2C_InitStruct);
	I2C_Cmd(I2Cx, ENABLE);
}
void I2C_Start(I2C_TypeDef *I2Cx)
{
	I2C_GenerateSTOP(I2Cx, DISABLE);
	I2C_GenerateSTART(I2Cx, ENABLE);
	while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) == RESET);
}
void I2C_Stop(I2C_TypeDef *I2Cx)
{
	I2C_GenerateSTOP(I2Cx, ENABLE);
}
void I2C_WriteAddress(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Is_Read)
{
	I2C_Send7bitAddress(I2Cx, Address, (Is_Read) ? I2C_Direction_Receiver : I2C_Direction_Transmitter);
	
	if (Is_Read)
	{
		while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == RESET);
	}
	else
	{
		while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == RESET);
	}
}
uint8_t I2C_WriteData(I2C_TypeDef *I2Cx, uint8_t Data)
{
	I2C_SendData(I2Cx, Data);
	
	while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == RESET);
	
	if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET)
	{
		I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
		I2C_Stop(I2Cx);
		return 1; // error (nack)
	}
	return 0; // success
}
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx, uint8_t ACK)
{
	if (ACK)
	{
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
	}
	else
	{
		I2C_AcknowledgeConfig(I2Cx, DISABLE);
	}
	while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == RESET);
	return I2C_ReceiveData(I2Cx);
}
