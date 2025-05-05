#ifndef LCD_H
#define LCD_H

#include "stm32f1xx.h"
#include "fonts.h"
//添加如下宏定义
//=======================================================================================
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
//=======================================================================================
//声名函数
void LCD_Draw_Picture_Pro(uint16_t *xstr,uint16_t *ystr,uint16_t sendbyte,uint8_t * pic);

#endif // LCD_H