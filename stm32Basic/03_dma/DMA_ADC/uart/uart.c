#include "uart.h"

// ==================== RX Buffer ====================
#define UART_RX_BUFFER_SIZE 128

typedef struct
{
    uint8_t buffer[UART_RX_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} UART_Buffer_t;

static UART_Buffer_t uart1_rx = {0};
static UART_Buffer_t uart2_rx = {0};
static UART_Buffer_t uart3_rx = {0};

// ==================== Private Utils ====================
static void uart_send_byte(USART_TypeDef *USARTx, uint8_t b)
{
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(USARTx, b);
}

static void itoa(int value, char *str)
{
    char buf[12];
    int i = 0, j = 0, sign = 0;
    if (value < 0)
    {
        sign = 1;
        value = -value;
    }
    do
    {
        buf[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);
    if (sign)
    {
        buf[i++] = '-';
    }
    for (j = 0; j < i; j++)
    {
        str[j] = buf[i - j - 1];
    }
    str[i] = '\0';
}

static void itox(uint32_t value, char *str, uint8_t uppercase)
{
    const char *hex_l = "0123456789abcdef";
    const char *hex_u = "0123456789ABCDEF";
    const char *hex = uppercase ? hex_u : hex_l;
    str[0] = '0';
    str[1] = 'x';
    for (int i = 0; i < 8; i++)
    {
        str[9 - i] = hex[value & 0xF];
        value >>= 4;
    }
    str[10] = '\0';
}

static void ftoa(float f, char *buf, uint8_t decimals)
{
    if (f < 0)
    {
        *buf++ = '-';
        f = -f;
    }
    int int_part = (int)f;
    float frac_part = f - (float)int_part;

    char tmp[12];
    itoa(int_part, tmp);
    char *p = tmp;
    while (*p)
    {
        *buf++ = *p++;
    }

    *buf++ = '.';
    for (uint8_t i = 0; i < decimals; i++)
    {
        frac_part *= 10.0f;
        int digit = (int)(frac_part + 0.5f);
        if (digit > 9)
        {
            digit = 9;
        }
        *buf++ = (char)digit + '0';
        frac_part -= (float)digit;
    }
    *buf = '\0';
}

static void uart_vprintf(USART_TypeDef *USARTx, const char *fmt, va_list args)
{
    char buf[64];
    char ch;

    while ((ch = *fmt++) != '\0')
    {
        if (ch == '%')
        {
            ch = *fmt++;
            uint8_t decimals = 2;
            uint8_t uppercase = 0;

            if (ch == '.')
            {
                ch = *fmt++;
                if (ch >= '0' && ch <= '9')
                    decimals = ch - '0';
                ch = *fmt++;
            }

            if (ch == 'X')
            {
                uppercase = 1;
                ch = 'x';
            }

            switch (ch)
            {
            case 'c':
                uart_send_byte(USARTx, (char)va_arg(args, int));
                break;
            case 's':
            {
                char *s = va_arg(args, char *);
                while (*s)
                {
                    uart_send_byte(USARTx, *s++);
                }
                break;
            }
            case 'd':
            {
                int val = va_arg(args, int);
                itoa(val, buf);
                char *p = buf;
                while (*p)
                {
                    uart_send_byte(USARTx, *p++);
                }
                break;
            }
            case 'x':
            {
                uint32_t val = va_arg(args, uint32_t);
                itox(val, buf, uppercase);
                char *p = buf;
                while (*p)
                {
                    uart_send_byte(USARTx, *p++);
                }
                break;
            }
            case 'f':
            {
                double val = va_arg(args, double);
                ftoa((float)val, buf, decimals);
                char *p = buf;
                while (*p)
                {
                    uart_send_byte(USARTx, *p++);
                }
                break;
            }
            case '%':
                uart_send_byte(USARTx, '%');
                break;
            default:
                uart_send_byte(USARTx, ch);
                break;
            }
        }
        else
            uart_send_byte(USARTx, ch);
    }
}

static uint8_t uart_scan_line(UART_Buffer_t *buf, uint8_t *data)
{
    uint16_t i = 0;
    uint16_t t = buf->tail;

    while (t != buf->head && i < UART_RX_BUFFER_SIZE - 1)
    {
        uint8_t c = buf->buffer[t];
        t = (t + 1) % UART_RX_BUFFER_SIZE;

        if (c == '\n')
        {
            uint16_t idx = buf->tail;
            uint16_t j = 0;
            while (idx != t)
            {
                data[j++] = buf->buffer[idx];
                idx = (idx + 1) % UART_RX_BUFFER_SIZE;
            }
            data[j] = '\0';
            buf->tail = t;
            return 1;
        }
        i++;
    }

    return 0;
}

// ==================== Public API ====================
void uart1_init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // TX: PA9 AF PP, RX: PA10 floating input
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStruct);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);
}

void uart2_init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // TX: PA2 AF PP, RX: PA3 floating input
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStruct);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
    NVIC_EnableIRQ(USART2_IRQn);
}

void uart3_init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // TX: PB10 AF PP, RX: PB11 floating input
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStruct);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
    NVIC_EnableIRQ(USART3_IRQn);
}

void uart1_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    uart_vprintf(USART1, fmt, args);
    va_end(args);
}

void uart2_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    uart_vprintf(USART2, fmt, args);
    va_end(args);
}

void uart3_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    uart_vprintf(USART3, fmt, args);
    va_end(args);
}

uint8_t uart1_scanf(uint8_t *data) { return uart_scan_line(&uart1_rx, data); }
uint8_t uart2_scanf(uint8_t *data) { return uart_scan_line(&uart2_rx, data); }
uint8_t uart3_scanf(uint8_t *data) { return uart_scan_line(&uart3_rx, data); }

// ==================== IRQ Handlers ====================
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART1) & 0xFF;
        uint16_t next = (uart1_rx.head + 1) % UART_RX_BUFFER_SIZE;
        if (next != uart1_rx.tail)
        {
            uart1_rx.buffer[uart1_rx.head] = data;
            uart1_rx.head = next;
        }
    }
}

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART2) & 0xFF;
        uint16_t next = (uart2_rx.head + 1) % UART_RX_BUFFER_SIZE;
        if (next != uart2_rx.tail)
        {
            uart2_rx.buffer[uart2_rx.head] = data;
            uart2_rx.head = next;
        }
    }
}

void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART3) & 0xFF;
        uint16_t next = (uart3_rx.head + 1) % UART_RX_BUFFER_SIZE;
        if (next != uart3_rx.tail)
        {
            uart3_rx.buffer[uart3_rx.head] = data;
            uart3_rx.head = next;
        }
    }
}
