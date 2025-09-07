#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "uart.h"

static volatile uint16_t adc_value[4];

void ADC1_DMA_Init(void);

int main()
{
	SystemInit();
	Delay_Init();
	uart1_init(115200);
	ADC1_DMA_Init();

	while (1)
	{
		uart1_printf("--------------------------------------------------------\n");
		uart1_printf("Val IR1: %d ", adc_value[0]);
		uart1_printf("Val IR2: %d ", adc_value[1]);
		uart1_printf("Val IR3: %d ", adc_value[2]);
		uart1_printf("Val IR4: %d", adc_value[3]);
		uart1_printf("\n--------------------------------------------------------\n");
		Delay_Ms(2000);
	}
}

void ADC1_DMA_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	ADC_InitTypeDef	 	ADC_InitStruct;
	DMA_InitTypeDef		DMA_InitStruct;
	
	// enable clock 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // enable DMA
	
	// config clock for ADC1
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	// config GPIO mode Analog
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	// config DMA1_Channel1 (ADC1)
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)adc_value;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = 4;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// config ADC1
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ScanConvMode = ENABLE; // multi-channel scan
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfChannel = 4;
	ADC_Init(ADC1, &ADC_InitStruct);
	
	// config ADC channel 0, 1, 2, 3
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_71Cycles5);
	
	// enable DMA for ADC1
	ADC_DMACmd(ADC1, ENABLE);
	
	// enable ADC1
	ADC_Cmd(ADC1, ENABLE);
	
	for (volatile int i = 0; i < 1000; i++);
	
	// calib ADC1
	ADC_ResetCalibration(ADC1); // reset calib
	while (ADC_GetResetCalibrationStatus(ADC1)); // wait reset calib set 0: initialized
	
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1)); // wait start calib set 0: completed
	
	// start conversion for ADC
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
