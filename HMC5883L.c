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

    HAL_I2C_Mem_Write(&hi2c2, 0x1E << 1, 0x02, 1, &Single_Mode, 1, 100);
}

int16_t HMC_Get_Axisdata(uint8_t axis)
{

    uint8_t LSB = 0;
    uint8_t MSB = 0;
    HAL_I2C_Mem_Read(&hi2c2, 0x3D, axis, 1, &MSB, 1, 100);
    HAL_I2C_Mem_Read(&hi2c2, 0x3D, axis + 1, 1, &LSB, 1, 100);

    return (MSB << 8) | (LSB & 0xFF);
    // 2’s complement form
}

void HMC_Get_Alldata(int16_t *X_Data, int16_t *Y_Data, int16_t *Z_Data)
{
    *X_Data = HMC_Get_Axisdata(X_axis);
    *Y_Data = HMC_Get_Axisdata(Y_axis);
    *Z_Data = HMC_Get_Axisdata(Z_axis);
}

void HMC5883_ReadXYZ(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t data[6];

    if (HAL_I2C_Mem_Read(&hi2c2, 0x3D, X_axis, 1, data, 6, 2000))
    {
        // HMC_INIT();
        // HMC_Con_Mode();
        // HAL_Delay(100);
        // HAL_I2C_Mem_Read(&hi2c2, 0x3D, X_axis, 1, data, 6,1000);
    }

    *x = (data[0] << 8) | data[1];
    *z = (data[2] << 8) | data[3];
    *y = (data[4] << 8) | data[5];
}

void HMC5883_Readangle(int16_t x, int16_t y,float_t *Rx,float_t *Ry,int16_t *angle,int16_t xmax, int16_t xmin, int16_t ymax, int16_t ymin) {

    // 计算偏移量
     int16_t Xoffset = (xmax - xmin) / 2;
      int16_t Yoffset = (ymax - ymin) / 2;

    // 计算缩放因子
    float_t Kx = 2.0f / (xmax - xmin); 
    float_t Ky = 2.0f / (ymax - ymin); 

  
    *Rx = (int16_t)(((float_t)(x) - Xoffset) * Kx);
    *Ry = (int16_t)(((float_t)(y) - Yoffset) * Ky);

     *angle = atan2((float)y, (float)x) * (180.0 / 3.14); 

   if (*angle < 0)
   {
       *angle += 360;
   }

}

void measure(float *Xoffest,float *Yoffest,float *Kx,float *Ky)
{

    uint8_t i = 0;

    float GaXmax = 0, GaXmin = 0, GaYmax = 0, GaYmin = 0;
    int16_t GaX, GaY,GaZ;
    while (i != 100)
    {
        HMC5883_ReadXYZ(&GaX,&GaY,&GaZ);

        GaXmax = GaXmax < GaX ? GaX : GaXmax;

        GaXmin = GaXmin > GaX ? GaX : GaXmin;

        GaYmax = GaYmax < GaY ? GaY : GaYmax;

        GaYmin = GaYmin > GaY ? GaY : GaYmin;

        HAL_Delay(200);

        i++;
    }

    *Xoffest = (GaXmax + GaXmin) / 2;
    *Yoffest = (GaYmax + GaYmin) / 2;
    *Kx = 2 / (GaXmax - GaXmin);
    *Ky = 2 / (GaXmax - GaXmin);
}
