#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"

#define RX_BUFFER_SIZE 256

typedef struct
{
	volatile char buffer[RX_BUFFER_SIZE];
	volatile uint16_t head;
	volatile uint16_t tail;
} RingBuffer_t;

typedef struct USART_Handle USART_Handle_t;

extern USART_Handle_t UART1;
extern USART_Handle_t UART2;

struct USART_Handle
{
	USART_TypeDef *Instance;
	RingBuffer_t rxBuffer;
	
	void (*sendByte)(USART_Handle_t *huart, uint8_t data);
	void (*sendString)(USART_Handle_t *huart, const char *str);
	int (*available)(USART_Handle_t *huart);
	char (*getChar)(USART_Handle_t *huart);
};

void USART_InitHandle(USART_Handle_t *huart, USART_TypeDef *USARTx, uint32_t baudrate);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif // __USART_H
