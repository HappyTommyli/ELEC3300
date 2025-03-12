#include "HMC5883L.h"


 uint8_t CRA = 0x70;
 uint8_t CRB = 0xA0;
 uint8_t Con_Mode = 0x00;
 uint8_t Single_Mode = 0x01;




void HMC_INIT() // open the register A and B
{

    HAL_I2C_Mem_Write(&hi2c2, 0x3c, 0x00, 1, &CRA, 1, 100);
    HAL_I2C_Mem_Write(&hi2c2, 0x3c, 0x01, 1, &CRB, 1, 100);
}

void HMC_Con_Mode()
{

    HAL_I2C_Mem_Write(&hi2c2, 0x1E << 1, 0x02, 1, &Con_Mode, 1, 100);
}

void HMC_Single_Mode()
{

    HAL_I2C_Mem_Write(&hi2c2, 0x1E << 1, 0x02, 1, &Single_Mode, 1, 100 );
}

int16_t HMC_Get_Axisdata(uint8_t axis)
{

    uint8_t LSB=0;
    uint8_t MSB=0;
    HAL_I2C_Mem_Read(&hi2c2, 0x3D,   axis , 1, &MSB, 1, 100);
    HAL_I2C_Mem_Read(&hi2c2, 0x3D,   axis+1 , 1, &LSB, 1, 100);

    return (MSB << 8) | (LSB & 0xFF);
    // 2’s complement form
}

void HMC_Get_Alldata(int16_t *X_Data,int16_t *Y_Data,int16_t *Z_Data)
{
    *X_Data = HMC_Get_Axisdata(X_axis);
    *Y_Data = HMC_Get_Axisdata(Y_axis);
    *Z_Data = HMC_Get_Axisdata(Z_axis);
}

void HMC5883_ReadXYZ( int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];

    
    HAL_I2C_Mem_Read(&hi2c2, 0x3D, X_axis, 1, data, 6,200);

  
    *x = (data[0] << 8) | data[1];
    *z = (data[2] << 8) | data[3];
    *y = (data[4] << 8) | data[5];
}

void HMC5883_ReadAngle(int8_t *angle) {
    int16_t x, y, z;
    HMC5883_ReadXYZ( &x, &y, &z); 

    *angle = atan2((float)y, (float)x) * (180.0 / 3.14); // 转换为度数


    if (*angle < 0) {
        *angle += 360;
    }
}








