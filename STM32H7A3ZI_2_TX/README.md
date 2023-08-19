# 초음파센서 및 조도센서 STM32

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)

## HardWare

### 사용 보드 및 모듈

<img width="20%" img src="https://github.com/crasdok/capstone/assets/118472691/cfe702d3-d627-4ac1-bfec-9163f5f86fbb">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/989a92d2-c85b-40db-a826-f4c484fbc64b">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/313d6a92-a6ba-4a9e-8387-5e4b77a7dd39">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/15f08b0e-ab5b-4e37-8276-e172cb195a3d">

> STM32H7A3ZI-Q보드, CAN Transciever, 조도센서, 초음파 센서

### 메인 박스 

<img width="40%" img src="https://github.com/crasdok/capstone/assets/118472691/7f0f0740-1cec-481f-b73c-b1ab1f477958">

> 아래쪽 오른쪽 보드가 이 보드

| 기능           | 설명                                                         |
| -------------- | ------------------------------------------------------------ |
| CAN Transciever | CAN 버스 네트워크에서 신호 변환, 신호 강화 및 수신, 전송제어 등의 역할을 수행한다. |
| 초음파 센서 | 좌,우, 후면 3군데에 초음파를 이용하여 거리 정보를 알 수 있다. |
| 조도 센서  | 밝고 어두운 정도를 알 수 있다. |

## SoftWare

* 1개의 초음파 센서의 2개의 입력신호 사이의 간격을 측정
```c
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
}
```
> 이러한 초음파 센서 3개가 있다.

* 초음파 발사 및 정지
```c
void HCSR04_Read (void)
{
   HAL_GPIO_WritePin(TRIG_PORT1, TRIG_PIN1, GPIO_PIN_SET);  // pull the TRIG pin HIGH
   delay(10);  // wait for 10 us
   HAL_GPIO_WritePin(TRIG_PORT1, TRIG_PIN1, GPIO_PIN_RESET);  // pull the TRIG pin low

   __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1);

}
```

* 측정 값을 FDCAN을 통해 송신 하는 코드
```c
sprintf ((char *)TxData_Node3_To_Node2," %d%d%d%d",D1_1,D1_2,D1_3,D2_1);
  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData_Node3_To_Node2)!= HAL_OK)
	  	{
	  		  		 Error_Handler();
	  	}
	  	HAL_Delay(100);
```

<br> [위로](#초음파센서-및-조도센서-STM32) <br>
