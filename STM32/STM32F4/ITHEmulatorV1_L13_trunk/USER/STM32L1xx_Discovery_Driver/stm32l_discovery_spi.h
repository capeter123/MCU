 /**
  ******************************************************************************
  * @file    stm32l_discovery_spi.h
  * @author  jiangst.fnst@cn.fujitsu.com
  * @version 
  * @date    Feb-2014
  * @brief   This file contains all the functions prototypes for the spi communication
  *          firmware driver.
  ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32l_discovery_spi_h
#define __stm32l_discovery_spi_h

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stm32l1xx.h>   
#include <string.h>
	 
/* Exported typedef -----------------------------------------------------------*/
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)
#define SPIBUFFERSIZE 1
	 
typedef struct SPI_RxTx_DataInfo
{
	uint8_t Rx_Idx;
	uint8_t Tx_Idx;
	uint8_t RxBuffer[SPIBUFFERSIZE];
	uint8_t TxBuffer[SPIBUFFERSIZE];
}SPI_RxTx_DataInfo;
	 
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* USER_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the USART communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, baudrate, CPU
   frequency...). */ 
#define USER_TIMEOUT                    ((uint32_t)0x3E8) /* Waiting 1s */
/* ACK Commands */
#define CMD_ACK                          0x66 
/* DMA Communication boards Interface */
#define SPIx_DR_ADDRESS               0x4001300C
#define DMAx_CLK                    			 RCC_AHBPeriph_DMA1
#define SPIx_TX_DMA_CHANNEL       DMA1_Channel3
#define SPIx_TX_DMA_FLAG_TC        DMA1_FLAG_TC3
#define SPIx_TX_DMA_FLAG_GL        DMA1_FLAG_GL3
#define SPIx_RX_DMA_CHANNEL       DMA1_Channel2
#define SPIx_RX_DMA_FLAG_TC        DMA1_FLAG_TC2
#define SPIx_RX_DMA_FLAG_GL        DMA1_FLAG_GL2

/* SPIx Communication boards Interface */
#define SPIx                           		SPI1
#define SPIx_CLK                       RCC_APB2Periph_SPI1
#define SPIx_IRQn                      SPI1_IRQn
#define SPIx_IRQHANDLER          SPI1_IRQHandler

#define SPIx_NSS_PIN             			GPIO_Pin_4
#define SPIx_NSS_GPIO_PORT       	GPIOA
#define SPIx_NSS_GPIO_CLK    			RCC_AHBPeriph_GPIOA
#define SPIx_NSS_SOURCE       			GPIO_PinSource4
#define SPIx_NSS_AF                  	GPIO_AF_SPI1

#define SPIx_SCK_PIN                   GPIO_Pin_5                
#define SPIx_SCK_GPIO_PORT     		GPIOA                       
#define SPIx_SCK_GPIO_CLK          RCC_AHBPeriph_GPIOA
#define SPIx_SCK_SOURCE         		GPIO_PinSource5
#define SPIx_SCK_AF                  	GPIO_AF_SPI1

#define SPIx_MISO_PIN                  GPIO_Pin_11                
#define SPIx_MISO_GPIO_PORT       GPIOA                    
#define SPIx_MISO_GPIO_CLK        	 RCC_AHBPeriph_GPIOA
#define SPIx_MISO_SOURCE           GPIO_PinSource11
#define SPIx_MISO_AF                   GPIO_AF_SPI1

#define SPIx_MOSI_PIN                  GPIO_Pin_12           
#define SPIx_MOSI_GPIO_PORT       GPIOA                    
#define SPIx_MOSI_GPIO_CLK         RCC_AHBPeriph_GPIOA
#define SPIx_MOSI_SOURCE           GPIO_PinSource12
#define SPIx_MOSI_AF                   GPIO_AF_SPI1

/****************Test on the same board***********************/
/* SPIy Communication boards Interface */
////#define SPIy                           		SPI2
////#define SPIy_CLK                       RCC_APB1Periph_SPI2
////#define SPIy_IRQn                      SPI2_IRQn
////#define SPIy_IRQHANDLER          SPI2_IRQHandler

////#define SPIy_NSS_PIN             			GPIO_Pin_12
////#define SPIy_NSS_GPIO_PORT       	GPIOB
////#define SPIy_NSS_GPIO_CLK    			RCC_AHBPeriph_GPIOB
////#define SPIy_NSS_SOURCE       			GPIO_PinSource12
////#define SPIy_NSS_AF                  	GPIO_AF_SPI2

////#define SPIy_SCK_PIN                   GPIO_Pin_13                
////#define SPIy_SCK_GPIO_PORT     		GPIOB                       
////#define SPIy_SCK_GPIO_CLK          RCC_AHBPeriph_GPIOB
////#define SPIy_SCK_SOURCE         		GPIO_PinSource13
////#define SPIy_SCK_AF                  	GPIO_AF_SPI2

////#define SPIy_MISO_PIN                  GPIO_Pin_14               
////#define SPIy_MISO_GPIO_PORT       GPIOB                    
////#define SPIy_MISO_GPIO_CLK        	 RCC_AHBPeriph_GPIOB
////#define SPIy_MISO_SOURCE           GPIO_PinSource14
////#define SPIy_MISO_AF                   GPIO_AF_SPI2

////#define SPIy_MOSI_PIN                  GPIO_Pin_15           
////#define SPIy_MOSI_GPIO_PORT       GPIOB                    
////#define SPIy_MOSI_GPIO_CLK         RCC_AHBPeriph_GPIOB
////#define SPIy_MOSI_SOURCE           GPIO_PinSource15
////#define SPIy_MOSI_AF                   GPIO_AF_SPI2

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SPIx_InitConfig(uint16_t SPIxMode);
void SPIx_DMA_RWData(uint32_t* pSendBuf, uint32_t* pRecvBuf);
void SPIx_IRQ_RWBytes(uint8_t* pBuf, uint8_t len, _Bool bTxMode);
void SPIx_ISR_Handler(void);

#ifdef __cplusplus
}
#endif

#endif //__stm32l_discovery_spi_h

/******************* (C) COPYRIGHT 2014 FNST *****END OF FILE****/
