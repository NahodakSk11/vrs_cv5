#include <stddef.h>
#include "stm32l1xx.h"
#include "vrs_cv5.h"
#include <stdio.h>

uint16_t adc_value = 0;
uint16_t printmode = 0;


int main(void) {

	nvic_init();
	usart_init();
	gpio_init();
	adc_init();
	USART_SendData(USART1, '\n');
	//Infinite loop
	char vypis[10];
	uint16_t i = 10;
	while (1) {
		if (i >= 10) {
			if (printmode) {
				sprintf(vypis, "%f", ((float) adc_value) / (1241.21));
				vypis[4] = 'V';
				vypis[5] = ' ';
				vypis[6] = ' ';
				vypis[7] = '\n';
				vypis[8] = '\r';
				vypis[9] = 0;
			} else
				sprintf(vypis, "%d\n\r", adc_value);

			i = 0;
		}

		if (USART1->SR & USART_FLAG_TC) {
			if (i < 10) {

				USART_SendData(USART1, vypis[i]);
				if (vypis[i] == 0) {
					i = 10;
				} else
					i++;
			}
		}
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
