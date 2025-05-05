

#ifndef WS_DRIVE_MPU6050
#define WS_DRIVE_MPU6050

#include "main.h"	

void     WS_Init_MPU6050(void)          ;
void     WS_IIC_Write_Reg(uint8_t REG_Address,uint8_t REG_data);
uint8_t  WS_IIC_Read_Reg(uint8_t REG_Address);

int16_t WS_MPU6050_GetData(uint8_t REG_Address);


int WS_OS_Task_MPU6050(void * date);


#endif


