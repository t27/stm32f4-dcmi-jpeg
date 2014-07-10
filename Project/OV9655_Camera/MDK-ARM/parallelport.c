//#include <stm32f4xx.h>
//#include <stm32f4xx_gpio.h>

///* This funcion shows how to initialize 
// * the GPIO pins on GPIOD and how to configure
// * them as inputs and outputs 
// */
//void init_GPIO(void){

//	/* This TypeDef is a structure defined in the
//	 * ST's library and it contains all the properties
//	 * the corresponding peripheral has, such as output mode,
//	 * pullup / pulldown resistors etc.
//	 * 
//	 * These structures are defined for every peripheral so 
//	 * every peripheral has it's own TypeDef. The good news is
//	 * they always work the same so once you've got a hang
//	 * of it you can initialize any peripheral.
//	 * 
//	 * The properties of the periperals can be found in the corresponding
//	 * header file e.g. stm32f4xx_gpio.h and the source file stm32f4xx_gpio.c
//	 */
//	GPIO_InitTypeDef GPIO_InitStruct;

//	/* This enables the peripheral clock to the GPIOD IO module
//	 * Every peripheral's clock has to be enabled 
//	 * 
//	 * The STM32F4 Discovery's User Manual and the STM32F407VGT6's
//	 * datasheet contain the information which peripheral clock has to be used.
//	 * 
//	 * It is also mentioned at the beginning of the peripheral library's 
//	 * source file, e.g. stm32f4xx_gpio.c
//	 */
//     
//    //For Data Pins of
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ; // we want to configure all LED GPIO pins
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; 		// we want the pins to be an output
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; 	// this sets the GPIO modules clock speed
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 	// this sets the pin type to push / pull (as opposed to open drain)
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 	// this sets the pullup / pulldown resistors to be inactive
//	GPIO_Init(GPIOD, &GPIO_InitStruct); 			// this finally passes all the values to the GPIO_Init function which takes care of setting the corresponding bits.

//   
//    
//    
//    //For Rx Enable of FT232H
//     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 ; // we want to configure all LED GPIO pins
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; 		// we want the pins to be an output
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; 	// this sets the GPIO modules clock speed
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 	// this sets the pin type to push / pull (as opposed to open drain)
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 	// this sets the pullup / pulldown resistors to be inactive
//	GPIO_Init(GPIOC, &GPIO_InitStruct); 			
//    
//    
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

//	/* Here the GPIOA module is initialized.
//	 * We want to use PA0 as an input because
//	 * the USER button on the board is connected
//	 * between this pin and VCC.
//	 */
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;		  // we want to configure PB3
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; 	  // we want it to be an input
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//this sets the GPIO modules clock speed
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   // this sets the pin type to push / pull (as opposed to open drain)
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;   // this enables the pulldown resistor --> we want to detect a high level
//	GPIO_Init(GPIOB, &GPIO_InitStruct);

//}

//int main(void){
//  
//  // initialize the GPIO pins we need
//  init_GPIO();

//  /* This flashed the LEDs on the board once
//   * Two registers are used to set the pins (pin level is VCC)
//   * or to reset the pins (pin level is GND)
//   * 
//   * BSRR stands for bit set/reset register
//   * it is seperated into a high and a low word (each of 16 bit size)
//   * 
//   * A logical 1 in BSRRL will set the pin and a logical 1 in BSRRH will
//   * reset the pin. A logical 0 in either register has no effect
//   */
//  GPIOD->BSRRL = 0xF000; // set PD12 thru PD15
// // Delay(1000000L);		 // wait a short period of time
//  GPIOD->BSRRH = 0xF000; // reset PD12 thru PD15
//  
//  // this counter is used to count the number of button presses
//  uint8_t i = 0; 

//  while (1){  
//    
//		/* Every GPIO port has an input and 
//		 * output data register, ODR and IDR 
//		 * respectively, which hold the status of the pin
//		 * 
//		 * Here the IDR of GPIOA is checked whether bit 0 is
//		 * set or not. If it's set the button is pressed
//		 */
//		if(GPIOA->IDR & 0x0001){
//			// if the number of button presses is greater than 4, reset the counter (we start counting from 0!)
//			if(i > 3){
//				i = 0;
//			}
//			else{ // if it's smaller than 4, switch the LEDs

//				switch(i){

//					case 0:
//						GPIOD->BSRRL = 0x1000; // this sets LED1 (green)
//						GPIOD->BSRRH = 0x8000; // this resets LED4 (blue)
//						break;

//					case 1:
//						GPIOD->BSRRL = 0x2000; // this sets LED2 (orange)
//						GPIOD->BSRRH = 0x1000; // this resets LED1 
//						break;

//					case 2:
//						GPIOD->BSRRL = 0x4000; // this sets LED3 (red)
//						GPIOD->BSRRH = 0x2000; // this resets LED2
//						break;

//					case 3: 
//						GPIOD->BSRRL = 0x8000; // this sets LED4
//						GPIOD->BSRRH = 0x4000; // this resets LED3
//						break;
//					}

//				i++; // increase the counter every time the switch is pressed
//			}
//			Delay(3000000L); // add a small delay to debounce the switch
//		}
//	}
//}