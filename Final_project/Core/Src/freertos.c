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
#include "queue.h"
#include "semphr.h"
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
    .name       = "defaultTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
    .name       = "myTask02",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityLow2,
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
void MX_FREERTOS_Init(void)
{
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
    for (;;) {
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

 // 全局定义信号量和队列
SemaphoreHandle_t xI2C_CompleteSem = NULL;
QueueHandle_t xSensorDataQueue = NULL;

// 定义传感器数据结构
typedef struct {
    uint16_t gesture_data;
    uint16_t touch_data;
} SensorData_t;

uint8_t pbuf[24];

extern I2C_HandleTypeDef hi2c1;
sInfo_t MGC_info = {0};
reset_info(&MGC_info);
uint16_t lastTimeStamp;
uint16_t nowTimeStamp;
uint16_t lastTouch;
uint16_t nowTouch;
uint8_t pbuf[24];

// 初始化函数
void System_Init(void) {
    // 创建信号量
    xI2C_CompleteSem = xSemaphoreCreateBinary();
    if (xI2C_CompleteSem == NULL) {
        Error_Handler();  // 信号量创建失败
    }

    // 创建队列
    xSensorDataQueue = xQueueCreate(5, sizeof(SensorData_t));
    if (xSensorDataQueue == NULL) {
        Error_Handler();  // 队列创建失败
    }
}

// I2C 接收完成回调函数
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // 释放信号量
    xSemaphoreGiveFromISR(xI2C_CompleteSem, &xHigherPriorityTaskWoken);

    // 数据解析
    SensorData_t sensorData;
    handle_data(&MGC_info,pbuf,&nowTimeStamp,&nowTouch);
    sensorData.gesture_data=getGestureInfo(&MGC_info);
    sensorData.touch_data=getTouchInfo(&MGC_info,&lastTimeStamp,&nowTimeStamp,&lastTouch,&nowTouch);

    // 发送到队列
    if (xQueueSendFromISR(xSensorDataQueue, &sensorData, &xHigherPriorityTaskWoken) != pdPASS) {
        Error_Handler();  // 队列满
    }

    // 启动下一次接收
    HAL_I2C_Master_Receive_IT(&hi2c1, 0x85, pbuf, 24);

    // 触发上下文切换
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


// 主任务
void StartTask02(void *argument) {
    System_Init();
    // 启动 I2C 接收
    HAL_I2C_Master_Receive_IT(&hi2c1, 0x85, pbuf, 24);
    vTaskDelay(pdMS_TO_TICKS(500));
    // 任务进入空闲状态
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(100));  // 空转
    }
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
