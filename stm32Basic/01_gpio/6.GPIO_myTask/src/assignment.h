#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

/**
 * @file    assignment.h
 * @brief   write STM32 program to control 3 LEDs
 * @author  Hai Bui Van
 */
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

extern uint8_t led3_state;

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        BTN_RELEASED, // nha
        BTN_PRESS_DEBOUNCE,
        BTN_PRESSED, // nhan
        BTN_RELEASE_DEBOUNCE
    } ButtonState;

    void GPIO_Config(void);
    void delay_ms(uint32_t ms);
    void Button_Debounced(void); // phuong phap lay mau
    void BTN_StateHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSIGNMENT_H */
