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
> go_back, nrf_motor, rpi_motor 함수의 제어 포함


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
			  ridar();
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
				ridar();
			  }
  ```
  > 수신된 RF값 데이터를 기반으로 움직임을 제어하는 역할을 한다. 수신된 데이터의 특정 값에 따라 모터의 동작 방향을 설정하고 모터의 속도를 조절하고, 만약 모터의 속도가 일정 값 이상이면 최대 속도로 제한한다. 이후에 빛 
  센서와 라이다 센서의 데이터를 활용하여 추가적인 동작을 수행
  
* nrf_motor 함수
```c
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
```
> 수신된 RF값 데이터를 기반으로 움직임을 제어. 수신된 데이터의 특정 값에 따라 앞바퀴의 방향을 설정한다
> <br/>
> RxData[3]==0 으로 원격 조종 모드로 설정되고 각각의 상황에 따라 앞바퀴의 방향을 조향한다.
> 50<=RxData[1]&&RxData[1]<=65 으로 중앙값으로 돌아오면 역방향으로 회전을 해 중앙에 바퀴가 위치한다.

|범위| 방향                                                      |
| ------- | ------------------------------------------------------------ |
| RxData[1]  < 50 | Left |
| RxData[1] > 65 | Right |
| 50<=RxData[1]&&RxData[1]<=65 | Go |


* rpi_motor 함수

```c
void rpi_motor (void){
	  if(RxData[3]==1)
	  {
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
	  }
}
```
> 수신된 CAN값 데이터를 기반으로 움직임을 제어. 수신된 데이터의 특정 값에 따라 모터의 동작 방향을 설정한다.
> <br/>
> RxData[3]==1 으로 차선인식 모드로 설정되고 각각의 상황에 따라 앞바퀴의 방향을 조향한다.
> RxData_From_Node4[0]=='G'라면 바퀴를 중앙에 위치하게 한다.

|범위| 방향                                                      |
| ------- | ------------------------------------------------------------ |
| RxData_From_Node4[0]=='L' | Left |
| RxData_From_Node4[0]=='R' | Right |
| RxData_From_Node4[0]=='G' | Go |

* Lidar 함수
```c
  void Lidar (void){


	  for(int j=0;j<3;j++)
	  {b[j]=RxData_From_Node1[j]-'0';}

	  int Distance4 = 100* b[0]  +10*b[1] +b[2];//rider 
  	  if(Distance4<=20)
  	  {
  		  htim1.Instance->CCR1=0;
  		  htim1.Instance->CCR2=0;
  	  }
}
```
> 라이다 값을 불러와 3자리 정수로 만든 뒤, Lidar로부터 측정된 값이 20 이하일때, htim1 타이머의 채널 1, 2 의 PWM 듀티 사이클을 0으로 설정(양바퀴의 움직임을 멈춘다).


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
> Distance1, Distance2, 또는 Distance3 중 하나라도 15 이하의 값이라면 부저가 울리는 코드



* light_sensor 함수

```c
void light_sensor (void){

	 for(int l=12;l<=14;l++)
	 {a[l]=RxData_From_Node3[l]-'0';}
	 int jodo = 100* a[12]  +10*a[13] +a[14];
	      htim3.Instance->CCR1=jodo;
	      if (jodo<45)
	      {
	    	  htim3.Instance->CCR1=0;
	      }
}
```
> 라이다 센서에서 계산한 밝기 값을 타이머 3의 채널 1의 캡처/비교 레지스터인 CCR1에 설정한다. 이를 통해 LED의 밝기가 설정된다.
> <br/>
> 만약 계산된 밝기 값이 45보다 작다면, htim3.Instance->CCR1=0;(LED의 밝기를 0으로 설정하여 LED를 꺼준다.)

### 기능 구현 영상 
  
![뒷바퀴 엑셀](https://github.com/qkcvb110/Portfolio/assets/121782690/c9771932-f7c3-4f74-92a0-41dc8a3f20c6)
> 뒷바퀴 전,후진

![부저 (1)](https://github.com/qkcvb110/Portfolio/assets/121782690/8dcb642f-5dad-4d8f-a2a7-7644101f6e36)
> 부저 (LED로 대체)

<br> [위로](#초음파센서-및-조도센서-STM32) <br>
