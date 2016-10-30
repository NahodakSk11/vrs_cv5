#include <stddef.h>
#include "stm32l1xx.h"
#include "vrs_cv5.h"
#include <stm32l1xx_gpio.h>
#include <string.h>

char buffer[128];
char *vypis_ptr = NULL;

volatile int indexVypis = 0;
char nextZnak = 0;

//fukncia prerusenia adc1
void ADC1_IRQHandler(void) {
	if (ADC1->SR & ADC_SR_EOC) {
		adc_value = ADC1->DR;
	}
}

//fukncia prerusenia usart
void USART2_IRQHandler() {
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		if (USART_ReceiveData(USART2) == 'm') {
			printmode = !printmode;
		}
	}

	else if (USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
		TC_Interrupt_Handle();
	}
}

//funkcia ktora nastane pri preruseni TC usartu
//sluzi na posielanie dat na terminal za pouzitia preruseni
void TC_Interrupt_Handle(void) {
	if (vypis_ptr) {
		USART_SendData(USART2, *vypis_ptr++);
		if (!(*vypis_ptr)) {
			vypis_ptr = 0;
			USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		}
	}
}

void USART_puts_async(char *s) {
	while (vypis_ptr);
	__disable_irq();
	memcpy(buffer, s, 128); // skopiruje string z *s do vypisu
	buffer[127] = 0;
	vypis_ptr = &buffer[0];
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	__enable_irq();
}

void delay(unsigned int i) //nas casovac/spomalovac
{
for (; i; i--)
	;
}
//funkcia na vypis do terminalu
void USART_puts(char *s) {
for (; *s; s++) {
	while (~USART2->SR & USART_FLAG_TXE)
		; // transmit data register empty
	USART_SendData(USART2, *s);
}
}

//funkcia pre inicializaciu gpio periferie
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

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_Init(GPIOA, &GPIO_InitStructure);

GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
}

//funkcia pre inicializaciu usart periferie
void usart_init(void) {
USART_InitTypeDef USART_InitStructure;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

USART_InitStructure.USART_BaudRate = 9600;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1;
USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl =
USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
USART_Init(USART2, &USART_InitStructure);
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//USART_ITConfig(USART2, USART_IT_TC, ENABLE);
USART_Cmd(USART2, ENABLE);
}

//funkcia pre inicializaciu nvic periferie
void nvic_init(void) {
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam prerušení nájdete v súbore stm32l1xx.h
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
NVIC_Init(&NVIC_InitStructure);
}

//funkcia pre inicializaciu adc periferie
void adc_init(void) {
ADC_InitTypeDef ADC_InitStructure;

/* Enable ADC clock */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

/* Enable the HSI oscillator */
RCC_HSICmd(ENABLE);
/* Check that HSI oscillator is ready */
while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	;

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
while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET) {
}
/* Start ADC Software Conversion */
ADC_SoftwareStartConv(ADC1);

ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
//ADC_ITConfig(ADC1, ADC_IT_OVR, ENABLE);
}

