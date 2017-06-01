#ifndef _SDIO_TEST_H_
#define _SDIO_TEST_H_
#ifdef __cplusplus
 extern "C"{
#endif

#define SD_BLOCK_SIZE	(512)
#define SD_BLOCK_NUM	(10)
#define SD_MULTI_BUF_SIZE (SD_BLOCK_SIZE * SD_BLOCK_NUM)

typedef enum{
	SD_TEST_PASSED = 0,
	SD_TEST_FAILED
}SDTestStatus;

typedef enum{
	SD_OP_ERASE,
	SD_OP_BLOCK,
	SD_OP_MULTI_BLOCK,
	SD_OP_END
}SDOperationType;

SDTestStatus SD_EraseTest(void);
SDTestStatus SD_SingleBlockTest(void);
SDTestStatus SD_MultiBlockTest(void);
SDTestStatus SD_TestAll(void);

#ifdef __cplusplus
}	
#endif

#endif
