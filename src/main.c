#include <stddef.h>
#include "stm32l1xx.h"
#include "vrs_cv5.h"
#include <stdio.h>

uint16_t adc_value = 0;
uint16_t printmode = 0;
/*
 int main(void) {

 nvic_init();
 usart_init();
 gpio_init();
 adc_init();

 while (1) {
 ADC_SoftwareStartConv(ADC1);
 while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
 }
 int AD_value = adc_value; // do AD_value  sa mi zapisuje hodnota vystupu z tlacidiel

 // èas kde pozorujem ktore tlacidlo je stlacene a podla toho nastavujem roznu rychlost blikania ledky
 //ak tlacidlo drzim stlacene tak ledka blika nizsie nastavenou rychlostou
 //pokial nedrzim tlacidlo tak sa ledka vypne
 //pre jednotlive tlacidla sme urcili rozmedzie pre AD_value +-cca 5.
 //pre tlacidlo 1 (najblizsie k dratom) to boli hodnoti cca v zormedzi 4046 až 4049
 if (AD_value > 3640 && AD_value < 3680) {
 GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
 delay(25000);
 }
 //tlacidlo 2
 else if (AD_value > 3445 && AD_value < 3490) {
 GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
 delay(50000);
 }
 //tlacidlo 3
 else if (AD_value > 2900 && AD_value < 2950) {
 GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
 delay(75000);
 }
 //tlacidlo 4
 else if (AD_value > 2000 && AD_value < 2050) {
 GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
 delay(100000);
 }
 // nic nie je zatlacene
 else {
 GPIO_ResetBits(GPIOA, GPIO_Pin_5);
 }
 }
 return 0;
 }*/



int main(void) {

	nvic_init();
	usart_init();
	gpio_init();
	adc_init();
	USART_SendData(USART2, '\n');
	USART_SendData(USART2, '\r');
	//Infinite loop
	char vypis[128];
	while (1) {
		/*if (USART2->SR & USART_FLAG_RXNE)
		{
			char d = USART_ReceiveData(USART2);
			while (~USART2->SR & USART_FLAG_TXE);
			USART_SendData(USART2, d);
		}*/
		if (printmode) {
			float val = (adc_value / 1241.21f) * 1000;
			int a = val / 1000;
			int b = (int)val % 1000;
			sprintf(vypis, "%d.%u V    \r", a, b);
		} else {
			sprintf(vypis, "%d       \r", adc_value);
		}

		UART_puts(vypis);
		delay(10000);
	}

	return 0;
}

//dole nepodstatne
#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
/* User can add his own implementation to report the file name and line number,
 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

/* Infinite loop */
while (1)
{
}
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func,
	const char *failedexpr) {
while (1) {
}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr) {
__assert_func(file, line, NULL, failedexpr);
}
