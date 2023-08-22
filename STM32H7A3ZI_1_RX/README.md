# 모터부분 및 수신부 STM32

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)



### 사용 보드 및 모듈

<img width="20%" img src="https://github.com/crasdok/capstone/assets/118472691/cfe702d3-d627-4ac1-bfec-9163f5f86fbb">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/989a92d2-c85b-40db-a826-f4c484fbc64b">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/5a0e47d7-3f01-4db7-a132-9dc02becb296">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/30eef5f5-7e50-4789-90c0-201fee7df43b">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/ba959228-4d32-4977-9bc6-4671318cd336">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/20dbf480-d868-448a-a87f-45c0f19e11b1">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/9327bd25-a9cc-49e3-8912-f67e9bb57eb8">

> STM32H7A3ZI-Q보드, CAN Transciever, 모터, 모터드라이버, 헤드라이트



### 메인 박스 

<img width="40%" img src="https://github.com/crasdok/capstone/assets/118472691/7f0f0740-1cec-481f-b73c-b1ab1f477958">

> 아래쪽 오른쪽 보드가 이 보드

| 기능           | 설명                                                         |
| -------------- | ------------------------------------------------------------ |
| CAN Transciever | CAN 버스 네트워크에서 신호 변환, 신호 강화 및 수신, 전송제어 등의 역할을 수행한다. |
| 모터 | 뒷 바퀴와 앞 바퀴를 각각 움직이게 함 |
| 모터 드라이버  | 모터의 전력을 공급하고 전류와 전압을 제어, 모터를 보호한다. |
| 헤드라이트  | 전방을 밝히며 조도센서에 의해 밝기가 조절 된다. |

### 센서들의 이용

<img width="70%" src="https://github.com/crasdok/capstone/assets/118472691/09678126-57b1-4e91-86e8-5eb091b7303f"/>

### 모터 제어 순서도

![KakaoTalk_20230821_223311406](https://github.com/sc11046/adas_with_can_nrf/assets/121782720/0034f505-db18-4bf8-bcbc-024101b9b17a)


* Main의 While문
```c
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (isDataAvailable(2) == 1)
	  {
		  NRF24_Receive(RxData);
	  }

	  go_back(); //RF통신을 통해 받은 값으로 앞,뒤로 모터를 돌리는 함수
	  buzzer(); //CAN통신을 통해 받은 값으로 부저를 울리는 함수
	  light_sensor(); //CAN통신을 통해 받은 값으로 헤드라이트의 밝기를 조절하는 함수
	  nrf_motor(); //RF통신을 통해 받은 값으로 앞바퀴의 좌,우를 움직이는 함수
	  rpi_motor(); //CAN통신을 통해 받은 값으로 앞바퀴의 좌,우를 움직이는 함수
	}
```

* go_back 함수
  
  ```c
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
```

* buzzer 함수

```c
void buzzer (void){

	  for(int i=0;i<11;i++)
	  {a[i]=RxData_From_Node3[i]-'0';}

	  int Distance1 = 100* a[1]  +10*a[2] +a[3];
	  int Distance2 = 100* a[4]  +10*a[5] +a[6];
	  int Distance3 = 100* a[8]  +10*a[9] +a[10];
    if (Distance1 <= 15 || Distance2 <= 15 || Distance3 <= 15)
    {
	 	  TIM2->ARR = C;
	 	  TIM2->CCR1 = TIM2->ARR / 2;
	 	  HAL_Delay(50);
	 	  TIM2->CCR1 = 0;
	 	  HAL_Delay(50);
	 	  }
}
```

*


  





<br> [위로](#초음파센서-및-조도센서-STM32) <br>
