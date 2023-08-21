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

### CAN통신을 사용한 이유

<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/e157f7d0-c303-4432-9d54-2087cfff16fa">

> Multi Master 통신방식 (CAN 인터페이스 하나로 여러 개의 모듈을 제어할 수 있기) 때문에 연결선을 감소시켜 무게 절감. 
> 추가적으로 ECU를 장착 하더라도 더욱 쉽게 확장할 수 있다는 장점을 가졌다.

### CAN Transciever를 사용한 이유

<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/6c874e58-b85d-4297-8683-6466162876ed">

> * 라즈베리파이에는 MCU내부에 CAN Controller가 존재하지 않기 때문에 MCP2515 사용
> * STM32에는 MCU내부에 CAN Controller가 존재하기 때문에 CAN Transciever 사용

### 메인 박스 

<img width="40%" img src="https://github.com/crasdok/capstone/assets/118472691/7f0f0740-1cec-481f-b73c-b1ab1f477958">

> 아래쪽 오른쪽 보드가 이 보드

| 기능           | 설명                                                         |
| -------------- | ------------------------------------------------------------ |
| CAN Transciever | CAN 버스 네트워크에서 신호 변환, 신호 강화 및 수신, 전송제어 등의 역할을 수행한다. |
| 모터 | 뒷 바퀴와 앞 바퀴를 각각 움직이게 함 |
| 모터 드라이버  | 모터의 전력을 공급하고 전류와 전압을 제어, 모터를 보호한다. |
| 헤드라이트  | 전방을 밝히며 조도센서에 의해 밝기가 조절 된다. |

### CAN통신 순서도 

<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/07dca26d-33f4-4e66-bbfc-56cf72ddde81">

### 모터 제어 순서도

![KakaoTalk_20230821_223311406](https://github.com/sc11046/adas_with_can_nrf/assets/121782720/0034f505-db18-4bf8-bcbc-024101b9b17a)





### CAN 프레임 구조


<img width="70%" img src="https://github.com/crasdok/capstone/assets/118472691/02003407-f90e-4cd1-a414-18997733698d">

> Start of Frame (SOF): 프레임의 시작을 나타내는 비트로, 높은 신호(0)로 시작됩니다.
>
> Arbitration ID (11 또는 29 비트)
>
> Remote Transmission Request (RTR): 데이터 프레임인지 아니면 리모트 프레임인지를 나타내는 비트입니다.
>
> Control Bits: 에러 처리 및 메시지 길이 정보를 포함하는 비트입니다.
>
> Data Field: 실제 데이터를 포함하는 부분입니다.
>
> Cyclic Redundancy Check (CRC): 에러 체크를 위한 CRC 코드로, 데이터의 무결성을 확인하는 데 사용됩니다.
>
> ACK Slot: 수신 노드가 메시지를 정상적으로 수신했음을 알려주는 비트입니다.
>
> End of Frame (EOF): 프레임의 끝을 나타내는 비트로, 높은 신호(1)로 끝납니다.


### 주요 코드

> FDCAN 통신을 위한 모듈 초기화 및 파라미터 설정
```c
MX_FDCAN1_Init();

```

> RF통신을 위한 모듈 초기화 및 설정들, 데이터 수신
```c
NRF24_Init();
uint8_t RxAddress[] = {0x00,0xDD,0xCC,0xBB,0xAA};
NRF24_RxMode(RxAddress, 10);//nrf rx
NRF24_ReadAll(data);//nrf rx

```


> 각 ECU들의 TxHeader.ID
```c
TxHeader.Identifier = 0x11; // 라이다
TxHeader.Identifier = 0x33; // 초음파, 조도센서
TxHeader.Identifier = 0x44; // 라즈베리파이
```

> FIFO CallBack : STM32 사용
>  <br/>
>  이 코드는 FDCAN1 모듈의 FIFO0에서 새로운 메시지가 도착할 때마다 해당 메시지를 읽어오는 기능을 수행합니다.
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

>  BUFFER CallBack :라즈베리파이 사용
>  <br/>
> 이 코드는 FDCAN1 모듈의 Rx 버퍼0에서 새로운 메시지가 도착할 때마다 해당 메시지를 읽어오는 기능을 수행합니다.
```c
void HAL_FDCAN_RxBufferNewMessageCallback(FDCAN_HandleTypeDef *hfdcan)
{

    if (FDCAN1 == hfdcan->Instance)
    {
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_BUFFER0, &RxHeader, RxData_From_Node4) != HAL_OK)
        {
            Error_Handler();
        }
    }

}
```

> 각각의 ECU들의 ACK 전송
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

> Main의 While문
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


### 라즈베리파이에서 STM32로 CAN통신

![](https://github.com/crasdok/capstone/assets/118472691/ffd34525-87b5-41d7-83ca-953b02066786)







<br> [위로](#초음파센서-및-조도센서-STM32) <br>
