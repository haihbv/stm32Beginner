#ifndef UART_H
#define UART_H

#include "stm32f10x.h"
#include <stdarg.h>

// ======================== Data Types ========================
typedef struct
{
    void (*init)(uint32_t baudrate);
    void (*printf)(const char *fmt, ...);
    uint8_t (*scanf)(uint8_t *data);
} uart_driver_t;

typedef enum
{
    UART_PORT1 = 0,
    UART_PORT2,
    UART_PORT3
} uart_port_t;

// ======================== Global Variables ========================
extern uart_driver_t uart1;
extern uart_driver_t uart2;
extern uart_driver_t uart3;

// ======================== API ========================

// Initialize UART
void uart1_init(uint32_t baudrate);
void uart2_init(uint32_t baudrate);
void uart3_init(uint32_t baudrate);

// printf data
void uart1_printf(const char *fmt, ...);
void uart2_printf(const char *fmt, ...);
void uart3_printf(const char *fmt, ...);

// Receive data
uint8_t uart1_scanf(uint8_t *data);
uint8_t uart2_scanf(uint8_t *data);
uint8_t uart3_scanf(uint8_t *data);

// IRQ Handlers
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

#endif // UART_H
