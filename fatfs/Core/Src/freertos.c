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


uint8_t open                 = 0;

// extern I2C_HandleTypeDef hi2c2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

QueueHandle_t xSensorDataQueue     = NULL;


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
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myTask04 */
osThreadId_t myTask04Handle;
const osThreadAttr_t myTask04_attributes = {
  .name = "myTask04",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTask04(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  ILI9341_Init();
  operating_window_Init();
  XPT2046_Init();

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

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(StartTask03, NULL, &myTask03_attributes);

  /* creation of myTask04 */
  myTask04Handle = osThreadNew(StartTask04, NULL, &myTask04_attributes);

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
    // uint16_t rec_Data;

    for (;;) {
        if (mode == mode_Sensor_lcd) {           
            if (xQueueReceive(xSensorDataQueue, &receivedData, portMAX_DELAY) == pdTRUE) {
                ILI9341_DispString_EN(70, 0, "sensormode_in");
                switch (receivedData.gesture_data) {
                    case eFilckR:
                        /* code */
                        ILI9341_DispString_EN(0, 0, "r");
                        break;
                    case eFilckL:
                        /* code */
                        ILI9341_DispString_EN(0, 0, "l");
                        break;
                    case eFilckU:
                        /* code */
                        ILI9341_DispString_EN(0, 0, "u");
                        break;
                    case eFilckD:
                        /* code */
                        ILI9341_DispString_EN(0, 0, "d");
                        break;
                    default:
                        break;
                }
                switch (receivedData.touch_data) {
                    case eTouchCenter:
                        /* code */
                        ILI9341_DispString_EN(0,20,"c");

                        break;
                    case  eTapCenter:
                        ILI9341_DispString_EN(0,20,"e");     
                        break;    
                    case eTouchDown:
                        /* code */
                        ILI9341_Clear(0,20,8,16);
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
   
    
    //open MGC3X30
    begin();

    for (;;) {

        if ((mode == mode_Sensor_PC)||(mode == mode_Sensor_lcd)) {
            sensorDataRecv(&info, &nowTimeStamp, &nowTouch);
            sensorData.touch_data   = getTouchInfo(&info, &lastTimeStamp, &nowTimeStamp, &lastTouch, &nowTouch);
            sensorData.gesture_data = getGestureInfo(&info);
            if (mode == mode_Sensor_lcd) {

                xQueueSend(xSensorDataQueue, &sensorData, portMAX_DELAY);
                sensorData.gesture_data = 0;
                sensorData.touch_data = 0;
                //info.gestureInfo        = 0;
                vTaskDelay(pdMS_TO_TICKS(100));
            }


            if (mode == mode_Sensor_PC) {
                switch (sensorData.gesture_data) {
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

                sensorData.gesture_data = 0;
                //info.gestureInfo        = 0;
            }
        }
    }


    for (;;) { vTaskDelay(pdMS_TO_TICKS(100)); }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief Function implementing the ESP_tsak thread.
 * @param argument: Not used
 * @retval None
 */

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     static int index = 0;
//     // vTaskDelay(pdMS_TO_TICKS(100));
//     // vTaskDelay(pdMS_TO_TICKS(100));
//     if (huart->Instance == USART3) {
//         sprintf(X_data, "%d", index);
//         ILI9341_DispString_EN_promax(0, 60, X_data, 3);
//         ILI9341_DispString_EN(0, 40, "in");
//         ESP_data_received = 1;
//         // for(uint8_t i=0;i<3;++i){
//         //   sprintf(X_data, "%u",ESP_rx_buffer[i]);
//         //  ILI9341_DispString_EN_promax(i*24, 120, X_data, 3);
//         // }
//         ILI9341_Clear(0, 120, 8 * 25, 16, WHITE);
//         sprintf(X_data, "%u", ESP_rx_buffer[0]);
//         ILI9341_DispString_EN_promax(0, 120, X_data, 3);
//         index++;
//         // if (ESP_rx_buffer[7] == 1) {
//         //     ILI9341_DispString_EN(0, 40, "yes");
//         // }
//         memset(ESP_rx_buffer, 0, 1);
//         HAL_UART_Receive_IT(&huart1, ESP_rx_buffer, 3);
//     }
// }

/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
    /* Infinite loop */

    // Open Esp8266


    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    // ESP8266_Config();
    //  HAL_UART_Receive_IT(&huart1, ESP_rx_buffer,8);
    for (;;) {
        // if (mode_Sensor_PC) {
       
        // }
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
  FIL text;
  DWORD fileSize;  // 文件大小，方便验证
  uint8_t buff[BUFF_TOTAL_BYTE];
  uint8_t res=0, x=0;	
  uint16_t xstr=INIT_X,ystr=INIT_Y;
  UINT brr;
  uint32_t move=0;


  /* Infinite loop */
  for(;;)
  {
    res = f_open(&text, "down.bin", FA_READ);
        if (res == FR_OK)
        {
            printf("ok\n");
            // 文件打开成功，进行读取操作.
            fileSize = f_size(&text);
            printf("File size: %lu bytes\n", (unsigned long)fileSize);
            while (1)
            {
                for (x = 0; x < (TOTAL_BYTE % BUFF_TOTAL_BYTE ? (TOTAL_BYTE / BUFF_TOTAL_BYTE) + 1 : (TOTAL_BYTE / BUFF_TOTAL_BYTE)); x++)
                {
                    f_lseek(&text, move);
                    res = f_read(&text, buff, BUFF_TOTAL_BYTE, &brr);
                    LCD_Draw_Picture_Pro(&xstr, &ystr, brr, buff);
                    move += brr;
                }
                //printf("xstr=%d,ystr=%d\n",xstr,ystr);
                //printf("move=%d\n",move);
                if (brr < BUFF_TOTAL_BYTE) break;
            }

            f_close(&text);  // 关闭文件
            move = 0;//复位

        }
        else {
            // 文件打开失败，进行错误处理
            printf("no ok=%d\n", res);
            // ...
        }    
    osDelay(1);
  }
  /* USER CODE END StartTask04 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

