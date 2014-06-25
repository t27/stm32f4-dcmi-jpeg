#include <stm32f4xx.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <stm32f4xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src

#define MAX_STRLEN 12 // this is the maximum string length of our string in characters


void USART1_IRQHandler(void);
void Delay2(__IO uint32_t nCount);
void init_USART1(uint32_t baudrate);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void USART_writebyte(USART_TypeDef* USARTx, uint8_t *dat);
