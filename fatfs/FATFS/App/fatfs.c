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
 #include "bsp_xpt2046_lcd.h"
 #include "ws_drive_tft.h"
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */
#define LCD_WIDTH   240
#define LCD_HEIGHT  320
#define BUFFER_SIZE 512
#define LINE_BUF_SIZE  LCD_WIDTH*16  // 每行240像素
#define PIC_SIZE 2048 // 240*320*2字节
uint8_t framebuffer[PIC_SIZE] = {1};

  struct bmp_file             //BMP文件头结构 , 本结构体所占用字节数,即14个字节
  {
    uint16_t  type1;           //位图文件的类型，必须为BM，我这里类型不对，所以显示有误。
    uint16_t  type2;           
    uint32_t  size;            //位图文件的大小，以字节为单位
    uint16_t  rd1;             // 位图文件保留字，必须为0
    uint16_t  rd2;             // 位图文件保留字，必须为0
    uint32_t  offset;          // 位图数据的起始位置，以相对于位图
  };
  
  
  struct bmp_info            //图像信息区
  {
    uint32_t bsize;          //本结构体所占用字节数,即40个字节
    uint32_t width;          // 位图的宽度，以像素为单位，像素数量是4字节对齐的
    uint32_t height;         // 位图的高度，以像素为单位
    uint16_t planes;         // 目标设备的级别，必须为1
    uint16_t count;          // 每个像素所需的位数，必须是1(双色）// 4(16色)，8(256色)或24(真彩色)之一
    uint32_t compression;    // 位图压缩类型，必须是 0(不压缩),// 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
    uint32_t sizeimage;      // 位图的大小，以字节为单位
    uint32_t xmeter;         // 位图水平分辨率，每米像素数
    uint32_t ymeter;         // 位图垂直分辨率，每米像素数
    uint32_t cused;          // 位图实际使用的颜色表中的颜色数
    uint32_t cimportant;     // 位图显示过程中重要的颜色数
  };
  
   
  
  struct bmp_head 
  {
    struct bmp_file file;
    struct bmp_info info;
  };
  
  


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

char WS_TFT_Load_Pic_Bmp(uint16_t x,uint16_t y, char  *path ) {
	   FRESULT  fre;
	   struct bmp_head bmp;
	   uint16_t  color , color1  ;
	   static uint8_t  buffer[300];
	   uint32_t  br;
	 
	   fre = f_open(&SDFile,path,FA_READ);
	   if(fre == FR_OK)
		 {
      if( f_read(&SDFile, (char *)&bmp + 2, sizeof(struct bmp_head ) - 2, &br) == FR_OK)
      {				  
        f_lseek(&SDFile,bmp.file.offset);
        uint16_t  width = 0;
        y += bmp.info.height-1;
      
        while(1)
        {
          if( f_read(&SDFile,buffer,300,&br) == FR_OK)
          {
            int i;            
            if(bmp.info.count == 24)
            {
              for (i = 0; i < br; i += 3)
              {
                  WS_TFT_Draw_Point(x+width,y,RGB888ToRGB565(buffer[i+2],buffer[i+1],buffer[i]));
                  width  ++;
                  if(width == bmp.info.width)
                  {
                    width = 0;
                    y --     ;
                  }
              }
            }
            if(bmp.info.count == 16)
            {
              for (i = 0; i < br; i += 2)
              {
                color = 0;
                color |= (uint16_t)buffer[i+1]		 << 8    ;
                color |= (uint16_t)buffer[i]		  	       ;
                
                color1   = color << 1;
                color1  &= 0xffe0;//1111111111100000
                color1  |= color & 0x001f;//0000000000011111 											
                
                Draw_Point(x+width,y,color1) ;
              
                width  ++;
                if(width == bmp.info.width)
                {
                  width = 0;
                  y --     ;
                }               
                if((color&0x8000) == (uint16_t)0x8000)   
                {
                  ILI9341_DispString_EN(0,0,"error1");
                }
              }
            }
          }
          else
          {
            break;
          }
          if(br == 0)
          {
            break;
          }  
        }
      }
			f_close(&SDFile);
		 }
		 else{//打开失败
		   ILI9341_DispString_EN(0,240,"open fail  \r\n");
		 }
	
}

// void binimage(uint16_t x,uint16_t y, char  *path){
//   UINT br;                    /* 成功读取的字节个数 */
//   FRESULT res;
//   uint32_t offset = 0;       /* 存储文件指针移动 */
//   uint32_t file_size = 0;   /* 文件大小 */  
//   uint16_t  color , color1  ;
//   res = f_open(&SDFile, path, FA_READ);//打开bin文件
  
//     if (res == FR_OK)     //打开成功
//     {
//       for (int i = 0; i < br; i += 2)
//               {
//                 color = 0;
//                 color |= (uint16_t)buffer[i+1]		 << 8    ;
//                 color |= (uint16_t)buffer[i]		  	       ;
                
//                 color1   = color << 1;
//                 color1  &= 0xffe0;//1111111111100000
//                 color1  |= color & 0x001f;//0000000000011111 											
                
//                 Draw_Point(x+width,y,color1) ;
              
//                 width  ++;
//                 if(width == bmp.info.width)
//                 {
//                   width = 0;
//                   y --     ;
//                 }               
//     }
//     else
//     {
//         ILI9341_DispString_EN(0, 240, "open fail  \r\n");
//     }

// }

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
