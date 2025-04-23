/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */




/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void send_AT_command(const char *cmd) {
    HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
}

void ESP8266_AutoConfig() {
    // ??? AP ??
    send_AT_command("AT+CWMODE=2");
    // ??????
    send_AT_command("AT+CWSAP=\"ESP_Ethan\",\"12345678\",5,3");
    // ?????
    send_AT_command("AT+CIPMUX=1");
    // ?? TCP ???
    send_AT_command("AT+CIPSERVER=1,8080");
}




  

    




// void send_to_Usart3(char *cmd)
// {
//     HAL_USART_Transmit(&husart3, (uint8_t *)cmd, strlen(cmd), 0xFFFF);
//     while (HAL_USART_GetState(&husart3) == HAL_USART_STATE_BUSY_RX); // �?测USART坑�?�结�?
// }

// void show_Usart3_Message()
// {
//     HAL_USART_Transmit(&husart1, (uint8_t *)&RxBuffer3, USART3_Rx_Cnt, 0xFFFF); // 将收到的信杯坑�?�出�?
//     while (HAL_USART_GetState(&husart1) == HAL_USART_STATE_BUSY_TX);            // �?测USART坑�?�结�?
//     USART3_Rx_Cnt = 0;
//     memset(RxBuffer3, 0x00, sizeof(RxBuffer3)); // 清空数组
// }

// void HAL_USART_RxCpltCallback(USART_HandleTypeDef *hUSART)
// {
//     if (hUSART->Instance == USART1) {
//         if (USART1_Rx_Cnt >= (RXBUFFERSIZE - 1)) { // 溢出判断
//             USART1_Rx_Cnt = 0;
//             memset(RxBuffer1, 0x00, sizeof(RxBuffer1));
//             HAL_USART_Transmit(&husart1, (uint8_t *)"数杮溢出", 10, 0xFFFF);
//         } else {
//             RxBuffer1[USART1_Rx_Cnt++] = aRxBuffer1; // 接收数杮转存
//             // HAL_USART_Transmit(&husart3, (uint8_t *)test, sizeof(test),0xFFFF);

//             /*
//             if((RxBuffer1[USART1_Rx_Cnt-1] == 0x0A)&&(RxBuffer1[USART1_Rx_Cnt-2] == 0x0D)){ //判断结束�?
//                 HAL_USART_Transmit(&husart3, (uint8_t *)&RxBuffer1, USART1_Rx_Cnt,0xFFFF); //将收到的信杯坑�?�出�?
//                 while(HAL_USART_GetState(&husart3) == HAL_USART_STATE_BUSY_TX);//�?测USART坑�?�结�?
//                 USART1_Rx_Cnt = 0;
//                 memset(RxBuffer1,0x00,sizeof(RxBuffer1)); //清空数组
//             }
//             */
//         }
//         HAL_USART_Receive_IT(&husart1, (uint8_t *)&aRxBuffer1, 1); // 冝开坯接收中�?
        
//     }
//     if (hUSART->Instance == USART3) {
//         if (USART3_Rx_Cnt >= (RXBUFFERSIZE - 1)) { // 溢出判断
//             USART3_Rx_Cnt = 0;
//             memset(RxBuffer3, 0x00, sizeof(RxBuffer3));
//             HAL_USART_Transmit(&husart1, (uint8_t *)"数杮溢出", 10, 0xFFFF);
//         } else {
//             RxBuffer3[USART3_Rx_Cnt++] = aRxBuffer3; // 接收数杮转存
//             /*
//             if(((RxBuffer3[USART3_Rx_Cnt-1] == 0x0A)&&(RxBuffer3[USART3_Rx_Cnt-2] == 0x0D))){ //判断结束�?
//                 HAL_USART_Transmit(&husart1, (uint8_t *)&RxBuffer3, USART3_Rx_Cnt,0xFFFF); //将收到的信杯坑�?�出�?
//                 while(HAL_USART_GetState(&husart1) == HAL_USART_STATE_BUSY_TX);//�?测USART坑�?�结�?
//                 USART3_Rx_Cnt = 0;
//                 memset(RxBuffer3,0x00,sizeof(RxBuffer3)); //清空数组
//             }
//             */
//         }
//         HAL_USART_Receive_IT(&husart3, (uint8_t *)&aRxBuffer3, 1); // 冝开坯接收中�?
//     }
// }



    // HAL_Delay(2000);
    // show_Usart3_Message();

    // char cmd1[] = "AT+CWLAP\r\n";
    // send_to_Usart3(cmd1);
    // HAL_Delay(10000);
    // show_Usart3_Message();

    // char cmd2[] = "AT+CWJAP=\"Redmi K30\",\"lpnb6666\"\r\n";
    // send_to_Usart3(cmd2);
    // HAL_Delay(20000);
    // show_Usart3_Message();

    // char cmd4[] = "AT+CIPMUX=0\r\n";
    // send_to_Usart3(cmd4);
    // HAL_Delay(3000);
    // show_Usart3_Message();

    // char cmd5[] = "AT+CIPMODE=1\r\n";
    // send_to_Usart3(cmd5);
    // HAL_Delay(3000);
    // show_Usart3_Message();

    // char cmd3[] = "AT+CIFSR\r\n";
    // send_to_Usart3(cmd3);
    // HAL_Delay(3000);
    // show_Usart3_Message();

    // send_to_Usart3("AT+CIPSTART=\"TCP\",\"192.168.43.193\",8899\r\n");
    // HAL_Delay(10000);
    // show_Usart3_Message();

    // send_to_Usart3("AT+CIPSEND\r\n");
    // HAL_Delay(3000);
    // show_Usart3_Message();

    // send_to_Usart3(">\r\n");
    // HAL_Delay(3000);
    // show_Usart3_Message();

    // send_to_Usart3("lp\r\n");
    // HAL_Delay(3000);
    // show_Usart3_Message();

    // send_to_Usart3("123456\r\n");
    // HAL_Delay(3000);
    // show_Usart3_Message();

    // send_to_Usart3("hello everyone\r\n");
    // HAL_Delay(3000);
    // show_Usart3_Message();

    // send_to_Usart3("+++");
    // HAL_Delay(3000);
    // show_Usart3_Message();

/* USER CODE END 1 */
