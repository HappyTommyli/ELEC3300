/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
 #include "stdio.h"
 #include "sdio.h"
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */
uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  FRESULT   fre;
  char test1[64];
	fre  = f_mount(&SDFatFS, (TCHAR const*)SDPath,1);
	if(fre == FR_OK)
	{
		 
		ILI9341_DispString_EN(0,0,"SD card  mount OK \r\n");
		sprintf(test1,"SD cara init OK  size %.1f M\r\n",(float)hsd.SdCard.BlockNbr * hsd.SdCard.BlockSize / 1024 /1024);
    	ILI9341_DispString_EN(0,10,test1);
	   
		 fre = f_open(&SDFile,"test.txt",FA_READ); 
		 if(fre == FR_OK)
		 {
		    char buf[64];
			  uint32_t  br;
			  if( f_read(&SDFile,buf,63,&br) == FR_OK)
				{
				  sprintf(test1,"read test.txt size %d \r\n",br);
          			ILI9341_DispString_EN(0,20,test1);
					buf[br] = '\0'; 
					ILI9341_DispString_EN(0,30,buf);
				}
				f_close(&SDFile);
		 }
		 else
		 {
		    sprintf(test1,"open test.txt fail %d \r\n",fre);
        	ILI9341_DispString_EN(0,40,test1);
		 }
		 
		 fre = f_open(&SDFile,"stm32.txt",FA_READ); 
		 if(fre != FR_OK)
		 {
		    fre = f_open(&SDFile,"stm32.txt",FA_READ); 
			  if(fre == FR_OK)
				{
				  ILI9341_DispString_EN(0,50,"create stm32.txt OK \r\n");
					uint32_t  bw;
					if( f_write(&SDFile,"MAIJI",56,&bw) == FR_OK)
					{
					  ILI9341_DispString_EN(0,60,"write stm32.txt OK \r\n");
					}
					
					f_close(&SDFile);
				}
				else
				{
				  sprintf(test1,"create stm32.txt fail %d \r\n",fre);
          			ILI9341_DispString_EN(0,70,test1);
				}
		 }
	
	}
	else
	{
    	sprintf(test1,"mount SD card fail %d \r\n",fre);
	   ILI9341_DispString_EN(0,0,test1);
	   return;
	}
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
