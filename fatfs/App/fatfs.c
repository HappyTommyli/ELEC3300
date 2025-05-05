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
  FRESULT   fre;
  char test1[72];
	fre  = f_mount(&SDFatFS, (TCHAR const*)SDPath,1);
  if(fre == FR_OK)
	{		  
		 fre = f_open(&SDFile,"0:/lovedog.bin",FA_READ); 
		 if(fre == FR_OK)
		 {
        uint32_t br;
        if (f_read(&SDFile, test1, sizeof(test1)-1, &br) == FR_OK) {
          ILI9341_DispString_EN(0,0,test1);
          f_close(&SDFile);
        }
        else
        {
          sprintf(test1,"open test.txt fail %d \r\n",fre);
          ILI9341_DispString_EN(0,40,test1);
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
void ILI9341_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  // 设置列地址（0x2A）
  ILI9341_Write_Cmd(0x2A);
  ILI9341_Write_Data(x0 >> 8);
  ILI9341_Write_Data(x0 & 0xFF);
  ILI9341_Write_Data(x1 >> 8);
  ILI9341_Write_Data(x1 & 0xFF);

  // 设置行地址（0x2B）
  ILI9341_Write_Cmd(0x2B);
  ILI9341_Write_Data(y0 >> 8);
  ILI9341_Write_Data(y0 & 0xFF);
  ILI9341_Write_Data(y1 >> 8);
  ILI9341_Write_Data(y1 & 0xFF);

  // 进入内存写入模式（0x2C）
  ILI9341_Write_Cmd(0x2C);
}

void Draw_Point(uint16_t x,uint16_t y,uint16_t color)
{
	//  设置区域
	ILI9341_SetWindow(x,y,x+1,y+1);
	//   写入数据
	ILI9341_Write_Data(color); 
}

uint16_t  RGB888ToRGB565(uint8_t  r , uint8_t g , uint8_t b)
{
    uint16_t   color = 0;
	  color  |=  (uint16_t)(b>>3)        ;    //  0~4
	  color  |=  (uint16_t)(g>>2) << 5 ;    //  5~10
	  color  |=  (uint16_t)(r>>3) << 11;    // 11~15
	  return color;
}

void LCD_Draw_Picture_Pro(uint16_t *xstr,uint16_t *ystr,uint16_t sendbyte,uint8_t * pic)
{
    
    uint8_t temp,t1;
    uint16_t width_byte;
    uint16_t height_total,y_t;
    uint16_t x_t = *xstr;
		uint8_t *pusMsk=pic;//指向指针的指针，为了不改变pic这个指针
    uint16_t width_total = INIT_X + PICTURE_WIDTH;
    uint16_t flag = 0;
		
    width_byte = PICTURE_WIDTH % 8 ? (PICTURE_WIDTH / 8 + 1) : (PICTURE_WIDTH / 8);//宽度是否是8的整数倍，不是对一行的最后一个字节做特殊处理
    height_total = sendbyte % width_byte ? (sendbyte / width_byte + 1) : (sendbyte / width_byte);//计算一共有多少行
    
    
    for (y_t = *ystr; y_t < height_total+*ystr+1;) {
				temp = *pusMsk;
        flag++;
				if (flag>sendbyte) {//判断数据是否已经读取完了
            break;
        }
				
        if (x_t >= width_total - (PICTURE_WIDTH % 8)) {       //如果像素点是8的倍数，或者大于整数倍一
            if (PICTURE_WIDTH % 8) {    //整数倍
                for (t1 = 0; t1 < 4; t1++) {
                    //判断画点
									if (temp & 0x80) {
                        LCD_Color_DrawPoint(x_t, y_t, BLACK);
                    }else{
                        LCD_Color_DrawPoint(x_t, y_t, WHITE);
                    }
                }
                    temp <<= 1;
                    x_t++;

                x_t = INIT_X;
                if(++y_t>=INIT_Y+PICTURE_HEIGHT){*ystr=INIT_Y;y_t=*ystr;}
								
            
            }
            else
            {
                x_t = INIT_X;
                if(++y_t>=INIT_Y+PICTURE_HEIGHT){*ystr=INIT_Y;y_t=*ystr;}
								
                for (t1 = 0; t1 < 8; t1++) {
                   
                    //判断画点
									if (temp & 0x80) {
                        LCD_Color_DrawPoint(x_t, y_t, BLACK);
                    }else{
                        LCD_Color_DrawPoint(x_t, y_t, WHITE);
                    }
                    temp <<= 1;
                    x_t++;

                }
            }
        }
        else {
            for (t1 = 0; t1 < 8; t1++) {
                
                //判断画点
								if (temp & 0x80) {
                        LCD_Color_DrawPoint(x_t, y_t, BLACK);
                    }else{
                        LCD_Color_DrawPoint(x_t, y_t, WHITE);
                    }
                    temp <<= 1;
                x_t++;

            }
        
        }
				pusMsk++;
        
    }
    *xstr = x_t;
    *ystr = y_t;
 
}



void LCD_Draw_Picture_Promax  (uint16_t *xstr,uint16_t *ystr,uint16_t sendbyte,uint8_t * pic)
{
    
    uint8_t temp,t1;
    uint16_t width_byte;
    uint16_t height_total,y_t;
    uint16_t x_t = *xstr;
		uint8_t *pusMsk=pic;
    uint16_t width_total = INIT_X + PICTURE_WIDTH;
    uint16_t flag = 0;
		
    width_byte = PICTURE_WIDTH % 8 ? (PICTURE_WIDTH / 8 + 1) : (PICTURE_WIDTH / 8);//宽度是否是8的整数倍，不是对一行的最后一个字节做特殊处理
    height_total = sendbyte % width_byte ? (sendbyte / width_byte + 1) : (sendbyte / width_byte);//计算一共有多少行
    
    
    for (y_t = *ystr; y_t < height_total+*ystr+1;) {
				temp = *pusMsk;
        flag++;
				if (flag>sendbyte) {
            break;
        }
				
        if (x_t >= width_total - (PICTURE_WIDTH % 8)) {
            if (PICTURE_WIDTH % 8) {
                for (t1 = 0; t1 < 4; t1++) {
                    //判断画点
									if (temp & 0x80) {
                        LCD_Color_DrawPoint(x_t, y_t, BLACK);
                    }else{
                        LCD_Color_DrawPoint(x_t, y_t, WHITE);
                    }
                    temp <<= 1;
                    x_t++;
                }
                x_t = INIT_X;
                if(++y_t>=INIT_Y+PICTURE_HEIGHT){*ystr=INIT_Y;y_t=*ystr;}
								
            
            }
            else
            {
                x_t = INIT_X;
                if(++y_t>=INIT_Y+PICTURE_HEIGHT){*ystr=INIT_Y;y_t=*ystr;}
								
                for (t1 = 0; t1 < 8; t1++) {
                   
                    //判断画点
									if (temp & 0x80) {
                        LCD_Color_DrawPoint(x_t, y_t, BLACK);
                    }else{
                        LCD_Color_DrawPoint(x_t, y_t, WHITE);
                    }
                    temp <<= 1;
                    x_t++;

                }
                
            }
        }
        else {
            for (t1 = 0; t1 < 8; t1++) {
                
                //判断画点
								if (temp & 0x80) {
                        LCD_Color_DrawPoint(x_t, y_t, BLACK);
                    }else{
                        LCD_Color_DrawPoint(x_t, y_t, WHITE);
                    }
                    temp <<= 1;
                    x_t++;

            }
            
        
        }
				pusMsk++;
        
      }
      *xstr = x_t;
      *ystr = y_t;

 
}


/* USER CODE END Application */
