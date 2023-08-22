# Can,CanFD 통신
### CAN (Controller Area Network) 및 CAN FD (CAN with Flexible Data-Rate)은 자동차 및 산업 분야에서 널리 사용되는 통신 프로토콜입니다. 이들은 실시간 통신을 위해 개발된 프로토콜로, 주로 차량 내부의 제어 시스템이나 여러 센서 및 액추에이터 간의 데이터 교환에 사용됩니다.




### CAN통신을 사용한 이유

<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/e157f7d0-c303-4432-9d54-2087cfff16fa">

> 1. Multi Master 통신방식 (CAN 인터페이스 하나로 여러 개의 모듈을 제어할 수 있기) 때문에 연결선을 감소시켜 무게 절감.
> 2. 추가적으로 ECU를 장착 하더라도 더욱 쉽게 확장할 수 있다는 장점을 가졌다.
> 3. 안정적이고 신뢰성 있는 통신: CAN은 실시간 통신을 지원하며, 오류 검출 및 복구 메커니즘을 내장하고 있어서 안정적인 데이터 교환을 제공한다.
> 4. 실시간 응용에 적합: CAN은 실시간 통신을 지원하여 시간에 민감한 응용에서 사용된다.
> 5. 안정적인 환경에서 동작: CAN은 강한 전자기적 간섭(EMI) 및 노이즈에도 강한 환경에서 동작할 수 있는 프로토콜이다.
> 6. 비용 효율성: CAN은 비교적 저렴하게 구현될 수 있으며, 복잡한 네트워크도 상대적으로 저비용으로 구축할 수 있다.


### CAN Transciever를 사용한 이유

<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/6c874e58-b85d-4297-8683-6466162876ed">

> * 라즈베리파이에는 MCU내부에 CAN Controller가 존재하지 않기 때문에 MCP2515 사용
> * STM32에는 MCU내부에 CAN Controller가 존재하기 때문에 CAN Transciever 사용

### CAN통신 순서도 

<img width="120%" img src="https://github.com/crasdok/capstone/assets/118472691/5d2bb8fc-c130-45b9-b615-68fb2368fe94">

### CAN 프레임 구조


<img width="70%" img src="https://github.com/crasdok/capstone/assets/118472691/02003407-f90e-4cd1-a414-18997733698d">

> Start of Frame (SOF): 프레임의 시작을 나타내는 비트로, 높은 신호(0)로 시작된다.
>
> Arbitration ID (11 또는 29 비트)
>
> Remote Transmission Request (RTR): 데이터 프레임인지 아니면 리모트 프레임인지를 나타내는 비트이다.
>
> Control Bits: 에러 처리 및 메시지 길이 정보를 포함하는 비트이다.
>
> Data Field: 실제 데이터를 포함하는 부분이다.
>
> Cyclic Redundancy Check (CRC): 에러 체크를 위한 CRC 코드로, 데이터의 무결성을 확인하는 데 사용된다.
>
> ACK Slot: 수신 노드가 메시지를 정상적으로 수신했음을 알려주는 비트이다.
>
> End of Frame (EOF): 프레임의 끝을 나타내는 비트로, 높은 신호(1)로 끝난다.



### 주요 코드

<img width="70%" img src="https://github.com/crasdok/capstone/assets/118472691/25651329-2268-453d-9e8e-edeb2f575bdf">
<img width="70%" img src="https://github.com/crasdok/capstone/assets/118472691/db3db726-478a-40ec-84ba-a7f0bb10a3b4">

> FDCAN 통신을 위한 모듈 초기화 및 파라미터 설정
```c
    void MX_FDCAN1_Init(void)
    {
        hfdcan1.Instance = FDCAN1;
        hfdcan1.Init.FrameFormat = FDCAN_FRAME_FD_NO_BRS;
        hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
        hfdcan1.Init.AutoRetransmission = ENABLE;
        hfdcan1.Init.TransmitPause = DISABLE;
        hfdcan1.Init.ProtocolException = DISABLE;
        hfdcan1.Init.NominalPrescaler = 1;
        hfdcan1.Init.NominalSyncJumpWidth = 1;
        hfdcan1.Init.NominalTimeSeg1 = 5;
        hfdcan1.Init.NominalTimeSeg2 = 2;
     }
```

> 모든 ECU들은 통신하기 위해 각 ECU들의 클럭 속도를 맞추는게 중요하다.
> <br/>
> APB1 clock = 4MHz
> <br/>
> Prescaler = 1
> <br/>
> 4MHz / 1  →  초당 4000000 bit
> <br/>
> 1타임 퀀텀의 시간 단위 = 1 / 4MHz =0.25us
> <br/>
> SYNC_SEG → 1 타임 퀀텀 사용 (고정)
> <br/>
> BIT SEGMENT 1 (BS1) → 5 타임 퀀텀 배정
> <br/>
> BIT SEGMENT 2 (BS2) → 2 타임 퀀텀 배정
> <br/>
> SYNC_SEG + BS1 + BS2 = 총 8개 타임 퀀텀, 1 타임 퀀텀 당 0.25us
> <br/>
> total quantum → 0.25us * 8 = 2us
> <br/>
> 따라서 1 bit 당 2us 소요 (500kbps → 초당 500k bit)
> <br/>
> SAMPLE POINT*= (SYNC_SEG + BS1) / 전체 타임 퀀텀 = (1 + 5) / 8 =75%
> <br/>
> BaudRate (통신속도) = 500kbps

* 각 ECU들의 TxHeader.ID
```c
    TxHeader.Identifier = 0x11;  //라이다
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_16;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_FD_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0x0;

    TxHeader.Identifier = 0x33; // 초음파, 조도센서
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_16;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_FD_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0x0;
```

```python
message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x4C]) // 라즈베리파이
```
> * TxHeader.Id = 0x11; //라이다
> * TxHeader.Id = 0x33; // 초음파, 조도센서
> * TxHeader.Id = 0x44; // 라즈베리파이


* FIFO CallBack : STM32 2개에서 데이터를 받는데에 사용

```c
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
   if(FDCAN1 == hfdcan->Instance) // 현재 FDCAN 인스턴스가 FDCAN1 인지 확인
   {
	  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) // 수신 FIFO0에서 새로운 메시지 도착 인터럽트 플래그인지 확인
	  {

		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData_From_Node3) != HAL_OK)
		{
		Error_Handler();
		}

	  }
   }
```
>  이 코드는 FDCAN1 모듈의 FIFO0에서 새로운 메시지가 도착할 때마다 해당 메시지를 읽어오는 기능을 수행한다.
> <br/>
> HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData_From_Node3): 이 함수는 FDCAN 수신 FIFO0에서 메시지를 가져온다.
> <br/>
> hfdcan은 FDCAN 핸들, FDCAN_RX_FIFO0은 사용할 수신 FIFO 번호, RxHeader는 메시지의 헤더 정보를 저장할 변수, RxData_From_Node3는 메시지 데이터를 저장할 버퍼이다.
> <br/>
> 만약 메시지 가져오기가 실패하면 Error_Handler() 함수가 호출된다.

*  BUFFER CallBack :라즈베리파이 에서 데이터를 받는데에 사용
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
> 이 코드는 FDCAN1 모듈의 Rx 버퍼0에서 새로운 메시지가 도착할 때마다 해당 메시지를 읽어오는 기능을 수행한다.
> <br/>
> HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_BUFFER0, &RxHeader, RxData_From_Node4): 이 함수는 FDCAN 수신 버퍼에서 메시지를 가져온다.
> <br/>
> hfdcan은 FDCAN 핸들, FDCAN_RX_BUFFER0은 사용할 수신 버퍼의 번호, RxHeader는 메시지의 헤더 정보를 저장할 변수, RxData_From_Node4는 메시지 데이터를 저장할 버퍼이다.
> <br/>
> 만약 메시지 가져오기가 실패하면 Error_Handler() 함수가 호출된다.


* 각각의 ECU들의 ACK 전송
```c
        if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_BUFFER_NEW_MESSAGE, 0) != HAL_OK) // 수신 버퍼에 새로운 메시지 도착  알림 실패시
          {
            /* Notification Error */
            Error_Handler(); // 오류 처리
          }
            if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) //  FIFO 0에 새로운 메시지 도착 시 알림 실패시
              {
                Error_Handler(); // 오류 처리
              }
            if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK) //  FIFO 1에 새로운 메시지 도착 시 알림 실패시
              {
                Error_Handler(); // 오류 처리
              }
```

* STM32 CAN통신을 이용하여 데이터 전송

```c
    sprintf ((char *)TxData_Node1_To_Node3 ,"%d%d%d",Dist1,Dist2,Dist3);
    if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData_Node1_To_Node3)!= HAL_OK)
    {
        Error_Handler();
    }
```
>  문자열 포맷팅으로 데이터를 문자열로 변환하고, 그 문자열 데이터를 CAN 메시지의 데이터 부분에 넣어서 전송하는 것이다. 이로써 다양한 센서 데이터나 상태 정보를 CAN 메시지로 전송하여 통신하는 기능을 구현할 수 있다.

* 라즈베리파이 CAN통신 코드

### Raspberry Pi CAN 초기화 및 설정
> 라즈베리파이에 CAN Controller Module을 사용하기 위해선 config.txt 를 수정해야 함
> 수정 후 재부팅해주어야 적용완료가 됨
```python
dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=12
dtoverlay=spi-bcm2835-overlay
```
>  재부팅 후 CAN 유틸리티 다운로드(터미널)
```python
sudo apt-get install can-utils
```
> CAN 활성화(터미널)
```python
sudo ip link set can0 up type can bitrate 4000000
```
> CAN이 정상적으로 작동이 확인하는 코드(터미널)
```python
sudo ifconfig can0
```

* CAN통신 설정, 초기화 
```python
bus = can.Bus(interface='socketcan', // 사용할 CAN 인터페이스를 지정
              channel='can0', //  사용할 CAN 채널을 지정
              receive_own_messages=True) // 자신이 전송한 메시지도 수신하는 것을 허용
```

* CAN통신을 이용해 메시시 전송
```python
message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x52]) //ID=0x44, R 값은 0X52, L값은 0x4C이다. 0X52 와 0x4C 는 아스키코드 값이다.
bus.send(message, timeout=0.2) // bus.send 함수를 호출하여 메시지를 전송
```
>

* 라즈베리파이 코드 시작 전 전송 큐 길이를 10000으로 설정
```python
sudo ifconfig can0 txqueuelen 10000
```
> * 고대역폭 활용: CAN 네트워크에서 많은 양의 메시지를 처리해야 할 때, 높은 큐 길이는 대역폭을 더 효율적으로 활용할 수 있다.
> * 대량 데이터 전송: 대량의 CAN 메시지를 전송해야 할 경우, 큐 길이를 늘려서 한 번에 더 많은 메시지를 보낼 수 있다.
> * 효율적인 트래픽 관리: CAN 네트워크에서 네트워크 트래픽의 혼잡을 완화할 수 있다. 큐 길이를 늘려서 패킷 충돌이나 데이터 손실을 방지할 수 있다.
> * 응답 시간 개선: 큰 큐 길이를 사용하면 패킷이 큐에 더 오래 남아있어서 더 빠른 응답 시간을 얻을 수 있다.
> * 고속 통신에서의 안정성: 높은 속도의 CAN 통신 환경에서 큰 큐 길이는 안정성과 데이터 정확성을 향상시킬 수 있다.

### 라즈베리파이에서 STM32로 CAN통신

![](https://github.com/crasdok/capstone/assets/118472691/ffd34525-87b5-41d7-83ca-953b02066786)

### STM32 Rx에서 모드 CAN통신 값 받는 영상

ㄴㅇㄹㄴㅇㄹㅇㄴㄹㅇㄹㄴ
