

#include "stm32f4xx.h"

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>

 
void init_GPIO_parallel(void);

//Read TXE Value
int TXEval(void);
void writeParallelFTDIdata(uint8_t* data);

