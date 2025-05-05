
#ifndef WS_DRIVE_BORAD
#define WS_DRIVE_BORAD

#include "main.h"

#define DEBUG_EN 1


void WS_Debug(char *p,...) ;  //  printf 

char  getKeyFallingEdge (unsigned char io ) ;

//  蜂鸣器驱动函数
void WS_Borad_Buzzer_Open_Time(int time);

//  运行灯驱动函数
void WS_Borad_LED_RUN_Open_Time(int time);
//  调试灯驱动函数
void WS_Borad_LED_DEBUG_Open_Time(int time);


//   任务函数 , 按键扫描
int  WS_OS_Task_Keyborad(void * date);
//   任务函数 , 运行灯
int  WS_OS_Task_Runled(void * date);
int  WS_OS_Task_B(void * date);


#endif

