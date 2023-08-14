/*
 * motor.c
 *
 *  Created on: 2023. 8. 7.
 *      Author: sc110
 */

#include "motor.h"
#include "main.h"


enum notes {
  C = 956,
  D = 852,
  E = 758,
  F = 716,
  G = 638
};
void go_back(void){
		if(RxData[2]==0)
			  {
			  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 0);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,0);
			  htim1.Instance->CCR1=RxData[0];
			  htim1.Instance->CCR2=RxData[0];
			  if(RxData[0]>=100)
				  {
					  htim1.Instance->CCR1=99;
					  htim1.Instance->CCR2=99;
				  }
			  light_sensor();
			 // ridar();
			  }
		if(RxData[2]==1)
				{
			    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 1);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,1);
				htim1.Instance->CCR1=RxData[0];
			    htim1.Instance->CCR2=RxData[0];
				if(RxData[0]>=100)
					{
						htim1.Instance->CCR1=99;
						htim1.Instance->CCR2=99;
				    }
				light_sensor();
			//	ridar();
			  }
}

void nrf_motor (void){
	if(RxData[3]==0)
	{
			  if (RxData[1]  <50)
				  {
					  while (RxData[1]  < 50)
					  {

						  light_sensor();
						       buzzer();
						  if (isDataAvailable(2) == 1)
						  {
							  NRF24_Receive(RxData);
						  }
						  htim1.Instance->CCR3 = 80;
						  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
						  HAL_Delay(100);
						  go_back();

						  if(50<=RxData[1]&&RxData[1]<=65)
						  {
							  htim1.Instance->CCR3 = 100;
							  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
							  HAL_Delay(300);
							  htim1.Instance->CCR3 = 0;
						  }
					  }
				  }
			  if (RxData[1] > 65)
					  {
						  while (RxData[1] > 65)
						  {
							  light_sensor();
							       buzzer();
							  if (isDataAvailable(2) == 1)
							  {
								  NRF24_Receive(RxData);
							  }
							  htim1.Instance->CCR3 = 80;
							  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
							  HAL_Delay(100);
							  go_back();
							  if(50<=RxData[1]&&RxData[1]<=65)
							  {
								  htim1.Instance->CCR3 = 90;
								  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
								  HAL_Delay(250);
								  htim1.Instance->CCR3 = 0;
							  }

						  }
					  }


	  }
}
int executedOnce_l = 0;

void rpi_motor (void){
	  if(RxData[3]==1)
	  {

/*		  if (isDataAvailable(2) == 1)
		  {NRF24_Receive(RxData);}
		  light_sensor();
		  buzzer();
		  go_back();*/
/*					  while (RxData_From_Node4[0]=='l')
					  {
						  htim1.Instance->CCR3 = 80;
						  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
						  HAL_Delay(50);
						  while (RxData_From_Node4[0]=='L')
						  {
							  htim1.Instance->CCR3 = 100;
							  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
							  HAL_Delay(50);
							  htim1.Instance->CCR3 = 0;
							  if(RxData_From_Node4[0]=='l')
							  {
								  htim1.Instance->CCR3 = 100;
								  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
								  HAL_Delay(50);
								  htim1.Instance->CCR3 = 0;
								break;
							  }
						  }
						  if(RxData_From_Node4[0]=='G')
						  {
							  htim1.Instance->CCR3 = 100;
							  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
							  HAL_Delay(50);
							  htim1.Instance->CCR3 = 0;
							break;
						  }
					   }*/
/*					if (RxData_From_Node4[0]=='r' && !executedOnce_l)
						{
							  htim1.Instance->CCR3 = 35;
							  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
							  HAL_Delay(10);
							  executedOnce_l = 1;
						 }
				    if (RxData_From_Node4[0] != 'r') {
				        executedOnce_l = 0;  // 'r' 값이 아닐 때 executedOnce 재설정

				    }
				    while (RxData_From_Node4[0]=='r')
				    		{
									  if (RxData_From_Node4[0]=='G')
									  {
												  htim1.Instance->CCR3 = 30;
												  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
												  HAL_Delay(5);
												  break;
									  }
									  while (RxData_From_Node4[0]=='R')
									  {
												  htim1.Instance->CCR3 = 45;
												  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
												  HAL_Delay(10);
												  if(RxData_From_Node4[0]=='r')
												  {
													  htim1.Instance->CCR3 = 25;
													  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
													  HAL_Delay(0);
													  break;
												  }

									  }

				    		}*/
		  while (RxData_From_Node4[0]=='L')
		  {
			  light_sensor();
			       buzzer();
			  if (isDataAvailable(2) == 1)
			  {
				  NRF24_Receive(RxData);
			  }
			  htim1.Instance->CCR3 = 80;
			  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
			  HAL_Delay(100);
			  go_back();
			  if(RxData[3]==0)
			  {
				  break;
			  }
			  if(RxData_From_Node4[0]=='G')
			  {
				  htim1.Instance->CCR3 = 100;
				  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
				  HAL_Delay(200);
				  htim1.Instance->CCR3 = 0;
				break;
			  }

		  }
//80 100 80 90

			  while (RxData_From_Node4[0]=='R')
			  {
					   light_sensor();
				       buzzer();
				  if (isDataAvailable(2) == 1)
				  {
					  NRF24_Receive(RxData);
				  }
				  htim1.Instance->CCR3 = 80;
				  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
				  HAL_Delay(100);
				  go_back();
				  if(RxData[3]==0)
				  {
					  break;
				  }
				  if(RxData_From_Node4[0]=='G')
				  {
					  htim1.Instance->CCR3 = 90;
					  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
					  HAL_Delay(200);
					  htim1.Instance->CCR3 = 0;
					break;
				  }


			  }


/*							  while (RxData_From_Node4[0]=='R')
							  {
								  htim1.Instance->CCR3 = 100;
								  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
								  HAL_Delay(50);
								  htim1.Instance->CCR3 = 0;
								  if(RxData_From_Node4[0]=='r')
								  {
									  htim1.Instance->CCR3 = 90;
									  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
									  HAL_Delay(50);
									  htim1.Instance->CCR3 = 0;
									break;
								  }
							  }*/



	  }
}
