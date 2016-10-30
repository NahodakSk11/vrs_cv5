#include <stddef.h>
#include "stm32l1xx.h"
#include "vrs_cv5.h"
#include <stdio.h>

uint16_t adc_value = 0;
uint16_t printmode = 0;
char vypis[128];


int main(void) {

	__disable_irq();
	nvic_init();
	usart_init();
	gpio_init();
	adc_init();
	//USART_SendData(USART2, '\n');
	//USART_SendData(USART2, '\r');
	__enable_irq();

	//Infinite loop
	while (1) {

		if (printmode) {
			float val = (adc_value / 1241.21f) * 1000;
			int a = val / 1000;
			int b = (int)val % 1000;
			sprintf(vypis, "%d.%u V    \r", a, b);
		} else {
			sprintf(vypis, "%d       \r", adc_value);
		}
		USART_puts_async(vypis);
		//USART_puts(vypis);
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
