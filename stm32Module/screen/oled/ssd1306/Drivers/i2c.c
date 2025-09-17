#include "i2c.h"

void i2c_Init(I2C_TypeDef *I2Cx)
{
	GPIO_InitTypeDef gpio = {0};
	I2C_InitTypeDef i2c = {0};
	if (I2Cx == I2C1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		gpio.GPIO_Mode = GPIO_Mode_AF_OD;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &gpio);
	}
	else if (I2Cx == I2C2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

		gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		gpio.GPIO_Mode = GPIO_Mode_AF_OD;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &gpio);
	}
	else
	{
		return;
	}

	I2C_DeInit(I2Cx);
	i2c.I2C_ClockSpeed = 100000;
	i2c.I2C_Mode = I2C_Mode_I2C;
	i2c.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c.I2C_OwnAddress1 = 0x00;
	i2c.I2C_Ack = I2C_Ack_Enable;
	i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2Cx, &i2c);
	I2C_Cmd(I2Cx, ENABLE);
}

void i2c_Start(I2C_TypeDef *I2Cx)
{
	I2C_GenerateSTART(I2Cx, ENABLE);
	while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) == RESET)
		;
}
void i2c_Stop(I2C_TypeDef *I2Cx)
{
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

void i2c_Address(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t is_read)
{
	I2C_Send7bitAddress(I2Cx, addr, (is_read) ? I2C_Direction_Receiver : I2C_Direction_Transmitter);

	if (is_read)
	{
		while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == RESET)
			;
	}
	else
	{
		while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == RESET)
			;
	}
}

void i2c_Data(I2C_TypeDef *I2Cx, uint8_t data)
{
	I2C_SendData(I2Cx, data);
	while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == RESET)
		;
}
void i2c_Master_Transmit(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint16_t len)
{
	i2c_Start(I2Cx);
	i2c_Address(I2Cx, addr, 0); // write mode

	for (uint16_t i = 0; i < len; i++)
	{
		i2c_Data(I2Cx, data[i]);
	}
	i2c_Stop(I2Cx);
}
