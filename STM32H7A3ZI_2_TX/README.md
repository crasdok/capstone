# 초음파센서 및 조도센서 STM32

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)

## 목차
- [사용 보드 및 모듈](#사용-보드-및-모듈)
- [메인 박스](#메인-박스)
- [코드](#코드)
- [기능 구현 영상](#기능-구현-영상)

### 사용 보드 및 모듈

<img width="20%" img src="https://github.com/crasdok/capstone/assets/118472691/cfe702d3-d627-4ac1-bfec-9163f5f86fbb">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/989a92d2-c85b-40db-a826-f4c484fbc64b">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/313d6a92-a6ba-4a9e-8387-5e4b77a7dd39">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/15f08b0e-ab5b-4e37-8276-e172cb195a3d">

> STM32H7A3ZI-Q보드, CAN Transciever, 조도센서, 초음파 센서

### 메인 박스 
 
<img width="40%" img src="https://github.com/crasdok/capstone/assets/118472691/7f0f0740-1cec-481f-b73c-b1ab1f477958">
초음파 3개 사진 조도

> 아래쪽 오른쪽 보드가 이 보드

| 기능           | 설명                                                         |
| -------------- | ------------------------------------------------------------ |
| CAN Transciever | CAN 버스 네트워크에서 신호 변환, 신호 강화 및 수신, 전송제어 등의 역할을 수행한다. |
| 초음파 센서 | 좌,우, 후면 3군데에 초음파를 이용하여 거리 정보를 알 수 있다. |
| 조도 센서  | 밝고 어두운 정도를 알 수 있다. |

## IOC 

ioc 사진 
채널 124를 사용 adc

## 초음파 거리를 측정하는 과정 

<img width="100%" img src="https://github.com/crasdok/capstone/assets/118472691/2afaa613-30f2-49bb-ad56-e9f89311bec4">

 ## 코드
* 입력 캡처를 사용하여 시간 간격을 측정하고 이를 거리로 변환하는 데 사용
```c
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  
	{
		if (Is_First_Captured_1==0) 
		{
			IC_Val1_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); 
			Is_First_Captured_1 = 1;  
			
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured_1==1)   
		{
			IC_Val2_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  
			__HAL_TIM_SET_COUNTER(htim, 0);  

			if (IC_Val2_1 > IC_Val1_1)
			{
				Difference_1 = IC_Val2_1-IC_Val1_1;
			}

			else if (IC_Val1_1 > IC_Val2_1)
			{
				Difference_1 = (0xffff - IC_Val1_1) + IC_Val2_1;
			}

			Distance_1 = Difference_1 * .034/2;
			Is_First_Captured_1 = 0; 

			
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC1);
		}
	}


	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)  
	{
		if (Is_First_Captured_2==0) 
		{
			IC_Val1_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); 
			Is_First_Captured_2 = 1; 
			
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured_2==1)   
		{
			IC_Val2_2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  
			__HAL_TIM_SET_COUNTER(htim, 0);  

			if (IC_Val2_2 > IC_Val1_2)
			{
				Difference_2 = IC_Val2_2-IC_Val1_2;
			}

			else if (IC_Val1_2 > IC_Val2_2)
			{
				Difference_2 = (0xffff - IC_Val1_2) + IC_Val2_2;
			}

			Distance_2 = Difference_2 * .034/2 - 40;
			Is_First_Captured_2 = 0; 

			
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC2);
		}
	}




	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)  
			{
				if (Is_First_Captured_4==0) 
				{
					IC_Val1_4 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); 
					Is_First_Captured_4 = 1;  
					
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
				}

				else if (Is_First_Captured_4==1)   
				{
					IC_Val2_4 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  
					__HAL_TIM_SET_COUNTER(htim, 0);  

					if (IC_Val2_4 > IC_Val1_4)
					{
						Difference_4 = IC_Val2_4-IC_Val1_4;
					}

					else if (IC_Val1_4 > IC_Val2_4)
					{
						Difference_4 = (0xffff - IC_Val1_4) + IC_Val2_4;
					}

					Distance_4 = Difference_4 * .034/2;
					Is_First_Captured_4 = 0; 

					
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
					__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC4);
				}
			}


}
```
> 순서
> 1. 함수 내부에서 htim->Channel 값에 따라 다른 채널의 입력 캡처 인터럽트를 처리한다.
> 2. Is_First_Captured_x 변수는 각 채널의 첫 번째 캡처 값이 캡처되었는지 여부를 나타낸다.
> 3. 최초로 캡처되지 않은 경우, IC_Val1_1에 첫 번째 값을 저장하고 캡처가 되었음을 표시하며, 캡처를 하강 에지로 설정한다.
> 4. 이미 첫 번째 캡처가 된 경우, 두 번째 값을 IC_Val2_1에 저장하고 타이머 카운터를 초기화한다.
> 5. IC_Val2_1과 IC_Val1_1의 크기에 따라 두 값의 차이를 계산한다.
> 6. 계산된 시간 간격을 거리로 변환한다.
> 7. 다음 캡처를 위해 변수들을 재설정하고, 캡처를 상승 에지로 설정한다.
> * 채널 2,4 또한 반복과정



<br> [위로](#초음파센서-및-조도센서-STM32) <br>

* 초음파 발사 및 정지
```c
void HCSR04_Read1 (void)
{
   HAL_GPIO_WritePin(TRIG_PORT1, TRIG_PIN1, GPIO_PIN_SET);  // TRIG 핀을 HIGH로 설정하여 초음파 발신 신호를 보냄.
   delay(10);  // wait for 10 us
   HAL_GPIO_WritePin(TRIG_PORT1, TRIG_PIN1, GPIO_PIN_RESET);  // TRIG 핀을 LOW로 설정하여 초음파 발신 신호를 종료.

   __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1); // 타이머 3의 입력 캡처 채널 1 인터럽트를 활성화.

}
void HCSR04_Read2 (void)
{
   HAL_GPIO_WritePin(TRIG_PORT2, TRIG_PIN2, GPIO_PIN_SET);  // TRIG 핀을 HIGH로 설정하여 초음파 발신 신호를 보냄.
   delay(10);  
   HAL_GPIO_WritePin(TRIG_PORT2, TRIG_PIN2, GPIO_PIN_RESET);  // TRIG 핀을 LOW로 설정하여 초음파 발신 신호를 종료.

   __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC2); // 타이머 3의 입력 캡처 채널 2 인터럽트를 활성화.

}
void HCSR04_Read4 (void)
{
   HAL_GPIO_WritePin(TRIG_PORT4, TRIG_PIN4, GPIO_PIN_SET);  // TRIG 핀을 HIGH로 설정하여 초음파 발신 신호를 보낸다.
   delay(10);  
   HAL_GPIO_WritePin(TRIG_PORT4, TRIG_PIN4, GPIO_PIN_RESET);  // TRIG 핀을 LOW로 설정하여 초음파 발신 신호를 종료.

   __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC4); // 타이머 3의 입력 캡처 채널 4 인터럽트를 활성화.

}
```
> 초음파 거리 측정 센서는 초음파 신호를 발사하고 해당 신호가 물체에서 반사된 후 걸리는 시간을 측정하여 거리를 계산하는 원리로 동작.
> <br/>
> 이 코드를 통해 타이머가 초음파 발신과 수신 시간을 측정하여 거리를 계산하게 된다.

* 이 코드는 ADC 변환을 시작하고, 변환이 완료될 때까지 기다린 후 변환 결과 값(조도 값)을 가져와서 조정한 뒤 adc1 변수에 저장하는 역할을 수행한다.
```c
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, 10);
adc1 = HAL_ADC_GetValue(&hadc1)/650;
```

> adc1값에 650을 나눈 이유는 16비트 ADC값이기 때문에 최댓값 65535을 100으로 맞추기 위해 나누었다.


## 기능 구현 영상

조도, 초음파





<br> [위로](#초음파센서-및-조도센서-STM32) <br>
