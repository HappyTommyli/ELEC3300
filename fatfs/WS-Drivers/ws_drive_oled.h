

#ifndef __DRIVE_OLED_H__
#define __DRIVE_OLED_H__	

#include "main.h"
#include "stdlib.h"	

//  1 : �Դ�ģʽ��DMA���� ����ʡCPUʱ�䣬��Ҫ���ڣ�50ms������ WS_OLED_Start_Buffer_Update  ����
//  0 ���Ĵ���ģʽ��ֱ��д����OLED���ȽϺ�ʱ������д��������ģʽ
#define WS_OLED_EN_DIS_BUFFER  1 


#define ASCII12X24Code_EN  1    //  ����0 �� ʧ��12*24���ַ���ʾ �� ����1 �� ʹ��12*24���ַ���ʾ 
#define ASCII16X32Code_EN  1    //  ����0 �� ʧ��16*36���ַ���ʾ �� ����1 �� ʹ��16*32���ַ���ʾ 


	     

//  ����OLED��ʾ    
void WS_OLED_Open_Dis(void);

// �ر�OLED��ʾ     
void WS_OLED_Close_Dis(void); 

//��ʼ��SSD1306					    
void OLEDInit(void);

// ȫ��Ϩ��	  
void WS_OLED_Set_Clear(void) ;

// ����д���ݽ���OLED 
char WS_OLED_Write_Buffer(char adds , unsigned  char *data  , int len);

//  ����һ��DMA���䣬ʵѵ�������
void WS_OLED_Start_Buffer_Update(void);


//   ��ʼ��
void   WS_OLED_Init(void );

//   �ַ�����ʾ
void WS_OLED_Dis_String(int  x,int   y,   char disMode, char  *string);

//   ��ʽ����ʾ
void WS_OLED_Printf(int  x,int  y, char disMode , char *p,... );

void WS_OLED_Clear(void)  ;

int WS_OLED_Task_Buffer_Update(void *date);




#endif  

