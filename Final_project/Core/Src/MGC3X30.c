#include "MGC3X30.h"
#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

uint8_t _deviceAddr = 0x42;
uint8_t position;

// version for freertos
//change the hal_delay 
void MGC3X30_task(){

  // begin();
  // sInfo_t MGC_info = {0};
  // uint16_t lastTimeStamp;
  // uint16_t nowTimeStamp;
  // uint16_t lastTouch;
  // uint16_t nowTouch;
  // uint16_t gesture_data=0;
  // uint16_t touch_data=0;
  
  // sensorDataRecv(&MGC_info,&nowTimeStamp,&nowTouch);
  // gesture_data = getGestureInfo(&MGC_info);
  // touch_data = getTouchInfo(&MGC_info,&lastTimeStamp,&nowTimeStamp,&lastTouch,&nowTouch);
  
    // switch (data)
    // {
    // case 2:
    //   /* code */
    //   LCD_DrawString(0, 0, "r");
    //   break;
    // case 3:
    //   /* code */
    //   LCD_DrawString(0, 0, "l");
    //   break;
    // case 4:
    //   /* code */
    //   LCD_DrawString(0, 0, "u");
    //   break;
    // case 5:
    //   /* code */
    //   LCD_DrawString(0, 0, "d");
    //   break;

    // default:
    //   break;
    // }

}

void tsInput()
{ GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void tsOutput()
{ GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint8_t tsRead()
{
  return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
}

void tsWrite(uint8_t mode)
{
  if (mode)
  {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  }

  else
  {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
  }
}


void reset_info(sInfo_t *info){
    (*info).gestureInfo = 0;
    (*info).touchInfo = 0;
   (*info).xPosition =0;
   (*info).airWheelInfo=0;
   (*info).yPosition =0;
    (*info).zPosition =0;
}

uint8_t begin()
{
  tsInput();
  reset();
  return 1;
}

void reset()
{

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  // HAL_Delay(250);
   vTaskDelay(pdMS_TO_TICKS(250)); 
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
  // HAL_Delay(2000);
   vTaskDelay(pdMS_TO_TICKS(2000)); 
}

uint8_t setRuntimeparameter(uint8_t *pBuf, size_t size)
{
  if (pBuf == NULL)
  {
    return 0;
  }

  uint8_t data[4 + size];
  data[0] = 0x10;
  data[1] = 0x00;
  data[2] = 0x00;
  data[3] = 0xA2;

  for (uint16_t i = 0; i < size; i++)
  {
    data[4 + i] = pBuf[i];
  }

  HAL_I2C_Master_Transmit(&hi2c2, 0x84, data, 4 + size, 10000);

  return size;
}

uint8_t read(uint8_t *pBuf, size_t size)
{

  if (tsRead() != 0)
  {
    return 0;
  }
  tsOutput();
  tsWrite(0);
  HAL_I2C_Master_Receive(&hi2c2, 0x85, pBuf, size,10000);
  tsWrite(1);
  tsInput();
  // // HAL_Delay(5);
 vTaskDelay(pdMS_TO_TICKS(5)); 
  return size;
}

int8_t enableGestures()
{
  uint8_t pBuf[] = {0x85, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00};
  uint8_t recvBuf[16];
  int8_t ret = -1;

  setRuntimeparameter(pBuf, 12);
  if (read(recvBuf, 16) != 0)
  {
    if (recvBuf[4] == 0xA2)
    {
      uint16_t errorCode = (uint16_t)recvBuf[7] >> 8 | recvBuf[6];
      if (errorCode == 0)
      {
        ret = 0;
      }
    }
  }
  return ret;
}

int8_t disableGestures()
{
  uint8_t pBuf[] = {0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t recvBuf[16];
  int8_t ret = -1;
  setRuntimeparameter(pBuf, 12);
  if (read(recvBuf, 16) != 0)
  {
    if (recvBuf[4] == 0xA2)
    {
      uint16_t errorCode = (uint16_t)recvBuf[7] >> 8 | recvBuf[6];
      if (errorCode == 0)
      {
        ret = 0;
      }
    }
  }
  return ret;
}

int8_t enableAirWheel()
{
  uint8_t pBuf[]={0x90,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x20,0x00,0x00,0x00};
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

uint8_t getGestureInfo(sInfo_t *info)
{

  return (uint8_t)(info->gestureInfo & 0xFF);
}
uint16_t getTouchInfo(sInfo_t *info, uint16_t *lastTimeStamp,uint16_t *nowTimeStamp,uint16_t *lastTouch,uint16_t *nowTouch)
{
  uint16_t data = 0;
  data = (*info).touchInfo & 0xFFFF;
  if((*info).touchInfo & 0x3E0){
    if((*nowTouch == *lastTouch) && (*nowTimeStamp == *lastTimeStamp)){
      data = (*info).touchInfo & 0xFC1F;
    }
  }
  if((*info).touchInfo & 0x7C00){
    if((*nowTouch == *lastTouch) && (*nowTimeStamp == *lastTimeStamp)){
      data = (*info).touchInfo & 0x83FF;
    }
  }
  *lastTouch = *nowTouch;
  *lastTimeStamp = *nowTimeStamp;
  return data;
}

int8_t enableTouchDetection()
{
  uint8_t pBuf[]={0x97,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x08,0x00,0x00,0x00};
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
int8_t disableTouchDetection()
{
  uint8_t pBuf[]={0x97,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00};
  uint8_t recvBuf[16];
  int8_t ret =-1;
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

int8_t enableDataOutput()
{
  uint8_t pBuf[]={0xA0,0x00, 0x00,0x00, 0x1E,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF};
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
int8_t lockDataOutput()
{
  uint8_t pBuf[] = {0xA1, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};
  uint8_t recvBuf[16];
  int8_t ret = -1;
  setRuntimeparameter(pBuf, 12);
  if (read(recvBuf, 16) != 0)
  {
    if (recvBuf[4] == 0xA2)
    {
      uint16_t errorCode = (uint16_t)recvBuf[7] >> 8 | recvBuf[6];
      if (errorCode == 0)
      {
        ret = 0;
      }
    }
  }
  return ret;
}

void sensorDataRecv(sInfo_t *info,uint16_t *nowTimeStamp,uint16_t *nowTouch)
{
  uint8_t pbuf[24];
  position = 0;
 
  if (read(pbuf, 24) != 0)
  {
    if ((pbuf[3] == 0x91) && (pbuf[4] == 0x1E))
    {
       (*info).gestureInfo = pbuf[8] | (uint32_t)pbuf[9] << 8 | (uint32_t)pbuf[10] << 16 | (uint32_t)pbuf[11] << 24;
       (*info).touchInfo    = pbuf[12] | (uint32_t)pbuf[13]<<8 | (uint32_t)pbuf[14]<<16 |  (uint32_t)pbuf[15]<<24;
            *nowTimeStamp = (uint32_t)pbuf[14] |  (uint32_t)pbuf[15]<<8;
            *nowTouch = pbuf[12] | (uint32_t)pbuf[13]<<8;
        if(pbuf[7] & 0x02){
         (*info).airWheelInfo = pbuf[16] | (uint32_t)pbuf[17]<<8;
        }
        if(pbuf[7] & 0x01){
          position = 1;
          (*info).xPosition    = pbuf[18] | (uint32_t)pbuf[19]<<8;
           (*info).yPosition    = pbuf[20] | (uint32_t)pbuf[21]<<8;
          (*info).zPosition    = pbuf[22] | (uint32_t)pbuf[23]<<8;
        }
    }
    else if (pbuf[4] == 0x1F)
    {
      while (enableDataOutput() != 0)
      {
        // HAL_Delay(100);
         vTaskDelay(pdMS_TO_TICKS(100)); 
      }
      while (lockDataOutput() != 0)
      {
        // HAL_Delay(100);
         vTaskDelay(pdMS_TO_TICKS(100)); 
      }
    }
  }
  else
  {
    // HAL_Delay(5);
     vTaskDelay(pdMS_TO_TICKS(5)); 
  }
}


void handle_data(sInfo_t *info,uint8_t *pbuf,uint16_t *nowTimeStamp,uint16_t *nowTouch)
{
 
  position = 0;
  
    if ((pbuf[3] == 0x91) && (pbuf[4] == 0x1E))
    {
       (*info).gestureInfo = pbuf[8] | (uint32_t)pbuf[9] << 8 | (uint32_t)pbuf[10] << 16 | (uint32_t)pbuf[11] << 24;
       (*info).touchInfo    = pbuf[12] | (uint32_t)pbuf[13]<<8 | (uint32_t)pbuf[14]<<16 |  (uint32_t)pbuf[15]<<24;
            *nowTimeStamp = (uint32_t)pbuf[14] |  (uint32_t)pbuf[15]<<8;
            *nowTouch = pbuf[12] | (uint32_t)pbuf[13]<<8;
        if(pbuf[7] & 0x02){
         (*info).airWheelInfo = pbuf[16] | (uint32_t)pbuf[17]<<8;
        }
        if(pbuf[7] & 0x01){
          position = 1;
          (*info).xPosition    = pbuf[18] | (uint32_t)pbuf[19]<<8;
           (*info).yPosition    = pbuf[20] | (uint32_t)pbuf[21]<<8;
          (*info).zPosition    = pbuf[22] | (uint32_t)pbuf[23]<<8;
        }
    }
    else if (pbuf[4] == 0x1F)
    {
      while (enableDataOutput() != 0)
      {
        // HAL_Delay(100);
         vTaskDelay(pdMS_TO_TICKS(100)); 
      }
      while (lockDataOutput() != 0)
      {
        // HAL_Delay(100);
         vTaskDelay(pdMS_TO_TICKS(100)); 
      }
    }
}
