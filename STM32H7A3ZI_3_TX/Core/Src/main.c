/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "fdcan.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tfluna_i2c.h"
#include "stdio.h"
#include <stdbool.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE{
   HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, 0xFFFF);
   return ch;
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
FDCAN_FilterTypeDef sFilterConfig;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
FDCAN_FilterTypeDef sFilterConfig;
   FDCAN_TxHeaderTypeDef TxHeader;
   FDCAN_RxHeaderTypeDef RxHeader;
   uint8_t TxData_Node1_To_Node3[16];
   uint8_t TxData_Node3_To_Node2[16];
   uint8_t RxData_From_Node2[16];
   uint8_t RxData_From_Node3[16];

//int _write(int file, char *ptr, int len)
//{
//  /* Implement your write code here, this is used by puts and printf for example */
//  int i=0;
//  for(i=0 ; i<len ; i++)
//    ITM_SendChar((*ptr++));
//  return len;
//}

int _write(int file, unsigned char * p, int len){
	HAL_UART_Transmit(&huart3, p, len, 10);
	return len;
}

TF_Luna_Lidar TF_Luna_1;

int16_t  tfAddr = TFL_DEF_ADR;    // default I2C address
uint16_t tfFrame = TFL_DEF_FPS;   // default frame rate
int16_t  tfDist = 0 ;   // distance in centimeters

/* multi devices example:
 * Maybe 5 devices
 * int16_t tfDist[5]=0;
 *
 *
 *You should define other variables this way as well.
 *Example : tfFlux[5]=0;
 */


int16_t  tfFlux = 0 ;   // signal quality in arbitrary units
int16_t  tfTemp = 0 ;   // temperature in 0.01 degree Celsius

// other device variables
uint16_t tfTime = 0;    // devie clock in milliseconds

uint8_t  tfVer[3];      // device version number
/*multi devices example:
 * uint8_t  tfVer1[3];
 * uint8_t  tfVer2[3];
 * uint8_t  tfVer3[3];
 * uint8_t  tfVer4[3];
 */

uint8_t  tfCode[14];    // device serial number

// sub-loop counter for Time display
uint8_t tfCount = 0;

// FDCAN1 Callback
//void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
//{
//
//  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
//  {
//    /* Retreive Rx messages from RX FIFO0 */
//    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader1, RxData_From_Node1) != HAL_OK)
//    {
//    /* Reception Error */
//    Error_Handler();
//    }
//
//    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
//    {
//      /* Notification Error */
//      Error_Handler();
//    }
//  }
//}
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
//    printf("%s\r\n",);

//    HAL_Delay (1000);
   if(FDCAN1 == hfdcan->Instance){
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {

//       HAL_Delay (1000);
    /* Retreive Rx messages from RX FIFO0 */
     //?  기서?    ?????? ? RX FIFO0?   ?  ?   ?   ?????? ?? RxHeader ??????? 복사?   ??????? ?  ?  ?   ??????? RxData 배열 ??????? 복사?  ?  ?  .
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData_From_Node3) != HAL_OK)
    {
    /* Reception Error */
    Error_Handler();
    }
   }
   }

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  TF_Luna_init(&TF_Luna_1, &hi2c1, 0x10);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */


    /* multi devices example:
     *   If you want to connect more than one device,
     *   you must first connect them one by one and change their slave addresses.
     *   Each device must have a different address.
     *
     *   TF_Luna_init(&TF_Luna_2, &hi2c1, 0x11); // random address.
     *   TF_Luna_init(&TF_Luna_3, &hi2c1, 0x12);
     *   TF_Luna_init(&TF_Luna_4, &hi2c1, 0x13);
     *   TF_Luna_init(&TF_Luna_5, &hi2c1, 0x14);
     */
  int Dist1,Dist2,Dist3;


          Get_Frame_Rate( &TF_Luna_1,&tfFrame);
          Get_Firmware_Version(&TF_Luna_1, tfVer);
          Get_Prod_Code(&TF_Luna_1, tfCode);
          Get_Time(&TF_Luna_1, &tfTime);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
  	  getData(&TF_Luna_1, &tfDist, &tfFlux, &tfTemp);
  //	  printDataArray();
  //	  printf("%d",tfDist); // Open your SVW ITM Data Console
  	  	  HAL_Delay(100); // 250Hz
  	  if(tfDist > 200)
  	  {
  		  tfDist = 200;
  	  }
  	  Dist1= (tfDist-15) / 100;
  	  Dist2= ((tfDist-15) %100) /10;
  	  Dist3= (tfDist-15) % 10;
//  	memset(TxData_Node1_To_Node3, 0, sizeof(TxData_Node1_To_Node3));
//  	  	sprintf ((char *)TxData_Node1_To_Node3 + 2,"%d%d%d",Dist1,Dist2,Dist3);
  	sprintf ((char *)TxData_Node1_To_Node3 ,"%d%d%d",Dist1,Dist2,Dist3);
  //	  	sprintf ((char *)TxData_Node1_To_Node3, "1234");
  //	  	"%d",tfDist
  	  		  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData_Node1_To_Node3)!= HAL_OK)
  	  		              {
  //	  		                 Error_Handler();
  	  		              }
//	  		HAL_Delay(100);
//	  		     HAL_Delay (100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 32;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

