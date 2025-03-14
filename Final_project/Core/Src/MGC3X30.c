#include "MGC3X30.h"

DFRobot_MGC3130::DFRobot_MGC3130(,I2C_HandleTypeDef *hi2c2)
{
  _deviceAddr = DFRobot_MGC3130_IIC_ADDR;
  _hi2c2= hi2c2;
  position = false;
  lastTimeStamp=0 ;
}

bool DFRobot_MGC3130::begin(void)
{
  tsInput();
  reset();
  return true;
}


void DFRobot_MGC3130::reset()
{
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
  delay(250);
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
  delay(2000);
}


uint8_t DFRobot_MGC3130::setRuntimeparameter(void* pBuf,size_t size)
{
  if(pBuf == NULL){
  return 0;
  }
 
    uint8_t data[4 + size];  
    data[0] = 0x10;
    data[1] = 0x00;
    data[2] = 0x00;
    data[3] = 0xA2;

    for (uint16_t i = 0; i < size; i++) {
        data[4 + i] = pBuf[i];
    }

    HAL_I2C_Master_Transmit(&hi2c2,0x85, data, 4 + size, HAL_MAX_DELAY)
        
  return size;
}


uint8_t DFRobot_MGC3130::read(void* pBuf, size_t size)
{
  if(pBuf == NULL){
   return 0;

  }

    HAL_I2C_Master_Receive(&hi2c2,0x85, pBuf, size, HAL_MAX_DELAY)

  return size;
}

int8_t DFRobot_MGC3130::enableGestures()
{
  uint8_t pBuf[]={0x85,0x00, 0x00,0x00, 0x7F,0x00,0x00,0x00, 0x7F,0x00,0x00,0x00};
  uint8_t recvBuf[16];
  int8_t ret = -1;

  setRuntimeparameter(pBuf,12);
  if(read(recvBuf,16)!=0){
    if(recvBuf[4] == 0xA2){
      uint16_t errorCode = (uint16_t)recvBuf[7]>>8 | recvBuf[6];
      if(errorCode==0){
        ret = 0;
      }
    }
  }
  return ret;
}

int8_t DFRobot_MGC3130::disableGestures()
{
  uint8_t pBuf[]={0x85,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
  uint8_t recvBuf[16];
  int8_t ret = -1;
  setRuntimeparameter(pBuf,12);
  if(read(recvBuf,16)!=0){
    if(recvBuf[4] == 0xA2){
      uint16_t errorCode = (uint16_t)recvBuf[7]>>8 | recvBuf[6];
      if(errorCode==0){
        ret = 0;
      }
    }
  }
  return ret;
}

uint8_t DFRobot_MGC3130:: getGestureInfo()
{

  return (uint8_t) (info.gestureInfo & 0xFF);
}

void DFRobot_MGC3130:: sensorDataRecv()
{
  uint8_t pbuf[24];
  position = false;
  memset((void *)&info,0,sizeof(info));
  if(read(pbuf,24)!=0){
    if((pbuf[3] == 0x91) && (pbuf[4] == 0x1E)){
      info.gestureInfo  = pbuf[8]  | (uint32_t)pbuf[9]<<8  | (uint32_t)pbuf[10]<<16 |  (uint32_t)pbuf[11]<<24;
      //info.touchInfo    = pbuf[12] | (uint32_t)pbuf[13]<<8 | (uint32_t)pbuf[14]<<16 |  (uint32_t)pbuf[15]<<24;
      nowTimeStamp = (uint32_t)pbuf[14] |  (uint32_t)pbuf[15]<<8;
      nowTouch = pbuf[12] | (uint32_t)pbuf[13]<<8;
    //   if(pbuf[7] & 0x02){
    //     info.airWheelInfo = pbuf[16] | (uint32_t)pbuf[17]<<8;
    //   }
    //   if(pbuf[7] & 0x01){
    //     position = true;
    //     info.xPosition    = pbuf[18] | (uint32_t)pbuf[19]<<8;
    //     info.yPosition    = pbuf[20] | (uint32_t)pbuf[21]<<8;
    //     info.zPosition    = pbuf[22] | (uint32_t)pbuf[23]<<8;
    //   }
    } else if(pbuf[4] == 0x1F){
      while(enableDataOutput()!=0){
        delay(100);
      }
    //   while(lockDataOutput()!=0){
    //     delay(100);
    //   }
    }
  }else{
    delay(5);
  }
}