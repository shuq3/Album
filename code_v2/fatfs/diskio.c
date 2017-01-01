/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#include "../sdcard/sdcard.h"
#include "../led.h"

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* 磁盘号 */
)
{
	switch (pdrv)
	{
		case 0: 
		{
			if(SDMMC0_MOV_Flag) //卡移除了
				return  RES_NOTRDY;
			else
				return  RES_OK;
		}
	}
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* 磁盘号:0 */
)
{
	switch (pdrv)
	{
		case 0 : // SD
		{
			if(SD_Init() != SD_OK)
				return RES_ERROR;
			else
			{	
				SDMMC0_MOV_Flag = 0;	//由于卡已经初始化成功了,因此去除标志
				return RES_OK;
			}
		}
	}
	return RES_ERROR;
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
	SD_Error Status = SD_OK;
	switch (pdrv)
	{
		case 0 :
		{
			led_off();
			if(count > 1)
			{
				led_on();
				Status = SD_ReadMultiBlocks(sector,(u32 *)buff,count);
				led_off();
			}
			else
			{
				led1_on();
				Status = SD_ReadBlock(sector,(u32 *)buff);
				led1_off();
			}
			if(Status != SD_OK)	{
				return RES_ERROR;		
			}
		}break;
		default : return RES_ERROR;
	}
	return RES_OK;
}
