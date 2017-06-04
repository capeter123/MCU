#include "SDIO_Test.h"
#include "stm32f4xx_bsp.h"
#include "stm324xg_eval_sdio_sd.h"
#include <stdio.h>

extern SD_CardInfo SDCardInfo;
/* Private variables ---------------------------------------------------------*/
uint8_t aBuffer_Block_Tx[SD_BLOCK_SIZE]; 
uint8_t aBuffer_Block_Rx[SD_BLOCK_SIZE];
uint8_t aBuffer_MultiBlock_Tx[SD_MULTI_BUF_SIZE];
uint8_t aBuffer_MultiBlock_Rx[SD_MULTI_BUF_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void Fill_Buffer(uint8_t* pBuf, uint32_t len);
static SDTestStatus BufferCmp(const uint8_t* pBuf1, const uint8_t* pBuf2, uint32_t len);
static SDTestStatus EraseCheck(const uint8_t* pBuf, uint32_t len);
static void SD_NVIC_Configuration(void);

/**
  * @brief  DeInitializes the SDIO interface.
  * @param  None
  * @retval None
  */
void SD_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /*!< Disable SDIO Clock */
  SDIO_ClockCmd(DISABLE);
  
  /*!< Set Power State to OFF */
  SDIO_SetPowerState(SDIO_PowerState_OFF);

  /*!< DeInitializes the SDIO peripheral */
  SDIO_DeInit();
  
  /* Disable the SDIO APB2 Clock */
  RCC_APB2PeriphClockCmd(SD_SDIO_RCC_CLK, DISABLE);

  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_D0_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_D1_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_D2_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_D3_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_CLK_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_CMD_GPIO_PORT, SD_SDIO_CMD_AF_SOURCE, SD_SDIO_GPIO_AF);

  /* Configure PC.08, PC.09, PC.10, PC.11 pins: D0, D1, D2, D3 pins */
  GPIO_InitStructure.GPIO_Pin = SD_SDIO_D0_PIN | SD_SDIO_D1_PIN | SD_SDIO_D2_PIN | SD_SDIO_D3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SD_SDIO_DAT_GPIO_PORT, &GPIO_InitStructure);

  /* Configure PD.02 CMD line */
  GPIO_InitStructure.GPIO_Pin = SD_SDIO_CMD_PIN;
  GPIO_Init(SD_SDIO_CMD_GPIO_PORT, &GPIO_InitStructure);

  /* Configure PC.12 pin: CLK pin */
  GPIO_InitStructure.GPIO_Pin = SD_SDIO_CLK_PIN;
  GPIO_Init(SD_SDIO_DAT_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for 
  *         data transfer).
  * @param  None
  * @retval None
  */
void SD_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* GPIOC and GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(SD_SDIO_DAT_GPIO_CLK | SD_SDIO_CMD_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_D0_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_D1_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_D2_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_D3_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_DAT_GPIO_PORT, SD_SDIO_CLK_AF_SOURCE, SD_SDIO_GPIO_AF);
  GPIO_PinAFConfig(SD_SDIO_CMD_GPIO_PORT, SD_SDIO_CMD_AF_SOURCE, SD_SDIO_GPIO_AF);

  /* Configure PC.08, PC.09, PC.10, PC.11 pins: D0, D1, D2, D3 pins */
  GPIO_InitStructure.GPIO_Pin = SD_SDIO_D0_PIN | SD_SDIO_D1_PIN | SD_SDIO_D2_PIN | SD_SDIO_D3_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(SD_SDIO_DAT_GPIO_PORT, &GPIO_InitStructure);

  /* Configure PD.02 CMD line */
  GPIO_InitStructure.GPIO_Pin = SD_SDIO_CMD_PIN;
  GPIO_Init(SD_SDIO_CMD_GPIO_PORT, &GPIO_InitStructure);

  /* Configure PC.12 pin: CLK pin */
  GPIO_InitStructure.GPIO_Pin = SD_SDIO_CLK_PIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SD_SDIO_DAT_GPIO_PORT, &GPIO_InitStructure);
  
  /*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
  //GPIO_InitStructure.GPIO_Pin = SD_DETECT_PIN;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStructure);

  /* Enable the SDIO APB2 Clock */
  RCC_APB2PeriphClockCmd(SD_SDIO_RCC_CLK, ENABLE);

  /* Enable the DMA2 Clock */
  RCC_AHB1PeriphClockCmd(SD_SDIO_DMA_CLK, ENABLE);
	
	/* Enable NVIC for DMA2 and SDIO */
#ifdef SD_DMA_MODE
	SD_NVIC_Configuration();
#endif
}

/**
  * @brief  Configures the DMA2 Channel4 for SDIO Tx request.
  * @param  BufferSRC: pointer to the source buffer
  * @param  BufferSize: buffer size
  * @retval None
  */
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
{
  DMA_InitTypeDef SDDMA_InitStructure;

  DMA_ClearFlag(SD_SDIO_DMA_STREAM, SD_SDIO_DMA_FLAG_FEIF | SD_SDIO_DMA_FLAG_DMEIF | SD_SDIO_DMA_FLAG_TEIF | SD_SDIO_DMA_FLAG_HTIF | SD_SDIO_DMA_FLAG_TCIF);

  /* DMA2 Stream3  or Stream6 disable */
  DMA_Cmd(SD_SDIO_DMA_STREAM, DISABLE);

  /* DMA2 Stream3  or Stream6 Config */
  DMA_DeInit(SD_SDIO_DMA_STREAM);

  SDDMA_InitStructure.DMA_Channel = SD_SDIO_DMA_CHANNEL;
  SDDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
  SDDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)BufferSRC;
  SDDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  SDDMA_InitStructure.DMA_BufferSize = BufferSize;
  SDDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  SDDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  SDDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  SDDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  SDDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  SDDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  SDDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  SDDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  SDDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
  SDDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
  DMA_Init(SD_SDIO_DMA_STREAM, &SDDMA_InitStructure);
  DMA_ITConfig(SD_SDIO_DMA_STREAM, DMA_IT_TC, ENABLE);
  DMA_FlowControllerConfig(SD_SDIO_DMA_STREAM, DMA_FlowCtrl_Peripheral);

  /* DMA2 Stream3  or Stream6 enable */
  DMA_Cmd(SD_SDIO_DMA_STREAM, ENABLE);
    
}

/**
  * @brief  Configures the DMA2 Channel4 for SDIO Rx request.
  * @param  BufferDST: pointer to the destination buffer
  * @param  BufferSize: buffer size
  * @retval None
  */
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
{
  DMA_InitTypeDef SDDMA_InitStructure;

  DMA_ClearFlag(SD_SDIO_DMA_STREAM, SD_SDIO_DMA_FLAG_FEIF | SD_SDIO_DMA_FLAG_DMEIF | SD_SDIO_DMA_FLAG_TEIF | SD_SDIO_DMA_FLAG_HTIF | SD_SDIO_DMA_FLAG_TCIF);

  /* DMA2 Stream3  or Stream6 disable */
  DMA_Cmd(SD_SDIO_DMA_STREAM, DISABLE);

  /* DMA2 Stream3 or Stream6 Config */
  DMA_DeInit(SD_SDIO_DMA_STREAM);

  SDDMA_InitStructure.DMA_Channel = SD_SDIO_DMA_CHANNEL;
  SDDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
  SDDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)BufferDST;
  SDDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  SDDMA_InitStructure.DMA_BufferSize = BufferSize;
  SDDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  SDDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  SDDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  SDDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  SDDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  SDDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  SDDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  SDDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  SDDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
  SDDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
  DMA_Init(SD_SDIO_DMA_STREAM, &SDDMA_InitStructure);
  DMA_ITConfig(SD_SDIO_DMA_STREAM, DMA_IT_TC, ENABLE);
  DMA_FlowControllerConfig(SD_SDIO_DMA_STREAM, DMA_FlowCtrl_Peripheral);

  /* DMA2 Stream3 or Stream6 enable */
  DMA_Cmd(SD_SDIO_DMA_STREAM, ENABLE);
}


SDTestStatus SD_TestAll(void)
{
	SDTestStatus rv = SD_TEST_PASSED;
	SDOperationType sdOpStep= SD_OP_ERASE;
		
	if(SD_Init()!=SD_OK)
	{
		printf("SD_Init Failed\r\n");
		return SD_TEST_FAILED;
	}
	
	printf("\r\n CardType is: %d ", SDCardInfo.CardType);
	printf("\r\n CardCapacity is: %lld ", SDCardInfo.CardCapacity);
	printf("\r\n CardBlocksize is: %d ", SDCardInfo.CardBlockSize);
	printf("\r\n RCA is: %d ", SDCardInfo.RCA);
	printf("\r\n Manufactuturer ID is: %d \r\n", SDCardInfo.SD_cid.ManufacturerID);
	
	while((rv == SD_TEST_PASSED) && (sdOpStep != SD_OP_END)
		&& (SD_Detect() == SD_PRESENT))
	{
		switch(sdOpStep)
		{
			case SD_OP_ERASE:
				rv = SD_EraseTest();
				sdOpStep = SD_OP_BLOCK;
				break;
			case SD_OP_BLOCK:
				rv = SD_SingleBlockTest();
				sdOpStep = SD_OP_MULTI_BLOCK;
				break;
			case SD_OP_MULTI_BLOCK:
				rv = SD_MultiBlockTest();
				sdOpStep = SD_OP_END;
				break;
			default:
				break;
		}
	}
	
	if(rv == SD_TEST_PASSED)
		printf(">>>Congratulations, SD Test finished!!!\r\n");

	return rv;
}

SDTestStatus SD_EraseTest(void)
{
	SDTestStatus rv = SD_TEST_FAILED;
	SD_Error SDStatus;
	/* Erase NumberOfBlocks Blocks of 512Byte. */
	SDStatus = SD_Erase(0x0, (SD_BLOCK_SIZE * SD_BLOCK_NUM));
	if(SD_OK == SDStatus)
	{
		SDStatus = SD_ReadMultiBlocks(aBuffer_MultiBlock_Rx, 0x00, 
									SD_BLOCK_SIZE, SD_BLOCK_NUM);
		SDStatus = SD_WaitReadOperation();
		while(SD_GetStatus()!=SD_TRANSFER_OK);
	}
	
	/* Check the correctness of erased blocks.*/
	if(SD_OK == SDStatus)
	{
		rv = EraseCheck(aBuffer_MultiBlock_Rx, SD_MULTI_BUF_SIZE);
	}
	
	if(SD_TEST_PASSED != rv)
		printf("Erase Test Failed!\r\n");
	
	return rv;
}

SDTestStatus SD_SingleBlockTest(void)
{
	SDTestStatus rv = SD_TEST_FAILED;
	SD_Error SDStatus;
	/* Fill the write buffer. */
	Fill_Buffer(aBuffer_Block_Tx, SD_BLOCK_SIZE);
	/* Write block of 512 bytes on address 0x00 to SD Memory.*/
	SDStatus = SD_WriteBlock(aBuffer_Block_Tx, 0x0, SD_BLOCK_SIZE);
	SDStatus = SD_WaitWriteOperation();
	while(SD_GetStatus() != SD_TRANSFER_OK);
	
	if(SD_OK == SDStatus)
	{
		SDStatus = SD_ReadBlock(aBuffer_Block_Rx, 0x00, SD_BLOCK_SIZE);
		SDStatus = SD_WaitReadOperation();
		while(SD_GetStatus()!=SD_TRANSFER_OK);
	}
	
	/* Check the correctness of writen data. */
	if(SD_OK == SDStatus)
	{
		rv = BufferCmp(aBuffer_Block_Tx, aBuffer_Block_Rx, SD_BLOCK_SIZE);
	}
	
	if(SD_TEST_PASSED != rv)
		printf("Single Block R/W Test Failed!\r\n");
	
	return rv;
}

SDTestStatus SD_MultiBlockTest(void)
{
	SDTestStatus rv = SD_TEST_FAILED;
	SD_Error SDStatus;
	Fill_Buffer(aBuffer_MultiBlock_Tx, SD_MULTI_BUF_SIZE);
	SDStatus = SD_WriteMultiBlocks(aBuffer_MultiBlock_Tx, 0x0, SD_BLOCK_SIZE, SD_BLOCK_NUM);
	SDStatus = SD_WaitWriteOperation();
	while(SD_GetStatus() != SD_TRANSFER_OK);
	
	if(SD_OK == SDStatus)
	{
		SDStatus = SD_ReadMultiBlocks(aBuffer_MultiBlock_Rx, 0x0, SD_BLOCK_SIZE, SD_BLOCK_NUM);
		SDStatus = SD_WaitReadOperation();
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	
	if(SDStatus == SD_OK)
	{
		rv = BufferCmp(aBuffer_MultiBlock_Rx, aBuffer_MultiBlock_Tx, SD_MULTI_BUF_SIZE);
	}
	
	if(SD_TEST_FAILED == rv)
		printf("MultiBlock R/W Test Failed!\r\n");

	return rv;
}

static void Fill_Buffer(uint8_t* pBuf, uint32_t len)
{
	uint32_t i;
	for(i=0; i<len; ++i)
	{
		pBuf[i] = (uint8_t)i;
	}
}

static SDTestStatus BufferCmp(const uint8_t* pBuf1, const uint8_t* pBuf2, uint32_t len)
{
	uint8_t res = 0;
	for(; len>0; ++pBuf1, ++pBuf2, len--)
	{
		if((res = *pBuf1 - *pBuf2)!=0)
			break;
	}
	
	return (0==res)?SD_TEST_PASSED:SD_TEST_FAILED;
}

static SDTestStatus EraseCheck(const uint8_t* pBuf, uint32_t len)
{
	while(len--)
	{
		/* In some SD Cards the erased state is 0xFF, in others it's 0x00 */
		if((*pBuf!=0xFF) && (*pBuf!=0x00))
			return SD_TEST_FAILED;
		
		pBuf++;
	}
	
	return SD_TEST_PASSED;
}

static void SD_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_Init(&NVIC_InitStructure);
}
