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
#include "main.h"
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */
#define INIT_X 10	//起始x坐标
#define INIT_Y 5	//起始y坐标
#define	PICTURE_WIDTH 64
#define PICTURE_HEIGHT	64
#define TOTAL_BYTE	PICTURE_WIDTH*PICTURE_HEIGHT	//一张图片存储的总字节数，一个字节中有8个像素
#define BUFF_TOTAL_BYTE 512
//一张图片存储的总位数，即总像素，以300*230的图片为例，像素总个数为：300*230=69000
//但是为什么软件输出的图像8740*8=69920
//原因：图像的宽度300，即300个像素点，8个像素点对应一个字节，300\8=37余4，余下的4个像素被装入了一个字节的前四位中，后四位补0
//因此：一行相当于38个字节一共230行，总字节数：38*230=8740，多出来了：230*4=920补0像素
#define TOTAL_BIT		TOTAL_BYTE*8
  


/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  // FRESULT   fre;
  // char test1[72];
	f_mount(&SDFatFS, (TCHAR const*)SDPath,1);
  // if(fre == FR_OK)
	// {		  
	// 	 fre = f_open(&SDFile,"down.bin",FA_READ); 
	// 	 if(fre == FR_OK)
	// 	 {
  //       uint32_t br;
  //       if (f_read(&SDFile, test1, sizeof(test1)-1, &br) == FR_OK) {
  //         ILI9341_DispString_EN(0,0,test1);
  //         f_close(&SDFile);
  //       }
  //       else
  //       {
  //         sprintf(test1,"open test.txt fail %d \r\n",fre);
  //         ILI9341_DispString_EN(0,40,test1);
  //       }	
  //    }
  // }
	// else
	// {
  //   	sprintf(test1,"mount SD card fail %d \r\n",fre);
	//    ILI9341_DispString_EN(0,0,test1);
	//    return;
	// }

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
