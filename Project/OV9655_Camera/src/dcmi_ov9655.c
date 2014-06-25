 
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    dcmi_ov9655.c 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This file includes the driver for OV9655 Camera module mounted on 
  *          STM322xG-EVAL board RevA and RevB.                          
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB, STM32F4DIS-CAM
  *          and STM32F4DIS-LCD modules. 
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "dcmi_ov9655.h"
#include "main.h"

/** @addtogroup DCMI_OV9655_Camera
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  TIMEOUT  2

//#define  CAMERA_RST_PIN			  GPIO_Pin_12	
//#define  CAMERA_RST_PORT		  GPIOD

//#define  CAMERA_PWR_EN_PIN		GPIO_Pin_6
//#define  CAMERA_PWR_EN_PROT		GPIOD

/* Bits definitions ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
******************************************************************************
  Function:       DCMI_Control_IO_Init
  Description:    Init DCMI module power control
  Calls:          void
  Called By:      
  Input:          void
  Output:         void
  Return:         
  Others:         
******************************************************************************
*/
//void DCMI_Control_IO_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;

//  /* Enable GPIOD clocks */
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

//  /* camera RST pin configuration */
//  GPIO_InitStructure.GPIO_Pin = CAMERA_RST_PIN; 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//  GPIO_Init(CAMERA_RST_PORT, &GPIO_InitStructure);
//  /*Reset camera*/
//  GPIO_ResetBits(CAMERA_RST_PORT, CAMERA_RST_PIN);
//  Delay(10);
//  GPIO_SetBits(CAMERA_RST_PORT, CAMERA_RST_PIN);

//  /* camera PWR EN pin configuration */
//  GPIO_InitStructure.GPIO_Pin = CAMERA_PWR_EN_PIN; 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

//  GPIO_Init(CAMERA_PWR_EN_PROT, &GPIO_InitStructure);
//  GPIO_ResetBits(CAMERA_PWR_EN_PROT, CAMERA_PWR_EN_PIN);
//}

 
/**
  * @brief  Reset the OV9655 SCCB registers.
  * @param  None
  * @retval None
  */
//void DCMI_OV9655_Reset(void)
//{
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, SCCB_REG_RESET);    
//}

/**
  * @brief  Set the QVGA size(240*320).
  * @param  None
  * @retval None
  */
void DCMI_OV9655_QVGASizeSetup(void)
{  
    //write 0x400 to both hscale,vscale
    DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,HSCALE_LO,0x00);
    DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,VSCALE_LO,0x00);
    DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,SCALE_HI,0x22);
    //Setup image size over here!!!!
    
}

/**
  * @brief  Set the QQVGA size(120*160).
  * @param  None
  * @retval None
  */
void DCMI_OV9655_QQVGASizeSetup(void)
{
  //set up small image size over here!!
}

/**
  * @brief  Read the OV9655 Manufacturer identifier.
  * @param  OV9655ID: pointer to the OV9655 Manufacturer identifier. 
  * @retval None
  */
//void DCMI_OV9655_ReadID(OV9655_IDTypeDef* OV9655ID)
//{
//  OV9655ID->Manufacturer_ID1 = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_MIDH);
//  OV9655ID->Manufacturer_ID2 = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_MIDL);
//  OV9655ID->Version = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_VER);
//  OV9655ID->PID = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_PID);
//}

/**
  * @brief  Set the Internal Clock Prescaler.
  * @param  OV9655_Prescaler: the new value of the prescaler. 
  *         This parameter can be a value between 0x0 and 0x1F
  * @retval None
  */
//void DCMI_OV9655_SetPrescaler(uint8_t OV9655_Prescaler)
//{
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_CLKRC, OV9655_Prescaler);
//}

/**
  * @brief  Select the Output Format.
  * @param  OV9655_OuputFormat: the Format of the ouput Data.  
  *         This parameter can be one of the following values:
  *           @arg OUTPUT_FORMAT_RAWRGB_DATA 
  *           @arg OUTPUT_FORMAT_RAWRGB_INTERP    
  *           @arg OUTPUT_FORMAT_YUV              
  *           @arg OUTPUT_FORMAT_RGB    
  * @retval None
  */
//void DCMI_OV9655_SelectOutputFormat(uint8_t OV9655_OuputFormat)
//{
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_OuputFormat);
//}

/**
  * @brief  Select the Output Format Resolution.
  * @param  OV9655_FormatResolution: the Resolution of the ouput Data. 
  *         This parameter can be one of the following values:
  *           @arg FORMAT_CTRL_15fpsVGA 
  *           @arg FORMAT_CTRL_30fpsVGA_NoVArioPixel    
  *           @arg FORMAT_CTRL_30fpsVGA_VArioPixel     
  * @retval None
  */
//void DCMI_OV9655_SelectFormatResolution(uint8_t OV9655_FormatResolution)
//{
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_FormatResolution);
//}

/**
  * @brief  Set the new value of OV9655 registers
  * @param  OV9655_Register: the OV9655 Register to be configured. 
  * @param  Register_Val: The new value to be set 
  * @retval None
  */
void DCMI_OV9655_SetRegister(uint8_t OV9655_Register, uint8_t Register_Val)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_Register, Register_Val);
}

/**
  * @brief  Select the HREF Control signal option
  * @param  OV9665_HREFControl: the HREF Control signal option.
  *         This parameter can be one of the following value:
  *           @arg OV9665_HREFControl_Opt1: HREF edge offset to data output. 
  *           @arg OV9665_HREFControl_Opt2: HREF end 3 LSB    
  *           @arg OV9665_HREFControl_Opt3: HREF start 3 LSB      
  * @retval None
  */
//void DCMI_OV9655_HREFControl(uint8_t OV9665_HREFControl)
//{
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_HREF, OV9665_HREFControl);
//}

/**
  * @brief  Select the RGB format option
  * @param  OV9665_RGBOption: the RGB Format option.
  *         This parameter can be one of the following value:
  *           @arg RGB_NORMAL
  *           @arg RGB_565  
  *           @arg RGB_555    
  * @retval None
  */
//void DCMI_OV9655_SelectRGBOption(uint8_t OV9665_RGBOption)
//{
//  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, OV9665_RGBOption);
//}

/**
  * @brief  Writes a byte at a specific Camera register
  * @param  Device: OV9655 write address.
  * @param  Addr: OV9655 register address. 
  * @param  Data: data to be written to the specific register 
  * @retval 0x00 if write operation is OK. 
  *         0xFF if timeout condition occured (device not connected or bus error).
  */
uint8_t DCMI_SingleRandomWrite(uint8_t Device, uint16_t Addr, uint8_t Data)
{
  uint32_t timeout = DCMI_TIMEOUT_MAX;
  
  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }
   
  /* Send DCMI selcted device slave Address for write */
  I2C_Send7bitAddress(I2C1, Device, I2C_Direction_Transmitter);
 
  /* Test on I2C1 EV6 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }
 
  /* Send I2C1 location address LSB */
  I2C_SendData(I2C1, (uint8_t)(Addr));

  /* Test on I2C1 EV8 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }
  
  /* Send Data */
  I2C_SendData(I2C1, Data);    

  /* Test on I2C1 EV8 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }  
 
  /* Send I2C1 STOP Condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
  
  /* If operation is OK, return 0 */
  return 0;
}

/**
  * @brief  Reads a byte from a specific Camera register 
  * @param  Device: OV9655 write address.
  * @param  Addr: OV9655 register address. 
  * @retval data read from the specific register or 0xFF if timeout condition 
  *         occured. 
  */
uint8_t DCMI_SingleRandomRead(uint8_t Device, uint16_t Addr)
{
  uint32_t timeout = DCMI_TIMEOUT_MAX;
  uint8_t Data = 0;

  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Send DCMI selcted device slave Address for write */
  I2C_Send7bitAddress(I2C1, Device, I2C_Direction_Transmitter);
 
  /* Test on I2C1 EV6 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  } 

  /* Send I2C1 location address LSB */
  I2C_SendData(I2C1, (uint8_t)(Addr));

  /* Test on I2C1 EV8 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Clear AF flag if arised */
  I2C1->SR1 |= (uint16_t)0x0400; 

  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on I2C1 EV6 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Send DCMI selcted device slave Address for write */
  I2C_Send7bitAddress(I2C1, Device, I2C_Direction_Receiver);
   
  /* Test on I2C1 EV6 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }  
 
  /* Prepare an NACK for the next data received */
  I2C_AcknowledgeConfig(I2C1, DISABLE);

  /* Test on I2C1 EV7 and clear it */
  timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }   
    
  /* Prepare Stop after receiving data */
  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Receive the Data */
  Data = I2C_ReceiveData(I2C1);

  /* return the read data */
  return Data;
}

void tw9910_mask_set(u8 chipaddress, u8 command,
			   u8 mask, u8 set)
{
	uint8_t val;
	val=DCMI_SingleRandomRead( chipaddress|0x01, command);
	val &= ~mask;
	val |= set & mask;
    DCMI_SingleRandomWrite( chipaddress, command,val);
	//I2C_writeByte(client->I2Cx,client->chipAdd,command, val);
}

/**
  * @}
  */ 

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
