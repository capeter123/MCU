/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "hal_sdio_sd.h" /* SD card operation interfaces */
#include "sdio_test.h"
#include <string.h>

extern SD_CardInfo SDCardInfo;
/* Definitions of physical drive number for each drive */
#ifndef SD_BLOCK_SIZE
#define SD_BLOCK_SIZE	(512UL)
#endif
#define DEV_SDCARD	0
//#define DEV_RAM		1	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		2	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		3	/* Example: Map USB MSD to physical drive 2 */
BYTE sd_scratch_buf[SD_BLOCK_SIZE] __attribute__((aligned(4))) = {0};
//__align(4) BYTE sd_scratch_buf[SD_BLOCK_SIZE];
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
	//int result;

	switch (pdrv) {
	case DEV_SDCARD :
		//result = RAM_disk_status();

		// translate the reslut code here

		//return stat;
		stat = RES_OK;
		break;
	default:
		break;
	}
	
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
	SD_Error result;

	switch (pdrv) {
	case DEV_SDCARD :
		//result = RAM_disk_initialize();
		result = SD_Init();
		// translate the reslut code here
		stat = (DSTATUS)result;
		break;
	default:
		break;
	}
	
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;
	SD_Error result;

	switch (pdrv) {
	case DEV_SDCARD :
		// translate the arguments here

		//result = RAM_disk_read(buff, sector, count);
		if(((DWORD)buff&0x3) != 0)
		{
			UINT n;
			result = SD_OK;
			for(n=0; (n<count) && (SD_OK == result) ; n++)
			{
				result = SD_ReadBlock(sd_scratch_buf, (sector+n)*SD_BLOCK_SIZE, SD_BLOCK_SIZE);
				memcpy(buff, sd_scratch_buf, SD_BLOCK_SIZE);
				buff += SD_BLOCK_SIZE;
			}			
		}
		else
		{	
				if(count > 1)
						result = SD_ReadMultiBlocks(buff, sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE, count);
				else
						result = SD_ReadBlock(buff, sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE);
				
				/* Check if the SDIO data transfer is finished.*/
				SD_WaitReadOperation();
				/* Wait until end of SD card read operation.*/
				while(SD_GetStatus() != SD_TRANSFER_OK);

				// translate the reslut code here
				res = (result == SD_OK)?RES_OK:RES_ERROR;

				//return res;
		}
		
		break;
		
	default:
		break;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;
	SD_Error result;

	switch (pdrv) {
	case DEV_SDCARD :
		// translate the arguments here
		if(((DWORD)buff&0x3) != 0)
		{
			UINT n;
			result = SD_OK;
			for(n=0; (n<count) && (SD_OK == result); n++)
			{
				memcpy(sd_scratch_buf, buff, SD_BLOCK_SIZE);
				result = SD_WriteBlock(sd_scratch_buf, (sector+n)*SD_BLOCK_SIZE, SD_BLOCK_SIZE);
				buff += SD_BLOCK_SIZE;
			}
		}
		else
		{
				//result = RAM_disk_write(buff, sector, count);
				if(count>1)
						result = SD_WriteMultiBlocks((BYTE*)buff, sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE, count);
				else
						result = SD_WriteBlock((BYTE*)buff, sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE);
				
				SD_WaitWriteOperation();
				while(SD_GetStatus() != SD_TRANSFER_OK);
				// translate the reslut code here
				res = (SD_OK == result)?RES_OK:RES_ERROR;
		}
		//return res;
		break;
		
	default:
		break;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;
	//int result;
	if( DEV_SDCARD == pdrv)
	{
		switch(cmd)
		{
			case GET_BLOCK_SIZE:
				*(DWORD*)buff = SDCardInfo.CardBlockSize;;
				res = RES_OK;
				break;
			case GET_SECTOR_SIZE:
				*(DWORD*)buff = (WORD)SDCardInfo.CardBlockSize;
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT:
				*(DWORD*)buff = (DWORD)(SDCardInfo.CardCapacity >> 9);
				res = RES_OK;
				break;
			case CTRL_SYNC:
				while(SD_TRANSFER_OK != SD_GetStatus());
				res = RES_OK;
				break;
			default:
				break;
		}
	}

	return res;
}

