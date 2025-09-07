#include "servo.h"

#define SERVO_PERIOD 20000

static uint8_t servo_arr[] = {0, 30, 60, 90, 120, 150, 180};
int main()
{
	GPIO_Config();
	TIM1_PWM_Init(SERVO_PERIOD);
	TIM2_PWM_Init();
	while (1)
	{
		uint8_t i;
		uint8_t servo_len = sizeof(servo_arr) / sizeof(servo_arr[0]);
		for (i = 0; i < servo_len; i++)
		{
			Servo_Write_Angle(servo_arr[i]);
			delay_ms(1000);
		}
		for (i = servo_len; i > 0; i--)
		{
			Servo_Write_Angle(servo_arr[i - 1]);
			delay_ms(1000);
		}
	}
}
