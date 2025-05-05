
#include "ws_drive_borad.h"
#include "fatfs.h"

#include <stdarg.h>		  //  标准头文件
#include <stdio.h>
#include <string.h>


extern   UART_HandleTypeDef huart1;


//   格式化可变变量字符串缓存
char  formatBuf[128];
//   格式化输出的调试函数
void WS_Debug(char *p,...)   // 与 printf  一摸一样的功能 
{
	  /*  格式化数据  */
    va_list ap;
		va_start(ap,p);
		vsprintf(formatBuf,p,ap);
		va_end(ap);	
	
//  编译控制	
#ifdef DEBUG_EN    
	  //  信息输出接口
	  HAL_UART_Transmit(&huart1,(uint8_t *)formatBuf,strlen(formatBuf),20);
	  	
#endif	 
}

//    获取按键下降沿函数 , 调用要求：1~10间隔调用
//    注意：1、io变量必须使用无符号类型，
//          2、countFlag必须是静态或者全局变量
//          3、传入按键数据时无效位（没接按键的位）置1
//    返回值：被按下的按键编号
char  getKeyFallingEdge (unsigned char io )  // char : 0~255
{
    char              key   = 0  ;    //  局部变量
	  static  char  countFlag = 0  ;    //  静态变量
		if(io != 0xff )  // 说明有按键按下
		{
			 if(countFlag < 3)  countFlag ++;  //  1,2,3 
			
			 if(countFlag == 2) // 连续进入两次，实现去抖   
			 {
						switch(io)
						{
							 case 0xfe :  key = 1;  break ; //  1111 1110 , SW1按下
							 case 0xfd :  key = 2;  break ; //  1111 1101 , SW2按下
							 case 0xfb :  key = 3;  break ; //  1111 1011 , SW3按下
							 case 0xf7 :  key = 4;  break ; //  1111 0111 , SW4按下
							 case 0xef :  key = 5;  break ; //  1110 1111 , SW5按下
							 case 0xdf :  key = 6;  break ; //  1101 1111 , SW6按下
							 case 0xbf :  key = 7;  break ; //  1101 1111 , SW7按下
							 case 0x7f :  key = 8;  break ; //  1101 1111 , SW8按下
							
						}
			 }
		}
		else
		{
		   countFlag = 0;
		}
	  return key  ;
}

//   蜂鸣器关闭函数
int  WS_Borad_Buzzer_Close(void *date)
{
     HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
	   return 0;
}

//  蜂鸣器驱动函数
void WS_Borad_Buzzer_Open_Time(int time)
{
     HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);
	   WS_OS_Create_Timer(WS_Borad_Buzzer_Close,time) ;
}


//   运行灯关闭函数
int  WS_Borad_LED_RUN_Close(void *date)
{
     HAL_GPIO_WritePin(LED_RUN_GPIO_Port,LED_RUN_Pin,GPIO_PIN_SET);
	   return 0;
}

//  运行灯驱动函数
void WS_Borad_LED_RUN_Open_Time(int time)
{
      HAL_GPIO_WritePin(LED_RUN_GPIO_Port,LED_RUN_Pin,GPIO_PIN_RESET);
	    WS_OS_Create_Timer(WS_Borad_LED_RUN_Close,time) ;
}


//   调试灯关闭函数
int  WS_Borad_LED_DEBUG_Close(void *date)
{
     HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port,LED_DEBUG_Pin,GPIO_PIN_SET);
	   return 0;
}

//  调试灯驱动函数
void WS_Borad_LED_DEBUG_Open_Time(int time)
{
      HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port,LED_DEBUG_Pin,GPIO_PIN_RESET);
	    WS_OS_Create_Timer(WS_Borad_LED_DEBUG_Close,time) ;
}





uint8_t  buff[1024];

//   任务函数 , 运行灯
int  WS_OS_Task_Runled(void * date)
{
	
	  static  int num  = 0;
	  uint32_t  tick;
	
	  WS_DWT_Get_Tick_Dt(0,0);
	
//	  WS_TFT_Dis_16X32_String(0,100,"显示时间测试",WHITE,BROWN);
//	  WS_Font32_Read_Date(buff,(uint8_t *)"读取时间测试",6);
	 
	  tick = WS_DWT_Get_Tick_Dt(0,1)/72;
	
//    WS_Debug("time %d us\r\n",tick)  ;
	  
	
	  WS_Borad_LED_RUN_Open_Time(100)  ;
    return  1000;
}




//   任务函数 , 按键扫描
int  WS_OS_Task_Keyborad(void * date)
{
    static int key = 0;
	  key = getKeyFallingEdge(GPIOC->IDR | 0XE0) ;
		
		if(key == 1)
		{
			 WS_TFT_Clear(WHITE);
			 WS_TFT_Load_Pic_Bmp(10,180,"pic1.bmp");
		   //WS_Borad_Buzzer_Open_Time(100);
		}
		if(key == 2)
		{
			 WS_TFT_Clear(WHITE);
			 WS_TFT_Load_Pic_Bmp(0,0,"pic2.bmp");
		   //WS_Borad_Buzzer_Open_Time(500);
		}
		if(key == 3)
		{
		   WS_TFT_Clear(WHITE);
			 WS_TFT_Load_Pic_Bmp(0,0,"pic3.bmp");
		}
		
		if(key == 4)
		{
		   WS_TFT_Clear(WHITE);
			 WS_TFT_Load_Pic_Bmp(0,0,"pic4.bmp");
		}
		
			if(key == 5)
		{
		   WS_TFT_Clear(WHITE);
			 WS_TFT_Load_Pic_Bmp(0,0,"pic5.bmp");
		}
		
		if(key > 0)
		{
		    WS_Debug("key %d\r\n",key);  
			  WS_Borad_Buzzer_Open_Time(100);
		}
	  
	  return 10;  //  返回值决定了下次什么时候执行该函数
}

//   任务函数B
int  WS_OS_Task_B(void * date)
{
    static int num = 0;
	  WS_Debug("TASK B output %d\r\n",num ++);
	  
	  return 500;  //  返回值决定了下次什么时候执行该函数
}



/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
//  注意：回调函数是再中断程序内执行，不能再这个函数里面执行耗时操作
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
	
}






