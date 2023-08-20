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

### 순서도 

<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/07dca26d-33f4-4e66-bbfc-56cf72ddde81">

### 중요 코드

```c
MX_FDCAN1_Init

```
> FDCAN 통신을 위한 모듈 초기화 및 파라미터 설정

```c
NRF24_Init();
NRF24_RxMode(RxAddress, 10);//nrf rx
NRF24_ReadAll(data);//nrf rx

```

> CAN 포기화

```c
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
   if(FDCAN1 == hfdcan->Instance)
   {
	  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	  {

		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData_From_Node3) != HAL_OK)
		{
		Error_Handler();
		}

	  }
   }
```
> CallBack

```c
        if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_BUFFER_NEW_MESSAGE, 0) != HAL_OK)
          {
            /* Notification Error */
            Error_Handler();
          }
            if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
              {
                Error_Handler();
              }
            if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK)
              {
                Error_Handler();
              }
```
> 각각의 ECU들의 ACK 전송

```c
	  go_back();
	  buzzer();
	  light_sensor();
	  nrf_motor();
	  rpi_motor();
```
> 주요 함수 들로 자세히 보려면

```c
TxHeader.Identifier = 0x11; // 라이다
TxHeader.Identifier = 0x33; // 초음파, 조도센서
TxHeader.Identifier = 0x44; // 라즈베리파이
```
> 각 ECU들의 TxHeader.ID




<br> [위로](#초음파센서-및-조도센서-STM32) <br>
