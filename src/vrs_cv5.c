#include <stddef.h>
#include "stm32l1xx.h"
#include "vrs_cv5.h"
#include <stm32l1xx_gpio.h>

//fukncia prerusenia
void ADC1_IRQHandler(void) {
	if (ADC1->SR & ADC_SR_EOC) {
		adc_value = ADC1->DR;
	}
}

void delay(unsigned int i) //nas casovac/spomalovac
{
	for (; i; i--)
		;
}



void gpio_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void nvic_init(void) {
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam preru�en� n�jdete v s�bore stm32l1xx.h
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

}


void adc_init(void) {
ADC_InitTypeDef ADC_InitStructure;

/* Enable ADC clock */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

/* Enable the HSI oscillator */
RCC_HSICmd(ENABLE);
/* Check that HSI oscillator is ready */
while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

/* Initialize ADC structure */
ADC_StructInit(&ADC_InitStructure);
/* ADC1 configuration */
ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStructure.ADC_NbrOfConversion = 1;
ADC_Init(ADC1, &ADC_InitStructure);
/* ADCx regular channel8 configuration */
ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_16Cycles); // pinu 1 zodpoveda kanal 1
/* Enable the ADC */
ADC_Cmd(ADC1, ENABLE);
/* Wait until the ADC1 is ready */
while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET) {}
/* Start ADC Software Conversion */
ADC_SoftwareStartConv(ADC1);

ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
//ADC_ITConfig(ADC1, ADC_IT_OVR, ENABLE);
}
