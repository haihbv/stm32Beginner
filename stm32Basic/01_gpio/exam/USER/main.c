#include "gpio.h"

/*Variable*/
static uint8_t button_stable = 1;
static uint16_t sample_count = 0;
static uint64_t button_log = 0;


/*Function*/
void Read_Button(void);
void Delay_Ms(uint32_t time);

int main(void) {
  Config_Gpio();

  uint8_t led1_state = 0;
  uint8_t led2_state = 0;
  uint8_t led3_state = 0;

  uint8_t prev_button_state = 1;
  uint16_t led1_timer_ms = 0;
  uint16_t led2_timer_ms = 0;


  while (1) {
    Read_Button();

    if (button_stable == 0 && prev_button_state == 1) {
      led3_state = !led3_state;
    }
    prev_button_state = button_stable;

    //LED3
    if (led3_state) {
      GPIO_ResetBits(GPIOA, GPIO_Pin_2);
    } else {
      GPIO_SetBits(GPIOA, GPIO_Pin_2);
    }

    //LED1
    if (led1_timer_ms >= 3000) {
      led1_timer_ms = 0;
      led1_state = !led1_state;
      if (led1_state) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
      } else {
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
      }
    }

    //LED2
    if (led2_timer_ms >= 6000) {
      led2_timer_ms = 0;
      led2_state = !led2_state;
      if (led2_state) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
      } else {
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
      }
    }

    led1_timer_ms++;
    led2_timer_ms++;
    Delay_Ms(1);
  }
}

/*
	delay 1ms
	time: thoi gian delay tinh theo ms
*/
void Delay_Ms(uint32_t time) {
  uint16_t i, j;
  for (i = 0; i < time; i++) {
    for (j = 0; j < 0x2AFF; j++)
      ;
  }
}

/*
	thuat toan quet nut nhan -> chong nhieu
*/
void Read_Button(void) {
  char input_value = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
  button_log = button_log << 1;
  if (input_value) button_log |= 1;

  if (sample_count < 32) {
    sample_count++;
  }

  else {
    uint8_t logic_high_cnt = 0;
    for (uint8_t i = 0; i < 32; i++) {
      if ((button_log >> i) & 0x01) {
        logic_high_cnt++;
      }
    }

    float logic_high_percent = (logic_high_cnt * 100.0f / 32);
    if (logic_high_percent > 70) {
      button_stable = 1;
    } else {
      button_stable = 0;
    }
  }
}
