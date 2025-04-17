#ifndef __BSP_XPT2046_LCD_H
#define	__BSP_XPT2046_LCD_H


#include "stm32f1xx.h"
#include "bsp_ili9341_lcd.h"
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>



/* XPT2046 触摸屏触摸信号指示引脚定义(不使用中断)*/
#define    XPT2046_PENIRQ_GPIO_PORT   GPIOE
#define    XPT2046_PENIRQ_GPIO_PIN    GPIO_PIN_4

//触屏信号有效电平
#define   XPT2046_PENIRQ_ActiveLevel       0
#define   XPT2046_PENIRQ_Read()        HAL_GPIO_ReadPin ( XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN )



/*XPT2046 触摸屏模拟SPI引脚定义*/

#define    XPT2046_SPI_CS_PIN		          GPIO_PIN_13
#define    XPT2046_SPI_CS_PORT		        GPIOD

#define	   XPT2046_SPI_CLK_PIN	          GPIO_PIN_0
#define    XPT2046_SPI_CLK_PORT	          GPIOE

#define	   XPT2046_SPI_MOSI_PIN	          GPIO_PIN_2
#define	   XPT2046_SPI_MOSI_PORT	        GPIOE

#define	   XPT2046_SPI_MISO_PIN	          GPIO_PIN_3
#define	   XPT2046_SPI_MISO_PORT	        GPIOE


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}	//设置为高电平		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}//输出低电平

#define   XPT2046_CS_ENABLE()      digitalHi( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN )    
#define   XPT2046_CS_DISABLE()     digitalLo ( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN )  

#define   XPT2046_CLK_HIGH()       digitalHi ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN )    
#define   XPT2046_CLK_LOW()        digitalLo ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN ) 

#define   XPT2046_MOSI_1()         digitalHi ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN ) 
#define   XPT2046_MOSI_0()         digitalLo ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN )

#define   XPT2046_MISO()           HAL_GPIO_ReadPin ( XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN )



/******************************* XPT2046 触摸屏参数定义 ***************************/
//校准触摸屏时触摸坐标的AD值相差门限 
#define             XPT2046_THRESHOLD_CalDiff                    2               

#define	            XPT2046_CHANNEL_X 	                          0x90 	          //通道Y+的选择控制字	
#define	            XPT2046_CHANNEL_Y 	                          0xd0	          //通道X+的选择控制字

//触摸参数写到FLASH里的标志
#define							FLASH_TOUCH_PARA_FLAG_VALUE					0xA5

//触摸标志写到FLASH里的地址
#define 							FLASH_TOUCH_PARA_FLAG_ADDR						(1*1024)

//触摸参数写到FLASH里的地址
#define 							FLASH_TOUCH_PARA_ADDR									(2*1024)


/*信息输出*/
#define XPT2046_DEBUG_ON         0
#define INIT_X 0	//起始x坐标
#define INIT_Y 0	//起始y坐标
#define	PICTURE_WIDTH 240
#define PICTURE_HEIGHT	320
#define TOTAL_BYTE	9600	//一张图片存储的总字节数，一个字节中有8个像素
#define BUFF_TOTAL_BYTE 512
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
//一张图片存储的总位数，即总像素，以300*230的图片为例，像素总个数为：300*230=69000
//但是为什么软件输出的图像8740*8=69920
//原因：图像的宽度300，即300个像素点，8个像素点对应一个字节，300\8=37余4，余下的4个像素被装入了一个字节的前四位中，后四位补0
//因此：一行相当于38个字节一共230行，总字节数：38*230=8740，多出来了：230*4=920补0像素
#define TOTAL_BIT		TOTAL_BYTE*8
#define CHUNK_ROWS 40 
#define LINE_BUFFER_SIZE (240*16)



#define XPT2046_INFO(fmt,arg...)           printf("<<-XPT2046-INFO->> "fmt"\n",##arg)
#define XPT2046_ERROR(fmt,arg...)          printf("<<-XPT2046-ERROR->> "fmt"\n",##arg)
#define XPT2046_DEBUG(fmt,arg...)          do{\
                                          if(XPT2046_DEBUG_ON)\
                                          printf("<<-XPT2046-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

/******************************* 声明 XPT2046 相关的数据类型 ***************************/
typedef	struct          //液晶坐标结构体 
{		
	/*负数值表示无新数据*/
   int16_t x;			//记录最新的触摸参数值
   int16_t y; 
	
	/*用于记录连续触摸时(长按)的上一次触摸位置*/
	 int16_t pre_x;		
   int16_t pre_y;
	
} strType_XPT2046_Coordinate;   


typedef struct         //校准因子结构体 
{
	 float An,  		 //注:sizeof(long double) = 8
					Bn,     
					Cn,   
					Dn,    
					En,    
					Fn,     
					Divider;
	
} strType_XPT2046_Calibration;


typedef struct         //校准系数结构体（最终使用）
{
	 float dX_X,  			 
					dX_Y,     
					dX,   
					dY_X,    
					dY_Y,    
					dY;

} strType_XPT2046_TouchPara;

/******触摸状态机相关******/
typedef enum
{
	XPT2046_STATE_RELEASE  = 0,	//触摸释放
	XPT2046_STATE_WAITING,			//触摸按下
	XPT2046_STATE_PRESSED,			//触摸按下
}enumTouchState	;

#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			0

//触摸消抖阈值
#define DURIATION_TIME				2

/******************************* 声明 XPT2046 相关的外部全局变量 ***************************/
extern volatile uint8_t               ucXPT2046_TouchFlag;

extern strType_XPT2046_TouchPara      strXPT2046_TouchPara[];



/******************************** XPT2046 触摸屏函数声明 **********************************/
void XPT2046_Init( void );
uint8_t XPT2046_Touch_Calibrate( uint8_t LCD_Mode );
uint8_t XPT2046_Get_TouchedPoint( strType_XPT2046_Coordinate * displayPtr, strType_XPT2046_TouchPara * para );
void XPT2046_TouchDown(strType_XPT2046_Coordinate * touch);
void XPT2046_TouchUp(strType_XPT2046_Coordinate * touch);
void XPT2046_TouchEvenHandler(void);
void Calibrate_or_Get_TouchParaWithFlash(uint8_t LCD_Mode,uint8_t forceCal);

/******************************** 自用函数声明 **********************************/
void open_filedirectory(uint16_t xstr, uint16_t ystr, char filepath[]);
void LoadAndDisplayCFile(uint16_t x,uint16_t y,char *filepath);
void LCD_ShowPicture(uint16_t usXstar,uint16_t usYstar,uint16_t usPicH,uint16_t usPicV,uint8_t ucPicNum);
void operating_system (strType_XPT2046_Coordinate *program_num);
void operating_window_Init(void);
#endif /* __BSP_TOUCH_H */

