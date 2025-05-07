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
#include "fatfs.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
    eGestureInfo_t gesture_data;
    eTouchInfo_t touch_data;
} SensorData_t;

uint8_t open = 0;

// extern I2C_HandleTypeDef hi2c2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

QueueHandle_t xSensorDataQueue = NULL;

#define mode_default    0
#define mode_Sensor_lcd 1
#define mode_Sensor_PC  2
#define mode_Wifi_send  3
#define mode_Wifi_rec   4
#define mode_Rled       5

uint8_t mode = mode_Sensor_PC;

uint8_t enable_senor = 0;
uint8_t enable_wifi  = 0;
uint8_t enable_Rled  = 0;

extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;
extern uint8_t function1;
extern uint8_t function2;
extern uint8_t function3;
extern uint8_t function4;
extern uint8_t page;
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
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for switch_window */
osThreadId_t switch_windowHandle;
const osThreadAttr_t switch_window_attributes = {
  .name = "switch_window",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for myTask04 */
osThreadId_t myTask04Handle;
const osThreadAttr_t myTask04_attributes = {
  .name = "myTask04",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for myTask05 */
osThreadId_t myTask05Handle;
const osThreadAttr_t myTask05_attributes = {
  .name = "myTask05",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTask04(void *argument);
void StartTask05(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

    ILI9341_Init();
    XPT2046_Init();
    MX_FATFS_Init();
    MX_USART1_UART_Init();

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

    xSensorDataQueue = xQueueCreate(1, sizeof(SensorData_t));

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of switch_window */
  switch_windowHandle = osThreadNew(StartTask02, NULL, &switch_window_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(StartTask03, NULL, &myTask03_attributes);

  /* creation of myTask04 */
  myTask04Handle = osThreadNew(StartTask04, NULL, &myTask04_attributes);

  /* creation of myTask05 */
  myTask05Handle = osThreadNew(StartTask05, NULL, &myTask05_attributes);

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

    SensorData_t receivedData;
  
    for (;;) {      
                
            if (xQueueReceive(xSensorDataQueue, &receivedData, portMAX_DELAY) == pdTRUE) {
                    ILI9341_DispString_EN(80, 0, "yes");
        
                if (mode == mode_Sensor_lcd) {

                    switch (receivedData.gesture_data) {
                        case eFilckR:
                            /* code */
                             ILI9341_DispString_EN(100, 0, "r");

                            gesture_dir_arrow("right.bin");

                            break;
                        case eFilckL:
                            /* code */
                            // ILI9341_DispString_EN(0, 0, "l");

                            gesture_dir_arrow("left.bin");
                            break;
                        case eFilckU:
                            /* code */
                            // ILI9341_DispString_EN(0, 0, "u");

                            gesture_dir_arrow("up.bin");
                            break;
                        case eFilckD:
                            /* code */
                            // ILI9341_DispString_EN(0, 0, "d");

                            gesture_dir_arrow("down.bin");
                            break;
                        default:
                            break;
                    }
                    switch (receivedData.touch_data) {
                        case eTouchCenter:
                            /* code */
                            ILI9341_DispString_EN(0, 20, "c");

                            break;
                        case eTapCenter:
                            ILI9341_DispString_EN(0, 20, "e");
                            break;
                        case eTouchDown:
                            /* code */
                            ILI9341_Clear(0, 20, 8, 16);
                            break;
                        case eTouchUp:
                            /* code */

                            break;
                        case eTouchLeft:
                            /* code */

                            break;
                        case eTouchRight:

                            break;
                        default:

                            break;
                    }
                }
                if (mode == mode_Sensor_PC) {
                    switch (receivedData.gesture_data) {
                        case 2:
                            /* code */
                            ILI9341_DispString_EN(0, 0, "r");
                            send_data("2");
                            break;
                        case 3:
                            /* code */
                            ILI9341_DispString_EN(0, 0, "l");
                            send_data("1");
                            break;
                        case 4:
                            /* code */
                            ILI9341_DispString_EN(0, 0, "u");
                            send_data("3");
                            break;
                        case 5:
                            /* code */
                            ILI9341_DispString_EN(0, 0, "d");
                            send_data("4");
                            break;
                        default:
                            break;
                    }

                  
                    // info.gestureInfo        = 0;
                }
            }
        

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
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
    sInfo_t info;
    uint16_t lastTimeStamp;
    uint16_t nowTimeStamp;
    uint16_t lastTouch;
    uint16_t nowTouch;
    SensorData_t sensorData;

    // open MGC3X30
    begin();

        for (;;) {
            if ((mode == mode_Sensor_PC) || (mode == mode_Sensor_lcd)) {
                sensorDataRecv(&info, &nowTimeStamp, &nowTouch);
                sensorData.touch_data   = getTouchInfo(&info, &lastTimeStamp, &nowTimeStamp, &lastTouch, &nowTouch);
                sensorData.gesture_data = getGestureInfo(&info);
                
                xQueueSend(xSensorDataQueue, &sensorData, portMAX_DELAY); // 发送数据到任务1
                sensorData.gesture_data = 0;
                sensorData.touch_data   = 0;
              
                // info.gestureInfo        = 0;
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }

  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief Function implementing the ESP_tsak thread.
 * @param argument: Not used
 * @retval None
 */

/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
    /* Infinite loop */

    for (;;) {

        if (function2 == 1) {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
            ESP8266_Config();
            ILI9341_DispString_EN(120, 160, "wifiopen");
            function2=3;
          
        }

      
        osDelay(1);
    }

  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
 * @brief Function implementing the myTask04 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
  /* USER CODE BEGIN StartTask04 */

    /* Infinite loop */
    for (;;) {
        XPT2046_TouchEvenHandler();
        // ILI9341_DispChar_EN(80,0,mode);
        osDelay(35);
    }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
 * @brief Function implementing the myTask05 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask05 */
void StartTask05(void *argument)
{
  /* USER CODE BEGIN StartTask05 */
    /* Infinite loop */
    for (;;) {
        // 判断现在是不是主�?
        if (page == 0) {
    
            main_page();
        } else {

            if(mode==mode_Sensor_lcd){
                show_page();
            }else{
                 gesture_page();

            }
           
        
        
        }

        osDelay(1);
    }
  /* USER CODE END StartTask05 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

