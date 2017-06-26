 /**
  ******************************************************************************
  * @file    stm32l_discovery_spi.c
  * @author  jiangst.fnst@cn.fujitsu.com
  * @version 
  * @date    Feb-2014
  * @brief   This file includes driver for the SPI Communication Module mounted on 
  *          STM32l discovery board MB963
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 FNST</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l_discovery_spi.h"

__IO uint8_t SPIx_Data = CMD_ACK;
__IO uint32_t SPITimeOut = 0x0;
DMA_InitTypeDef DMA_InitStructure;
SPI_RxTx_DataInfo SPIRxTxDataInfo;
//extern _Bool gFlag_SPIRequest;;

static void SPIx_RCC_Config(void);
static void SPIx_NVIC_Config(void);
static void SPIx_GPIO_Config(void);
static void SPIx_Mode_Config(uint16_t SPIx_Mode, _Bool bISRMode);
static void SPIx_DMA_BasicConfig(void);
static void TimeOut_UserCallback(void);

void SPIx_InitConfig(uint16_t SPIxMode)
{
	SPIx_RCC_Config();
	SPIx_NVIC_Config();
	SPIx_GPIO_Config();
	SPIx_Mode_Config(SPIxMode, 1); // bISRMode = 0 that disable the SPI Tx/Rx Interrupt mode.
	SPIx_DMA_BasicConfig();
}

static void SPIx_RCC_Config(void)
{
	/* Enable SPIx, SCK, MOSI, MISO NSS and GPIO clocks */
	RCC_APB2PeriphClockCmd(SPIx_CLK, ENABLE);
	RCC_AHBPeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK | SPIx_NSS_GPIO_CLK, ENABLE);
	/* Enable the DMA peripheral */
  RCC_AHBPeriphClockCmd(DMAx_CLK, ENABLE);
}

static void SPIx_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPIx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
	
static void SPIx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
	GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
	GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
	GPIO_PinAFConfig(SPIx_NSS_GPIO_PORT, SPIx_NSS_SOURCE, SPIx_NSS_AF);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	
	 /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_MISO_PIN;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	/* SPI NSS pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_NSS_PIN;
  GPIO_Init(SPIx_NSS_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
static void SPIx_Mode_Config(uint16_t SPIx_Mode, _Bool bISRMode)
{
	SPI_InitTypeDef SPI_InitStructure;
	SPI_I2S_DeInit(SPIx);
	/*SPI InitStructure Initialized*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // Fpclk = 32MHz, so spi_baud rate is 1MHz and not effect in slave mode.
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_InitStructure.SPI_Mode = SPIx_Mode;
	if(SPIx_Mode == SPI_Mode_Master)
		SPI_SSOutputCmd(SPIx, ENABLE);
	
	SPI_Init(SPIx, &SPI_InitStructure);	
	
	if(bISRMode)
	{
		/* Enable the Rx buffer not empty interrupt */
		SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, ENABLE);
		/* Enable the SPI Error interrupt */
		SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_ERR, ENABLE);
		/* Enable the Tx buffer empty interrupt */
		SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
		
		SPI_Cmd(SPIx, ENABLE);
	}
}

static void SPIx_DMA_BasicConfig(void)
{
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
}

void SPIx_DMA_RWData(uint32_t* pSendBuf, uint32_t* pRecvBuf)
{
	//SPIx_DMA_BasicConfig();
	/*First, Disable the SPI SPE*/
	//SPI_Cmd(SPIx, DISABLE);
	
	/* DMA channel Tx of SPI Configuration */
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pSendBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(SPIx_TX_DMA_CHANNEL, &DMA_InitStructure);
	
	/* DMA channel Rx of SPI Configuration */
	DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pRecvBuf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_Init(SPIx_RX_DMA_CHANNEL, &DMA_InitStructure);
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
	
	SPI_Cmd(SPIx, ENABLE);
	/* Enable the DMA channels */
  DMA_Cmd(SPIx_RX_DMA_CHANNEL, ENABLE);
  DMA_Cmd(SPIx_TX_DMA_CHANNEL, ENABLE);
	/* Wait the SPI DMA transfers complete or time out */
  SPITimeOut = USER_TIMEOUT;
  while ((DMA_GetFlagStatus(SPIx_RX_DMA_FLAG_TC) == RESET)&&(SPITimeOut != 0x00)) ;
	  
	SPITimeOut = USER_TIMEOUT;

  while ((DMA_GetFlagStatus(SPIx_TX_DMA_FLAG_TC) == RESET)&&(SPITimeOut != 0x00));
  if(SPITimeOut == 0)
  {
		TimeOut_UserCallback();
  }  
    
  /* The BSY flag can be monitored to ensure that the SPI communication is complete.
       This is required to avoid corrupting the last transmission before disabling 
       the SPI or entering the Stop mode. The software must first wait until TXE=1
       and then until BSY=0.*/
  SPITimeOut = USER_TIMEOUT;
  while ((SPI_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)&&(SPITimeOut != 0x00));
	
  if(SPITimeOut == 0)
  {
    TimeOut_UserCallback();
  }
    
  SPITimeOut = USER_TIMEOUT;
  while ((SPI_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)&&(SPITimeOut != 0x00));
   
  if(SPITimeOut == 0)
  {
    TimeOut_UserCallback();
  } 
     
  /* Clear DMA1 global flags */
  DMA_ClearFlag(SPIx_TX_DMA_FLAG_GL);
  DMA_ClearFlag(SPIx_RX_DMA_FLAG_GL);
   
  /* Disable the DMA channels */
  DMA_Cmd(SPIx_RX_DMA_CHANNEL, DISABLE);
  DMA_Cmd(SPIx_TX_DMA_CHANNEL, DISABLE);
    
  /* Disable the SPI peripheral */
  SPI_Cmd(SPIx, DISABLE);
   
  /* Disable the SPI Rx and Tx DMA requests */
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
}
/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
static void TimeOut_UserCallback(void)
{
  /* User can add his own implementation to manage TimeOut Communication failure */
  /* Block communication and all processes */
	// TODO
//  while (1)
//  {   
//  }
}

void SPIx_IRQ_RWBytes(uint8_t* pBuf, uint8_t len, _Bool bTxMode)
{
	uint8_t actualBytesLen;
	actualBytesLen = MIN(len, SPIBUFFERSIZE);
	if(!pBuf || actualBytesLen==0x0)
		return;
	
	if(bTxMode == SET) // Tx mode.
	{
		memcpy(SPIRxTxDataInfo.TxBuffer, pBuf, sizeof(uint8_t)*actualBytesLen);
		SPIRxTxDataInfo.Tx_Idx = 0;
		SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
		//gFlag_SPIRequest = SET;
	}
	else // Rx mode.
	{
		SPIRxTxDataInfo.Rx_Idx = 0;
		//gFlag_SPIRequest = SET;
		SPITimeOut = USER_TIMEOUT;
		while(SPIRxTxDataInfo.Rx_Idx>=actualBytesLen && SPITimeOut!=0x0);
		if(SPITimeOut!=0x0)
		{
			memcpy(pBuf, SPIRxTxDataInfo.RxBuffer, actualBytesLen);
			SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, ENABLE);
		}
	}
}

void SPIx_ISR_Handler(void)
{
	/* SPI in Slave Tramitter mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_TXE) == SET)
  {
		if(SPIRxTxDataInfo.Tx_Idx < SPIBUFFERSIZE)
		{
				SPI_I2S_SendData(SPIx, SPIRxTxDataInfo.TxBuffer[SPIRxTxDataInfo.Tx_Idx++]);
		}
		else
		{
			SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
		}
  }
	
	if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_RXNE) == SET)
	{
		if(SPIRxTxDataInfo.Rx_Idx < SPIBUFFERSIZE)
		{
			SPIRxTxDataInfo.RxBuffer[SPIRxTxDataInfo.Rx_Idx++] = SPI_I2S_ReceiveData(SPIx);
		}
		else
		{
			SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, DISABLE);
		}
	}
	
	if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR) == SET)
	{
		SPI_I2S_ReceiveData(SPIx);
    SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR);
	}
}
/******************* (C) COPYRIGHT 2014 FNST *****END OF FILE****/
