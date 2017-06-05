#include "sdio_test.h"
#include "HAL_BSP_CONFIG.h"
#include "hal_sdio_sd.h"
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
