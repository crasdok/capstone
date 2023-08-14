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
#include "cmsis_os.h"
#include "adc.h"
#include "fdcan.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TRIG_PIN1 GPIO_PIN_5
#define TRIG_PORT1 GPIOA
#define TRIG_PIN2 GPIO_PIN_14
#define TRIG_PORT2 GPIOD

#define TRIG_PIN4 GPIO_PIN_3
#define TRIG_PORT4 GPIOA
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
FDCAN_FilterTypeDef sFilterConfig;
   FDCAN_TxHeaderTypeDef TxHeader;
   FDCAN_RxHeaderTypeDef RxHeader;
   uint8_t TxData_Node3_To_Node1[16];
   uint8_t TxData_Node3_To_Node2[16];
   uint8_t RxData_From_Node2[8];
   uint8_t RxData_From_Node1[8];
   uint32_t IC_Val1_1 = 0;
   uint32_t IC_Val2_1 = 0;
   uint32_t Difference_1 = 0;
   uint8_t Is_First_Captured_1 = 0;  // is the first value captured ?
   uint8_t Distance_1  = 0;
   uint32_t IC_Val1_2 = 0;
   uint32_t IC_Val2_2 = 0;
   uint32_t Difference_2 = 0;
   uint8_t Is_First_Captured_2 = 0;  // is the first value captured ?
   uint8_t Distance_2  = 0;

   uint32_t IC_Val1_4 = 0;
   uint32_t IC_Val2_4 = 0;
   uint32_t Difference_4 = 0;
   uint8_t Is_First_Captured_4 = 0;  // is the first value captured ?
   uint8_t Distance_4  = 0;
   uint16_t adc1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
void delay (uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while (__HAL_TIM_GET_COUNTER (&htim3) < time);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is channel1
	{
		if (Is_First_Captured_1==0) // if the first value is not captured
		{
			IC_Val1_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
			Is_First_Captured_1 = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured_1==1)   // if the first is already captured
		{
			IC_Val2_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val2_1 > IC_Val1_1)
			{
				Difference_1 = IC_Val2_1-IC_Val1_1;
			}

			else if (IC_Val1_1 > IC_Val2_1)
			{
				Difference_1 = (0xffff - IC_Val1_1) + IC_Val2_1;
			}

			Distance_1 = Difference_1 * .034/2;
			Is_First_Captured_1 = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC1);
		}
	}
//}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)  // if the interrupt source is channel1
	{
		if (Is_First_Captured_2==0) // if the first value is not captured
		{
			IC_Val1_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // read the first value
			Is_First_Captured_2 = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured_2==1)   // if the first is already captured
		{
			IC_Val2_2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val2_2 > IC_Val1_2)
			{
				Difference_2 = IC_Val2_2-IC_Val1_2;
			}

			else if (IC_Val1_2 > IC_Val2_2)
			{
				Difference_2 = (0xffff - IC_Val1_2) + IC_Val2_2;
			}

			Distance_2 = Difference_2 * .034/2 - 40;
			Is_First_Captured_2 = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC2);
		}
	}




	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)  // if the interrupt source is channel1
			{
				if (Is_First_Captured_4==0) // if the first value is not captured
				{
					IC_Val1_4 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
					Is_First_Captured_4 = 1;  // set the first captured as true
					// Now change the polarity to falling edge
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
				}

				else if (Is_First_Captured_4==1)   // if the first is already captured
				{
					IC_Val2_4 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value
					__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

					if (IC_Val2_4 > IC_Val1_4)
					{
						Difference_4 = IC_Val2_4-IC_Val1_4;
					}

					else if (IC_Val1_4 > IC_Val2_4)
					{
						Difference_4 = (0xffff - IC_Val1_4) + IC_Val2_4;
					}

					Distance_4 = Difference_4 * .034/2;
					Is_First_Captured_4 = 0; // set it back to false

					// set polarity to rising edge
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
					__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC4);
				}
			}


}


void HCSR04_Read1 (void)
{
   HAL_GPIO_WritePin(TRIG_PORT1, TRIG_PIN1, GPIO_PIN_SET);  // pull the TRIG pin HIGH
   delay(10);  // wait for 10 us
   HAL_GPIO_WritePin(TRIG_PORT1, TRIG_PIN1, GPIO_PIN_RESET);  // pull the TRIG pin low

   __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1);

}

void HCSR04_Read2 (void)
{
   HAL_GPIO_WritePin(TRIG_PORT2, TRIG_PIN2, GPIO_PIN_SET);  // pull the TRIG pin HIGH
     delay(10);  // wait for 10 us
     HAL_GPIO_WritePin(TRIG_PORT2, TRIG_PIN2, GPIO_PIN_RESET);  // pull the TRIG pin low

     __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC2);
}



void HCSR04_Read4 (void)
{
   HAL_GPIO_WritePin(TRIG_PORT4, TRIG_PIN4, GPIO_PIN_SET);  // pull the TRIG pin HIGH
     delay(10);  // wait for 10 us
     HAL_GPIO_WritePin(TRIG_PORT4, TRIG_PIN4, GPIO_PIN_RESET);  // pull the TRIG pin low

     __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC4);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
   if(FDCAN1 == hfdcan->Instance){
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {


    /* Retreive Rx messages from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData_From_Node2) != HAL_OK)
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
	HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
		HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

		HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FDCAN1_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
//  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);

  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);

  int D1_1,D1_2,D1_3,D2_1,D2_2,D2_3,D3_1,D3_2,D3_3;
  int adc1_1,adc1_2,adc1_3;
  /* USER CODE END 2 */

  /* Init scheduler */
//  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
//  MX_FREERTOS_Init();
  /* Start scheduler */
//  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  adc1 = HAL_ADC_GetValue(&hadc1)/650;

	   HCSR04_Read1();
	   HAL_Delay(100);
	   HCSR04_Read2();
	   HAL_Delay(100);

	   HCSR04_Read4();
	   HAL_Delay(100);

	   D1_1= Distance_1 / 100;
	   D1_2= (Distance_1 %100) /10;
	   D1_3= Distance_1 % 10;
	   D2_1= Distance_2 / 100;
	   D2_2= (Distance_2 %100) /10;
	   D2_3= Distance_2 % 10;
	   D3_1= Distance_4 / 100;
	   D3_2= (Distance_4 %100) /10;
	   D3_3= Distance_4 % 10;
	   adc1_1= adc1 / 100;
	   adc1_2= (adc1 %100) /10;
	   adc1_3= adc1 % 10;
	  sprintf ((char *)TxData_Node3_To_Node2," %d%d%d%d%d%d %d%d%d %d%d%d",D1_1,D1_2,D1_3,D2_1,D2_2,D2_3,D3_1,D3_2,D3_3,adc1_1,adc1_2,adc1_3);
	  	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData_Node3_To_Node2)!= HAL_OK)
	  	{
//	  		  		 Error_Handler();
	  	}
	  	HAL_Delay(100);
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
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);
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
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

