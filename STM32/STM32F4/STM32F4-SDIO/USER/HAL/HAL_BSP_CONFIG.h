#ifndef _STM32F4XX_BSP_H_
#define _STM32F4XX_BSP_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx.h"
	
/**
 * @brief Definition for LEDs
 */ 
#define LEDn						(2)
#define LEDs_RCC_CLK		RCC_AHB1Periph_GPIOF
#define LEDs_PORT				GPIOF
	
#define LED1_PIN				GPIO_Pin_9
#define LED1_GPIO_PORT	GPIOF
#define LED1_GPIO_CLK		RCC_AHB1Periph_GPIOF

#define LED2_PIN				GPIO_Pin_10
#define LED2_GPIO_PORT	GPIOF
#define LED2_GPIO_CLK		RCC_AHB1Periph_GPIOF
	
/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define DEBUG_COM1_BAUD									(115200U)
#define DEBUG_COM1                      USART1
#define DEBUG_COM1_CLK                  RCC_APB2Periph_USART1
#define DEBUG_COM1_GPIO_CLK							RCC_AHB1Periph_GPIOA
#define DEBUG_COM1_PORT									GPIOA

#define DEBUG_COM1_TX_PIN               GPIO_Pin_9
#define DEBUG_COM1_TX_GPIO_PORT         GPIOA
#define DEBUG_COM1_TX_GPIO_CLK         	RCC_AHB1Periph_GPIOA
#define DEBUG_COM1_TX_SOURCE            GPIO_PinSource9
#define DEBUG_COM1_TX_AF                GPIO_AF_USART1

#define DEBUG_COM1_RX_PIN               GPIO_Pin_10
#define DEBUG_COM1_RX_GPIO_PORT         GPIOA
#define DEBUG_COM1_RX_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define DEBUG_COM1_RX_SOURCE            GPIO_PinSource10
#define DEBUG_COM1_RX_AF                GPIO_AF_USART1

#define DEBUG_COM1_IRQn                 USART1_IRQn

#define USART1_DR_BASE  								(USART1_BASE + 0x04)
/* DMA Stream parameters definitions. You can modify these parameters to select
   a different DMA Stream and/or channel.
   But note that only DMA2 Streams are capable of Memory to Memory transfers. */
#define USART1_DMA_STREAM               DMA2_Stream7
#define USART1_DMA_CHANNEL              DMA_Channel_4
#define USART1_DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 
#define USART1_DMA_STREAM_IRQ           DMA2_Stream7_IRQn
#define USART1_DMA_IT_TCIF              DMA_IT_TC
#define USART1_DMA_STREAM_IRQHANDLER    DMA2_Stream7_IRQHandler

/**
  * @brief  SD FLASH SDIO Interface
  */
#define SD_SDIO_RCC_CLK									RCC_APB2Periph_SDIO

#define SD_SDIO_DAT_GPIO_CLK						RCC_AHB1Periph_GPIOC
#define SD_SDIO_CMD_GPIO_CLK						RCC_AHB1Periph_GPIOD
#define SD_SDIO_GPIO_AF									GPIO_AF_SDIO
#define SD_SDIO_DAT_GPIO_PORT						GPIOC
#define SD_SDIO_CMD_GPIO_PORT						GPIOD

#define SD_SDIO_D0_AF_SOURCE						GPIO_PinSource8
#define SD_SDIO_D1_AF_SOURCE						GPIO_PinSource9
#define SD_SDIO_D2_AF_SOURCE						GPIO_PinSource10
#define SD_SDIO_D3_AF_SOURCE						GPIO_PinSource11
#define SD_SDIO_CLK_AF_SOURCE						GPIO_PinSource12
#define SD_SDIO_CMD_AF_SOURCE						GPIO_PinSource2
#define SD_SDIO_D0_PIN									GPIO_Pin_8
#define SD_SDIO_D1_PIN									GPIO_Pin_9
#define SD_SDIO_D2_PIN									GPIO_Pin_10
#define SD_SDIO_D3_PIN									GPIO_Pin_11
#define SD_SDIO_CMD_PIN									GPIO_Pin_2
#define SD_SDIO_CLK_PIN									GPIO_Pin_12


#define SD_DETECT_PIN                   GPIO_Pin_13                 /* PH.13 */
#define SD_DETECT_GPIO_PORT             GPIOH                       /* GPIOH */
#define SD_DETECT_GPIO_CLK              RCC_AHB1Periph_GPIOH
   
#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40012C80)
/** 
  * @brief  SDIO Intialization Frequency (400KHz max)
  */
#define SDIO_INIT_CLK_DIV                ((uint8_t)0x76)
/** 
  * @brief  SDIO Data Transfer Frequency (25MHz max) 
  */
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x0) 

#define SD_SDIO_DMA                   DMA2
#define SD_SDIO_DMA_CLK               RCC_AHB1Periph_DMA2
 
#define SD_SDIO_DMA_STREAM3	          	3
//#define SD_SDIO_DMA_STREAM6           6

#ifdef SD_SDIO_DMA_STREAM3
 #define SD_SDIO_DMA_STREAM            DMA2_Stream3
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF3
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF3
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF3
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF3
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF3 
 #define SD_SDIO_DMA_IRQn              DMA2_Stream3_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream3_IRQHandler 
#elif defined SD_SDIO_DMA_STREAM6
 #define SD_SDIO_DMA_STREAM            DMA2_Stream6
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF6
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF6
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF6
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF6
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF6 
 #define SD_SDIO_DMA_IRQn              DMA2_Stream6_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream6_IRQHandler
#endif /* SD_SDIO_DMA_STREAM3 */

void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void); 
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize);

#ifdef __cplusplus
}
#endif
#endif
