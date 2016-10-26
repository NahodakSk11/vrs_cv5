#ifndef VRS_CV5_H_
#define VRS_CV5_H_


extern uint16_t adc_value;
extern uint16_t printmode;


void gpio_init();
void delay();
void usart_init();
void nvic_init();
void adc_init();
void UART_puts();


#endif /* VRS_CV5_H_ */
