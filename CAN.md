# Can,CanFD 통신
### CAN (Controller Area Network) 및 CAN FD (CAN with Flexible Data-Rate)은 자동차 및 산업 분야에서 널리 사용되는 통신 프로토콜입니다. 이들은 실시간 통신을 위해 개발된 프로토콜로, 주로 차량 내부의 제어 시스템이나 여러 센서 및 액추에이터 간의 데이터 교환에 사용됩니다.




### CAN통신을 사용한 이유

<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/e157f7d0-c303-4432-9d54-2087cfff16fa">

> 1. Multi Master 통신방식 (CAN 인터페이스 하나로 여러 개의 모듈을 제어할 수 있기) 때문에 연결선을 감소시켜 무게 절감.
> <br/>
> 2. 추가적으로 ECU를 장착 하더라도 더욱 쉽게 확장할 수 있다는 장점을 가졌다.
> <br/>
> 3. 안정적이고 신뢰성 있는 통신: CAN은 실시간 통신을 지원하며, 오류 검출 및 복구 메커니즘을 내장하고 있어서 안정적인 데이터 교환을 제공합니다.
> <br/>
> 4. 실시간 응용에 적합: CAN은 실시간 통신을 지원하여 시간에 민감한 응용에서 사용됩니다.
> <br/>
> 5. 안정적인 환경에서 동작: CAN은 강한 전자기적 간섭(EMI) 및 노이즈에도 강한 환경에서 동작할 수 있는 프로토콜입니다.
> <br/>
> 6. 비용 효율성: CAN은 비교적 저렴하게 구현될 수 있으며, 복잡한 네트워크도 상대적으로 저비용으로 구축할 수 있습니다.


### CAN Transciever를 사용한 이유

<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/6c874e58-b85d-4297-8683-6466162876ed">

> * 라즈베리파이에는 MCU내부에 CAN Controller가 존재하지 않기 때문에 MCP2515 사용
> * STM32에는 MCU내부에 CAN Controller가 존재하기 때문에 CAN Transciever 사용

### CAN통신 순서도 

<img width="120%" img src="https://github.com/crasdok/capstone/assets/118472691/fd8284e3-5e3c-4b65-bdee-68822bbe90eb">

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

> 각 ECU들의 TxHeader.ID
```c
TxHeader.Identifier = 0x11; // 라이다
TxHeader.Identifier = 0x33; // 초음파, 조도센서
TxHeader.Identifier = 0x44; // 라즈베리파이
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

* 라즈베리파이 CAN통신 코드
> 설정 및 값 전달
```python
bus = can.Bus(interface='socketcan',
              channel='can0',
              receive_own_messages=True)

message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x52]) //ID=0x44, R 값은 0X52, L값은 0x4C이다. 0X52 와 0x4C 는 아스키코드 값이다.
bus.send(message, timeout=0.2)
```

### 라즈베리파이에서 STM32로 CAN통신

![](https://github.com/crasdok/capstone/assets/118472691/ffd34525-87b5-41d7-83ca-953b02066786)

### STM32 Rx에서 모드 CAN통신 값 받는 영상

ㄴㅇㄹㄴㅇㄹㅇㄴㄹㅇㄹㄴ
