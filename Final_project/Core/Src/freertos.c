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
#include "queue.h"
#include "semphr.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
    uint8_t gesture_data;
    uint8_t touch_data;
} SensorData_t;

volatile uint8_t i2c_rx_done = 0; // 接收完成标志�?
extern I2C_HandleTypeDef hi2c2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
SemaphoreHandle_t xI2C_CompleteSem = NULL;
QueueHandle_t xSensorDataQueue     = NULL;

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

    // xI2C_CompleteSem = xSemaphoreCreateBinary();
    // if (xI2C_CompleteSem == NULL) {
    //     Error_Handler(); // 信号量创建失�?????
    // }

    // // 创建队列
    xSensorDataQueue = xQueueCreate(5, sizeof(SensorData_t));
    // if (xSensorDataQueue == NULL) {
    //     Error_Handler(); // 队列创建失败
    // }

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
    //char X_data[10];

    //  SensorData_t receivedData;
    uint16_t rec_Data;
    for (;;) {
        if (xQueueReceive(xSensorDataQueue, &rec_Data, portMAX_DELAY) == pdPASS) {
           
            LCD_DrawString(70, 0, "in");
            // if (rec_Data) {
            //     sprintf(X_data, "%d", rec_Data);
            //     LCD_DrawString_promax(16, 20, X_data, 5);
            // }

            switch (rec_Data) {
                case 2:
                    /* code */
                    LCD_DrawString(0, 0, "r");
                    break;
                case 3:
                    /* code */
                    LCD_DrawString(0, 0, "l");
                    break;
                case 4:
                    /* code */
                    LCD_DrawString(0, 0, "u");
                    break;
                case 5:
                    /* code */
                    LCD_DrawString(0, 0, "d");
                    break;

                default:
                    break;
            }.rawString_promax(0, 0, data, 5);
        //     sprintf(data, "%d", rxData.touch_data);
        //     LCD_DrawString_promax(0, 20, data, 5);
        // }
        /* Infinite loop */

        /* USER CODE END StartDefaultTask */
    }
}
/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the myTask02 thread.
 * @param argument: Not used
 * @retval None
 */

sInfo_t info;
uint16_t lastTimeStamp;
uint16_t nowTimeStamp;
uint16_t lastTouch;
uint16_t nowTouch;
uint8_t pbuf[24];

// void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef * hi2c)
// {

//     if (hi2c->Instance == I2C2) // 确保是正确的 I2C 外设
//     {
//         i2c_rx_done = 1;
//         LCD_DrawString(0, 20, "in");
//         HAL_I2C_Master_Receive_IT(&hi2c2, 0x85, pbuf, 24);
//         LCD_DrawString(0, 160, "no");
//     }
// }

/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
    /* USER CODE BEGIN StartTask02 */
    // char X_data[10];
    uint16_t sen_data;
    begin();

    for (;;) {
        sensorDataRecv(&info, &nowTimeStamp, &nowTouch);
        //sen_data = getTouchInfo(&info, &lastTimeStamp, &nowTimeStamp, &lastTouch, &nowTouch);
        // sensorData.touch_data   = getTouchInfo(&info, &lastTimeStamp, &nowTimeStamp, &lastTouch, &nowTouch);
        // sensorData.gesture_data = getGestureInfo(&info);
        sen_data = getGestureInfo(&info);
        xQueueSend(xSensorDataQueue, &sen_data, portMAX_DELAY);

        //   vTaskDelay(pdMS_TO_TICKS(10));
        // if (sensorData.touch_data) {
        //     sprintf(X_data, "%d", sensorData.touch_data);
        //     LCD_DrawString_promax(16, 20, X_data, 10);
        // }
        // if (info.xPosition) {
        //     sprintf(X_data, "%d", info.xPosition);
        //     LCD_DrawString_promax(16, 40, X_data, 10);
        // }
        // if (info.yPosition) {
        //     sprintf(X_data, "%d", info.yPosition);
        //     LCD_DrawString_promax(16, 60, X_data, 10);
        // }
        // if (info.zPosition) {
        //     sprintf(X_data, "%d", info.zPosition);
        //     LCD_DrawString_promax(16, 80, X_data, 10);
        // }
    }

    // if (HAL_I2C_Master_Receive_IT(&hi2c2, 0x85, pbuf, 24) != HAL_OK) {
    //     LCD_DrawString(0, 40, "error");
    // }

    // for (;;) {
    //     if (i2c_rx_done) {
    //         i2c_rx_done = 0;
    //         // 处理接收到的数据
    //         LCD_DrawString(0, 60, "here");
    //         handle_data(&info, pbuf, &nowTimeStamp, &nowTouch);
    //         sensorData.touch_data = getTouchInfo(&info, &lastTimeStamp, &nowTimeStamp, &lastTouch, &nowTouch);
    //         printf(X_data, "%d", sensorData.touch_data);
    //         LCD_DrawString_promax(50, 80, X_data, 5);

    //         // if ((pbuf[3] == 0x91) && (pbuf[4] == 0x1E)) {
    //         //     LCD_DrawString(0, 110, "cor");
    //         // }
    //     }
    //     vTaskDelay(pdMS_TO_TICKS(100)); // 空转
    // }
    /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
