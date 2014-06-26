
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "stm32f4_discovery_lis302dl.h"
#include "main.h"
#include "dcmi_ov9655.h"
#include "usart.h"
#include "dct.h"
#include "jpegenc.h"


/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup DCMI_OV9655_Camera
  * @{
  */ 
//#define YUVDEBUG
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DCMI_DR_ADDRESS     0x50050028
#define FSMC_LCD_ADDRESS    0x60100000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t KeyPressFlg = 0;
__IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
EXTI_InitTypeDef   EXTI_InitStructure;
//uint8_t capture_Flag = ENABLE;
uint8_t imagearray[FULLIMAGESIZE*2];//UYVY needs 2 bytes per pixel
static uint8_t frame_done;
/* Private function prototypes -----------------------------------------------*/
uint8_t DCMI_OV9655Config(void);
void DCMI_Config(void);
void I2C1_Config(void);
void EXTILine0_Config(void);
void LIS302DL_Reset(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
  
void write_jpeg(const unsigned char * _buffer,const unsigned int    _n)
{
   //write data to output file
	int i=0;
	char s[6];
	while(i<_n){
		sprintf(s,"%x,",_buffer[i]);
        USART_puts(USART2,s);
		i++;
	}
	;
}  
  
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
  /* SysTick end of count event each 10ms */
#ifndef  YUVDEBUG
  unsigned int line;
#endif   
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  
    init_USART1(1048576);
    
    USART_puts(USART2,"\nLoaded,");
    
    

    frame_done=0;

		   
  /*Camera configuration */
  
  // ****INIT the TW9910 in the following function
  if (DCMI_OV9655Config() == 0x00)//configures pins of DCMI,I2C and DMA and the camera settings, if it returns a positive response
  {
      //Successful
    int fr=0;
    USART_puts(USART2,"In TW9910\n");


    /* Enable DMA transfer */
    DMA_Cmd(DMA2_Stream1, ENABLE);

    /* Enable DCMI interface */
    DCMI_Cmd(ENABLE); 

    /* Start Image capture */ 
    DCMI_CaptureCmd(ENABLE);   

    while (1)
    {
//    int i;
//    Delay(50);
        
      if (frame_done) {
          char s[5];

          int i;
          fr++;

          DCMI_CaptureCmd(DISABLE);
#ifndef YUVDEBUG
        USART_puts(USART2,"\njpeg2\n");
        huffman_start(IMG_HEIGHT & -8, IMG_WIDTH & -8);
        huffman_resetdc();
#endif
         
#ifdef YUVDEBUG  
        USART_puts(USART2,"\nNewFrame\n");
      for(i=0;i<FULLIMAGESIZE*2;i++){
          sprintf(s,",%x",imagearray[i]);
          USART_puts(USART2,s);     
        }
#endif

#ifndef YUVDEBUG
            for (line=0; line<NUM_LINES; line++) {
                uint8_t* line_buffer=(uint8_t *)&(imagearray[line*(IMG_WIDTH*8*2)]);
                // encode the line using encoder
               encode_line_yuv(line_buffer,   line);
            }
           // write .jpeg footer (end-of-image marker)
            huffman_stop();
#endif  
// /* print the frame counter */            
//            sprintf(s,",%d",fr);
//            USART_puts(USART2,s);
//USART_puts(USART2,"\njpegends\n");
            
           DCMI_CaptureCmd(ENABLE);
           frame_done = 0;
      
      }
    }  
  }
  else {// Cant INIT the TW9910 
    USART_puts(USART2,"Cant Init TW9910");
    /* Go to infinite loop */
    while (1);      
  }
}

/**
  * @brief  Configures all needed resources (I2C, DCMI and DMA) to interface with
  *         the OV9655 camera module
  * @param  None
  * @retval 0x00 Camera module configured correctly 
  *         0xFF Camera module configuration failed
  */
uint8_t DCMI_OV9655Config(void)
{
    uint8_t readvalue;
    char str[8];
    uint8_t opmask=MODE|LEN|OEN_TRI_SEL_MASK;
  /* I2C1 will be used for camera configuration */
  I2C1_Config();  
//  - Read ID and check
    readvalue=DCMI_SingleRandomRead(TW9910_DEVICE_READ_ADDRESS,ID);
    sprintf(str,"ID=%x,",readvalue);
    USART_puts(USART2,str);
    
    
//- Write opform register
    //Mode=0, Len=0, rest default
    tw9910_mask_set(TW9910_DEVICE_WRITE_ADDRESS, OPFORM, opmask, 0x00|OEN_TRI_SEL_ALL_ON);
    
    
//- set resolution(vscale,hscale or vactive,hactive and vdelay hdelay)
      /* OV9655 Camera size setup */    
  DCMI_TW9910_SizeSetup();
    




//- write the hsync vsync pin configuration(output control 0x05)
 DCMI_SingleRandomWrite(TW9910_DEVICE_WRITE_ADDRESS, OUTCTR1, 0x02);//**0x11,0x01,0x10?
 //- write clock
 DCMI_SingleRandomWrite(TW9910_DEVICE_WRITE_ADDRESS, OUTCTR2, 0x00);

//- write vbi control register for invalid bit settings and Hactive enable
tw9910_mask_set(TW9910_DEVICE_WRITE_ADDRESS,VBICNTL,0x18, 0x18);//Enable HA_EN**0,1? and cntl656


//- read and print status registers(Nicely! - normal status, detected format etc.)
    readvalue=DCMI_SingleRandomRead(TW9910_DEVICE_READ_ADDRESS,STATUS1);
    sprintf(str,"St1=%x,",readvalue);
    USART_puts(USART2,str);
    while((DCMI_SingleRandomRead(TW9910_DEVICE_READ_ADDRESS,STATUS1)&~0x10)!=0x68){
    
        
        Delay(10);
    }
    readvalue=DCMI_SingleRandomRead(TW9910_DEVICE_READ_ADDRESS,STATUS1);
    sprintf(str,"St1=%x,",readvalue);
//    USART_puts(USART2,str);
    readvalue=DCMI_SingleRandomRead(TW9910_DEVICE_READ_ADDRESS,STATUS2);
    sprintf(str,"St2=%x,",readvalue);
    USART_puts(USART2,str);
    readvalue=DCMI_SingleRandomRead(TW9910_DEVICE_READ_ADDRESS,SDT);
    sprintf(str,"Sdt=%x\n",readvalue);
    USART_puts(USART2,str);


  /* Configure the DCMI to interface with the OV9655 camera module */
  DCMI_Config();
  
  return (0x00);
}

/**
  * @brief  Configures the I2C1 used for OV9655 camera module configuration.
  * @param  None
  * @retval None
  */
void I2C1_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStruct;

 /* I2C1 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 

  /* Connect I2C1 pins to AF4 ************************************************/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);  
  
  /* Configure I2C1 GPIOs *****************************************************/  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure I2C1 ***********************************************************/  
  /* I2C DeInit */   
  I2C_DeInit(I2C1);
    
  /* Enable the I2C peripheral */
  I2C_Cmd(I2C1, ENABLE);
 
  /* Set the I2C structure parameters */
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStruct.I2C_ClockSpeed = 30000;
  
  /* Initialize the I2C peripheral w/ selected parameters */
  I2C_Init(I2C1, &I2C_InitStruct);
}

/**
  * @brief  Configures the DCMI to interface with the OV9655 camera module.
  * @param  None
  * @retval None
  */
void DCMI_Config(void)
{
  DCMI_InitTypeDef DCMI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef  DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable DCMI GPIOs clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE | 
                         RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE); 

  /* Enable DCMI clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

  /* Connect DCMI pins to AF13 ************************************************/
  /* PCLK */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);
  /* D0-D7 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI);
  /* VSYNC */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);
  /* HSYNC */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);
  
  /* DCMI GPIO configuration **************************************************/
  /* D0 D1(PC6/7) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* D2..D4(PE0/1/4) D6/D7(PE5/6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 
	                              | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* D5(PB6), VSYNC(PB7) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* PCLK(PA6) HSYNC(PA4)*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* DCMI configuration *******************************************************/ 
  DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;
  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_Low;
  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;
  DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  
  DCMI_Init(&DCMI_InitStructure);

  /* Configures the DMA2 to transfer Data from DCMI to the LCD ****************/
  /* Enable DMA2 clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  
  /* DMA2 Stream1 Configuration */  
  DMA_DeInit(DMA2_Stream1);

  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)imagearray;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = (FULLIMAGESIZE*2*2)/4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  
  //Enable DMA1 channel IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
  DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);

  
}

void DMA2_Stream1_IRQHandler(void)
{
   
    
  //Test on DMA1 Channel1 Transfer Complete interrupt
    
  if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1))
  {  
      if(!frame_done){   
      USART_puts(USART2,"\nIn INTR\n");
      frame_done=1;
     }
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
      
  }
}

/**

  * @brief  
  * @param  None
  * @retval None
  */
//void LIS302DL_Reset(void)
//{
//  uint8_t ctrl = 0;
//  
//  LIS302DL_InitTypeDef  LIS302DL_InitStruct;
//  LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;  
//  
//  /* Set configuration of LIS302DL*/
//  LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
//  LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
//  LIS302DL_InitStruct.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE;
//  LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
//  LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
//  LIS302DL_Init(&LIS302DL_InitStruct);
//    
//  /* Set configuration of Internal High Pass Filter of LIS302DL*/
//  LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_LATCHED;
//  LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE;
//  LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE;
//  LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);

//  /* Required delay for the MEMS Accelerometre: Turn-on time = 3/Output data Rate 
//                                                             = 3/100 = 30ms */
//  Delay(30);
//  
//  /* Configure Click Window */
//  ctrl = 0xC0;
//  LIS302DL_Write(&ctrl, LIS302DL_CLICK_CTRL_REG3_ADDR, 1);
//}

///**
//  * @brief  MEMS accelerometre management of the timeout situation.
//  * @param  None.
//  * @retval None.
//  */
//uint32_t LIS302DL_TIMEOUT_UserCallback(void)
//{
//  /* MEMS Accelerometer Timeout error occured */
//  while (1) ;
//}
 
/**
  * @brief  Configures EXTI Line0 (connected to PA0 pin) in interrupt mode
  * @param  None
  * @retval None
  */
//void EXTILine0_Config(void)
//{
//  
//  GPIO_InitTypeDef   GPIO_InitStructure;
//  NVIC_InitTypeDef   NVIC_InitStructure;

//  /* Enable GPIOA clock */
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//  /* Enable SYSCFG clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//  
//  /* Configure PA0 pin as input floating */
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  /* Connect EXTI Line0 to PA0 pin */
////  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

////  /* Configure EXTI Line0 */
////  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
////  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
////  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
////  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
////  EXTI_Init(&EXTI_InitStructure);

////  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
////  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
////  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
////  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
////  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////  NVIC_Init(&NVIC_InitStructure);
//}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds
  * @retval None
  */
void Delay(uint32_t nTime)
{
  TimingDelay = nTime;

  while (TimingDelay != 0);

}

/**

  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

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

/**
  * @}
  */



/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
