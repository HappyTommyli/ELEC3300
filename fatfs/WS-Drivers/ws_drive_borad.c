
#include "ws_drive_borad.h"
#include "fatfs.h"

#include <stdarg.h>		  //  ��׼ͷ�ļ�
#include <stdio.h>
#include <string.h>


extern   UART_HandleTypeDef huart1;


//   ��ʽ���ɱ�����ַ�������
char  formatBuf[128];
//   ��ʽ������ĵ��Ժ���
void WS_Debug(char *p,...)   // �� printf  һ��һ���Ĺ��� 
{
	  /*  ��ʽ������  */
    va_list ap;
		va_start(ap,p);
		vsprintf(formatBuf,p,ap);
		va_end(ap);	
	
//  �������	
#ifdef DEBUG_EN    
	  //  ��Ϣ����ӿ�
	  HAL_UART_Transmit(&huart1,(uint8_t *)formatBuf,strlen(formatBuf),20);
	  	
#endif	 
}

//    ��ȡ�����½��غ��� , ����Ҫ��1~10�������
//    ע�⣺1��io��������ʹ���޷������ͣ�
//          2��countFlag�����Ǿ�̬����ȫ�ֱ���
//          3�����밴������ʱ��Чλ��û�Ӱ�����λ����1
//    ����ֵ�������µİ������
char  getKeyFallingEdge (unsigned char io )  // char : 0~255
{
    char              key   = 0  ;    //  �ֲ�����
	  static  char  countFlag = 0  ;    //  ��̬����
		if(io != 0xff )  // ˵���а�������
		{
			 if(countFlag < 3)  countFlag ++;  //  1,2,3 
			
			 if(countFlag == 2) // �����������Σ�ʵ��ȥ��   
			 {
						switch(io)
						{
							 case 0xfe :  key = 1;  break ; //  1111 1110 , SW1����
							 case 0xfd :  key = 2;  break ; //  1111 1101 , SW2����
							 case 0xfb :  key = 3;  break ; //  1111 1011 , SW3����
							 case 0xf7 :  key = 4;  break ; //  1111 0111 , SW4����
							 case 0xef :  key = 5;  break ; //  1110 1111 , SW5����
							 case 0xdf :  key = 6;  break ; //  1101 1111 , SW6����
							 case 0xbf :  key = 7;  break ; //  1101 1111 , SW7����
							 case 0x7f :  key = 8;  break ; //  1101 1111 , SW8����
							
						}
			 }
		}
		else
		{
		   countFlag = 0;
		}
	  return key  ;
}

//   �������رպ���
int  WS_Borad_Buzzer_Close(void *date)
{
     HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
	   return 0;
}

//  ��������������
void WS_Borad_Buzzer_Open_Time(int time)
{
     HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);
	   WS_OS_Create_Timer(WS_Borad_Buzzer_Close,time) ;
}


//   ���еƹرպ���
int  WS_Borad_LED_RUN_Close(void *date)
{
     HAL_GPIO_WritePin(LED_RUN_GPIO_Port,LED_RUN_Pin,GPIO_PIN_SET);
	   return 0;
}

//  ���е���������
void WS_Borad_LED_RUN_Open_Time(int time)
{
      HAL_GPIO_WritePin(LED_RUN_GPIO_Port,LED_RUN_Pin,GPIO_PIN_RESET);
	    WS_OS_Create_Timer(WS_Borad_LED_RUN_Close,time) ;
}


//   ���Եƹرպ���
int  WS_Borad_LED_DEBUG_Close(void *date)
{
     HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port,LED_DEBUG_Pin,GPIO_PIN_SET);
	   return 0;
}

//  ���Ե���������
void WS_Borad_LED_DEBUG_Open_Time(int time)
{
      HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port,LED_DEBUG_Pin,GPIO_PIN_RESET);
	    WS_OS_Create_Timer(WS_Borad_LED_DEBUG_Close,time) ;
}





uint8_t  buff[1024];

//   ������ , ���е�
int  WS_OS_Task_Runled(void * date)
{
	
	  static  int num  = 0;
	  uint32_t  tick;
	
	  WS_DWT_Get_Tick_Dt(0,0);
	
//	  WS_TFT_Dis_16X32_String(0,100,"��ʾʱ�����",WHITE,BROWN);
//	  WS_Font32_Read_Date(buff,(uint8_t *)"��ȡʱ�����",6);
	 
	  tick = WS_DWT_Get_Tick_Dt(0,1)/72;
	
//    WS_Debug("time %d us\r\n",tick)  ;
	  
	
	  WS_Borad_LED_RUN_Open_Time(100)  ;
    return  1000;
}




//   ������ , ����ɨ��
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
	  
	  return 10;  //  ����ֵ�������´�ʲôʱ��ִ�иú���
}

//   ������B
int  WS_OS_Task_B(void * date)
{
    static int num = 0;
	  WS_Debug("TASK B output %d\r\n",num ++);
	  
	  return 500;  //  ����ֵ�������´�ʲôʱ��ִ�иú���
}



/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
//  ע�⣺�ص����������жϳ�����ִ�У������������������ִ�к�ʱ����
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
	
}






