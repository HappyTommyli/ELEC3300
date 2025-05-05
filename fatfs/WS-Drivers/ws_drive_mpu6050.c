//****************************************
// Update to MPU6050 by shinetop
// MCU: STC89C52
// 2012.3.1
// 功能: 显示加速度计和陀螺仪的10位原始数据
//****************************************
// GY-52 MPU3050 IIC测试程序
// 使用单片机STC89C51 
// 晶振：11.0592M
// 显示：LCD1602
// 编译环境 Keil uVision2
// 参考宏晶网站24c04通信程序
// 时间：2011年9月1日
// QQ：531389319
//****************************************
#include "ws_drive_mpu6050.h"

extern  I2C_HandleTypeDef hi2c2;


typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;


//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取
//****************************************
//定义类型及变量
//****************************************


//MPU6050操作函数
void  WS_Init_MPU6050(void);													//初始化MPU6050

    
//**************************************
//向I2C设备写入一个字节数据
//**************************************
void WS_IIC_Write_Reg(uint8_t REG_Address,uint8_t REG_data)
{
	 HAL_StatusTypeDef stu =   HAL_I2C_Mem_Write(&hi2c2,SlaveAddress,REG_Address,I2C_MEMADD_SIZE_8BIT,&REG_data,1,1) ;
   if(stu != HAL_OK)
	 {
	   WS_Debug("IIC write err %d\r\n",stu);
	 }
}
//**************************************
//从I2C设备读取一个字节数据
//**************************************
uint8_t WS_IIC_Read_Reg(uint8_t REG_Address)
{
	 uint8_t REG_data;
	 HAL_I2C_Mem_Read(&hi2c2,SlaveAddress,REG_Address,I2C_MEMADD_SIZE_8BIT,&REG_data,1,1);
	 return REG_data;
}
//**************************************
//初始化MPU6050
//**************************************
void WS_Init_MPU6050(void)
{
	WS_IIC_Write_Reg(PWR_MGMT_1, 0x00);	//解除休眠状态
	WS_IIC_Write_Reg(SMPLRT_DIV, 0x07);
	WS_IIC_Write_Reg(CONFIG, 0x06);
	WS_IIC_Write_Reg(GYRO_CONFIG, 0x18);
	WS_IIC_Write_Reg(ACCEL_CONFIG, 0x01);
}
//**************************************
//合成数据
//**************************************
int16_t WS_MPU6050_GetData(uint8_t REG_Address)
{
	uint8_t buf[2];
	HAL_I2C_Mem_Read(&hi2c2,SlaveAddress,REG_Address,I2C_MEMADD_SIZE_8BIT,buf,2,1);
	return (buf[0]<<8)+buf[1];   //合成数据
}

//  任务函数 
int WS_OS_Task_MPU6050(void * date)
{
//	  WS_Debug ("ACCEL:x,y,x,GYRO:%6d,%6d,%6d,%6d,%6d,%6d\r\n",WS_MPU6050_GetData(ACCEL_XOUT_H),WS_MPU6050_GetData(ACCEL_YOUT_H),WS_MPU6050_GetData(ACCEL_ZOUT_H),
//	                                                           WS_MPU6050_GetData(GYRO_XOUT_H), WS_MPU6050_GetData(GYRO_YOUT_H) ,WS_MPU6050_GetData(GYRO_ZOUT_H));
	  return 200;
}





////*********************************************************
////主程序
////*********************************************************
//void main()
//{ 
//	
//	
//		Display10BitData(GetData(ACCEL_XOUT_H),2,0);	//显示X轴加速度
//		Display10BitData(GetData(ACCEL_YOUT_H),7,0);	//显示Y轴加速度
//		Display10BitData(GetData(ACCEL_ZOUT_H),12,0);	//显示Z轴加速度
//		Display10BitData(GetData(GYRO_XOUT_H),2,1);	//显示X轴角速度
//		Display10BitData(GetData(GYRO_YOUT_H),7,1);	//显示Y轴角速度
//		Display10BitData(GetData(GYRO_ZOUT_H),12,1);	//显示Z轴角速度
//   
//	}
//}

