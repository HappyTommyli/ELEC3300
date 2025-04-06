/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern I2C_HandleTypeDef hi2c1;


xSemaphoreHandle xI2C_CompleteSem;  // I2C传输完成信号量
xQueueHandle xSensorDataQueue;      // 传感器数据队列

 xI2C_CompleteSem = xSemaphoreCreateBinary();
 xSensorDataQueue = xQueueCreate(5, sizeof(SensorData_t));  // 队列深度5

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow2,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */



  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */


  LCD_INIT();

  /* Infinite loop */
  for(;;)
  { 
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {


    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // 释放信号量（注意使用FromISR版本）
    xSemaphoreGiveFromISR(xI2CSemaphore, &xHigherPriorityTaskWoken);
    
    // 必要时触发上下文切换
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
   typedef struct {
  uint16_t gesture_data;
  uint16_t touch_data;
} SensorData_t;

 
 
 
  sInfo_t MGC_info = {0};
  uint16_t lastTimeStamp;
  uint16_t nowTimeStamp;
  uint16_t lastTouch;
  uint16_t nowTouch;
  uint8_t pbuf[24];

  SensorData_t sensorData;

  
  const TickType_t xTimeout = pdMS_TO_TICKS(200);  // 信号量等待超时200ms

  
  begin();
   
  HAL_I2C_Master_Receive_IT(&hi2c1, 0x85, pbuf, 24, HAL_MAX_DELAY);

  while(1) {
        // 等待I2C传输完成信号量
        if(xSemaphoreTake(xI2C_CompleteSem, xTimeout) == pdTRUE) {
            // 数据解析
            handle_data(&MGC_info,pbuf,&nowTimeStamp,&nowTouch);

            // 发送到LCD任务（带队列保护）
            if(xQueueSend(xSensorDataQueue, &sensorData,10) != pdPASS) {
                // 队列满处理：可记录错误或丢弃数据
                Error_Handler();
            }
            
            // 立即重启下一次接收
             HAL_I2C_Master_Receive_IT(&hi2c1, 0x85, pbuf, 24, HAL_MAX_DELAY);

        } else {
            // 超时处理：可能是I2C总线挂死
            // I2C_Recovery();  // 自定义总线恢复函数
            vTaskDelay(pdMS_TO_TICKS(100));  // 等待恢复
            HAL_I2C_Master_Receive_IT(&hi2c1, 0x85, pbuf, 24, HAL_MAX_DELAY);
        }
    }
  
  
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

