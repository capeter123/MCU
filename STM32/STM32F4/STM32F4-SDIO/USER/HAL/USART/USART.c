#include "USART.h"
#include "HAL_BSP_CONFIG.h"
#include "stdio.h"

uint8_t DebugBuff[USART1_BUFFER_SIZE];

void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_AHB1PeriphClockCmd(DEBUG_COM1_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(DEBUG_COM1_CLK, ENABLE);
	
	GPIO_PinAFConfig(DEBUG_COM1_RX_GPIO_PORT, GPIO_PinSource10, DEBUG_COM1_RX_AF);
	GPIO_PinAFConfig(DEBUG_COM1_TX_GPIO_PORT, GPIO_PinSource9, DEBUG_COM1_TX_AF);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_COM1_TX_PIN | DEBUG_COM1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DEBUG_COM1_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = DEBUG_COM1_BAUD;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(DEBUG_COM1, &USART_InitStructure);
	USART_Cmd(DEBUG_COM1, ENABLE);
}

void USART1_DMA_Config(void)
{
	__IO uint32_t Timeout = TIMEOUT_MAX;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(USART1_DMA_STREAM_CLOCK, ENABLE);
	/* Reset DMA Stream registers (for debug purpose) */
  DMA_DeInit(USART1_DMA_STREAM);
	/* Check if the DMA Stream is disabled before enabling it.
     Note that this step is useful when the same Stream is used multiple times:
     enabled, then disabled then re-enabled... In this case, the DMA Stream disable
     will be effective only at the end of the ongoing data transfer and it will 
     not be possible to re-configure it before making sure that the Enable bit 
     has been cleared by hardware. If the Stream is used only once, this step might 
     be bypassed. */
  while (DMA_GetCmdStatus(USART1_DMA_STREAM) != DISABLE)
  {
  }
	
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DebugBuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = USART1_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	
	DMA_Init(USART1_DMA_STREAM, &DMA_InitStructure);
	DMA_ITConfig(USART1_DMA_STREAM, DMA_IT_TC, ENABLE);
	DMA_Cmd(USART1_DMA_STREAM, ENABLE);
	
	/* Check if the DMA Stream has been effectively enabled.
     The DMA Stream Enable bit is cleared immediately by hardware if there is an 
     error in the configuration parameters and the transfer is no started (ie. when
     wrong FIFO threshold is configured ...) */
  Timeout = TIMEOUT_MAX;
  while ((DMA_GetCmdStatus(USART1_DMA_STREAM) != ENABLE) && (Timeout-- > 0))
  {
  }
   
  /* Check if a timeout condition occurred */
  if (Timeout == 0)
  {
    /* Manage the error: to simplify the code enter an infinite loop */
    while (1)
    {
    }
  }

  /* Enable the DMA Stream IRQ Channel */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_DMA_STREAM_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

int fputc(int ch, FILE* f)
{
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}
