
#ifndef WS_DRIVE_BORAD
#define WS_DRIVE_BORAD

#include "main.h"

#define DEBUG_EN 1


void WS_Debug(char *p,...) ;  //  printf 

char  getKeyFallingEdge (unsigned char io ) ;

//  ��������������
void WS_Borad_Buzzer_Open_Time(int time);

//  ���е���������
void WS_Borad_LED_RUN_Open_Time(int time);
//  ���Ե���������
void WS_Borad_LED_DEBUG_Open_Time(int time);


//   ������ , ����ɨ��
int  WS_OS_Task_Keyborad(void * date);
//   ������ , ���е�
int  WS_OS_Task_Runled(void * date);
int  WS_OS_Task_B(void * date);


#endif

