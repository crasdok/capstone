# STM32F411

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)

## 바로가기

- [순서도](#순서도)
- [RF통신 사용 이유](#RF통신을-사용한-이유)
- [송신부 코드](#송신부-코드)
- [수신부 코드](#수신부-코드)
- [Live Expression](#RF-통신중-Live-Expression-화면-영상)

* 사용 보드 및 모듈

<img width="15%" img src="https://github.com/crasdok/capstone/assets/118472691/41401bde-bbf7-4947-a453-83145d4e18fd">
<img width="40%" img src="https://github.com/crasdok/capstone/assets/118472691/22a43bd1-8ad6-44d3-a3bf-ec31a0f7d39c">

<img width="15%" img src="https://github.com/crasdok/capstone/assets/118472691/685b7edb-ec31-4bab-a4dd-c7062b7bf040">
<img width="15%" img src="https://github.com/crasdok/capstone/assets/118472691/dae49f6e-9cae-484d-bf2f-1e7ae16f05cc">



> STM32F411보드, NRF24L01, 토글 스위치, 가변저항

 ## 순서도
<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/b4bea1f8-88fe-45f1-85f7-5c1a1f29941c">

## 각각의 보드와 모듈을 사용한 이유
|보드, 모듈| 설명                                                         |
| ------- | ------------------------------------------------------------ |
| STM32F411 | 실시간으로 바뀌는 값을 지속적으로 보내줘야 하고 송신부에 적은 면적을 차지하게 하고 싶었기 때문에 작고 성능 좋은 보드를 사용했다. |
| NRF24L01 | RF통신의 통신범위를 보다 넓히고 싶었기 때문에 사용했다. |
| 토글 스위치 | 모드 선택시 가장 알맞은 스위치라 생각하였기 때문에 사용했다. |
| 가변저항  | 악셀과 핸들의 변하는 값을 나타내기 위하여 사용했다. |

## RF통신을 사용한 이유

<img width="60%" img src="https://github.com/crasdok/capstone/assets/118472691/68a1244f-26f1-4be8-95d1-9b07a53805ee">

> RF통신은 방향성이 존재하지 않기 때문에 장애물이 있어도 신호를 전달할 수 있다는 장점 때문에

## 악셀값과 핸들값 송신부 사진

  <img width="50%" img src="https://github.com/crasdok/capstone/assets/118472691/a5820f42-f078-459c-b509-c5edebdb0df2">

| 기능           | 설명                                                         |
| -------------- | ------------------------------------------------------------ |
| NRF24L01 | 토글 스위치와 가변저항 각각의 값을 RF통신으로 STM32H7A3ZI보드에 보낸다. |
| 토글 스위치 | 전후진 과 주행모드를 선택할 수 있게 해준다. |
| 가변저항  | 악셀이 어느정도 밟았는지 와 핸들이 어느정도 위치에 있는지 값으로 나타내준다. |

<br> [위로](#STM32F411) <br>

 

## 송신부 코드

* 데이터 주소와 값의 길이 설정
```c
uint8_t TxAddress[] = {0xEE,0xDD,0xCC,0xBB,0xAA};
uint8_t TxData[32];
uint8_t RxAddress[] = {0x00,0xDD,0xCC,0xBB,0xAA};
NRF24_ReadAll(data);//nrf rx
```
* NRF24 Init 송,수신부 공통으로 사용
```c
void NRF24_Init (void)
{
	CE_Disable(); // chip을 설정하기 전에 비활성화
	nrf24_reset (0); // 모든 설정을 초기화
	nrf24_WriteReg(CONFIG, 0);  // CONFIG 레지스터 설정
	nrf24_WriteReg(EN_AA, 0);  // 자동 ACK 비활성화
	nrf24_WriteReg (EN_RXADDR, 0);  // 데이터 파이프 비활성화
	nrf24_WriteReg (SETUP_AW, 0x03);  // TX/RX 주소의 길이를 5 바이트로 설정
	nrf24_WriteReg (SETUP_RETR, 0);   // 데이터 재전송 설정 없음
	nrf24_WriteReg (RF_CH, 0);  // RF 채널 설정 (나중에 Tx 또는 Rx 중에 설정될 예정)
	nrf24_WriteReg (RF_SETUP, 0x0E);   // RF 전송 속도 및 출력 설정 (출력 = 0dB, 데이터 속도 = 2Mbps)
	CE_Enable(); // 설정을 마친 후 chip 활성화

}
```
> 이렇게 초기화된 모듈은 나중에 특정 동작에 따라 데이터 통신을 수행할 준비가 된 상태이다.

* TX모드 설정
```c

void NRF24_TxMode (uint8_t *Address, uint8_t channel) 
{
	
	CE_Disable(); // chip을 설정하기 전에 비활성화
	nrf24_WriteReg (RF_CH, channel);  // RF 채널을 설정
	nrf24_WriteRegMulti(TX_ADDR, Address, 5);  // TX 주소 설정

	uint8_t config = nrf24_ReadReg(CONFIG); // 모듈을 전원 켬
	config = config | (1<<1);   // PWR_UP 비트에 1을 설정
//	config = config & (0xF2);    // write 0 in the PRIM_RX, and 1 in the PWR_UP, and all other bits are masked
	nrf24_WriteReg (CONFIG, config);

	CE_Enable(); // 설정을 마친 후 chip 활성화
}
```
> 해당 함수를 호출하면 NRF24 모듈이 데이터 송신을 수행하기 위해 설정된다.

* NRF24 데이터 전송

```c
uint8_t NRF24_Transmit (uint8_t *data)
{
	uint8_t cmdtosend = 0;

	CS_Select(); // 디바이스 선택 (CS 신호 활성화)

	// 페이로드 커맨드 설정
	cmdtosend = W_TX_PAYLOAD; 
	HAL_SPI_Transmit(NRF24_SPI, &cmdtosend, 1, 100);

	// 페이로드 전송
	HAL_SPI_Transmit(NRF24_SPI, data,32, 1000);
//	HAL_SPI_Transmit(NRF24_SPI, data, sizeof(data), 1000);
	// Unselect the device
	CS_UnSelect(); // 디바이스 선택 해제 (CS 신호 비활성화)

	HAL_Delay(1);

	uint8_t fifostatus = nrf24_ReadReg(FIFO_STATUS); // FIFO_STATUS 레지스터의 상태 읽기

	// FIFO_STATUS의 네 번째 비트를 확인하여 TX FIFO가 비어 있는지 확인
	if ((fifostatus&(1<<4)) && (!(fifostatus&(1<<3))))
	{
		// TX FIFO를 비우는 커맨드 보내기
		cmdtosend = FLUSH_TX;
		nrfsendCmd(cmdtosend);

		// FIFO_STATUS 초기화
		nrf24_reset (FIFO_STATUS);

		return 1; // 전송 성공
	}

	return 0; // 전송 실패
}
```
> 이렇게 설정된 함수를 호출하면 NRF24 모듈을 사용하여 데이터를 전송할 수 있다.


* ADC값 변환 후 가공
```c
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
TxData[0] = HAL_ADC_GetValue(&hadc1)/27;
TxData[1] = HAL_ADC_GetValue(&hadc1)/40;
```
> 이 코드는 ADC 모듈을 사용하여 아날로그 입력 값을 변환하여 디지털 값으로 읽어오고, 그 값을 TxData 배열에 저장하는 과정을 나타낸다.

* 모드변경 스위치 부분
```c
            if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==1) //  GPIOB 포트의 3번 핀을 읽어와서 버튼의 상태를 감지 후 HIGH(1) 상태이면
	    {
	    	button = 1; // 전후진 상태 = 1
	    	TxData[2] = button; 
	    }
	    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==0) // GPIOB 포트의 3번 핀을 읽어와서 버튼의 상태를 감지 후 LOW(0) 상태이면
	    {
	    	button = 0; // 전후진 상태 = 0
	    	TxData[2] = button;
	    }
	    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==1) //  GPIOB 포트의 4번 핀을 읽어와서 버튼의 상태를 감지 후 HIGH(1) 상태이면
	    {
	    	button2 = 1; // 주행모드 변경 스위치 상태 = 1
	    	TxData[3] = button2;
	    }
	    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==0) // GPIOB 포트의 4번 핀을 읽어와서 버튼의 상태를 감지 후 LOW(0) 상태이면
	    {
	    	button2 = 0; // 주행모드 변경 스위치 상태 = 0
	    	TxData[3] = button2;
	    }
```
> 이 코드는 스위치 값을 읽어와 그 값을 TxData 배열에 저장 하는 코드이다.

* 전송하는지 확인하기 위한 led 작동

```c
if (NRF24_Transmit(TxData) == 1) // NRF24_Transmit 함수를 호출하여 반환하는 값이 1인 경우 (즉, 전송 성공한 경우)
	  	  {
	  		  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // : GPIOC 포트의 13번 핀 (LED가 연결된 핀)의 상태를 토글합니다.
	  		  HAL_Delay(100);
	  	  }
```
> 이 코드는 조건문을 사용하여 NRF24 모듈을 통해 데이터를 전송하고, 전송이 성공한 경우 LED를 깜박이는 기능을 수행한다.

* TxData[]의 각각의 의미
```c
TxData[0] , RxData[0]= 모터 부분의 가변저항 값
TxData[1] , RxData[1]= 핸들 부분의 가변저항 값
TxData[2] , RxData[2]= 전후진 모드변경 스위치 값
TxData[3] , RxData[3]= 주행모드 변경 스위치 값

```
<br> [위로](#STM32F411) <br>

## 수신부 코드

* Rxmode 설정

```c
void NRF24_RxMode (uint8_t *Address, uint8_t channel)
{
	// chip을 설정하기 전에 비활성화
	CE_Disable();

	// STATUS 레지스터 초기화
	nrf24_reset (STATUS);

	// RF 채널 선택
	nrf24_WriteReg (RF_CH, channel);  // select the channel

	// 데이터 파이프 2 선택
	uint8_t en_rxaddr = nrf24_ReadReg(EN_RXADDR);
	en_rxaddr = en_rxaddr | (1<<2);
	nrf24_WriteReg (EN_RXADDR, en_rxaddr);

// 데이터 파이프 1의 주소를 쓰면서 데이터 파이프 2의 LSB 주소를 설정.
// 데이터 파이프 2에서 데이터 파이프 5까지의 주소는 LSB를 제외하고 4바이트가 동일하다.

	nrf24_WriteRegMulti(RX_ADDR_P1, Address, 5);  // Pipe1 주소 설정
	nrf24_WriteReg(RX_ADDR_P2, 0xEE);  // Pipe2의 LSB 주소 설정

	nrf24_WriteReg (RX_PW_P2, 32);   // 파이프 2의 페이로드 크기를 32비트로 설정


	// 수신 모드에서 디바이스 전원 켬
	uint8_t config = nrf24_ReadReg(CONFIG);
	config = config | (1<<1) | (1<<0);
	nrf24_WriteReg (CONFIG, config);

	// 설정을 마친 후 chip 활성화
	CE_Enable();
}
```
> 해당 함수를 호출하면 NRF24 모듈이 데이터 수신을 위해 설정된다.


* NRF24 모듈의 레지스터 값을 읽고 배열에 저장
```c
void NRF24_ReadAll (uint8_t *data)
{
	for (int i=0; i<10; i++) // 처음 10개의 레지스터 값을 data 배열에 저장
	{
		*(data+i) = nrf24_ReadReg(i);
	}

	nrf24_ReadReg_Multi(RX_ADDR_P0, (data+10), 5); //data 배열에 RX 주소 파이프 0과 1의 주소 값을 저장
	nrf24_ReadReg_Multi(RX_ADDR_P1, (data+15), 5); //                     

	*(data+20) = nrf24_ReadReg(RX_ADDR_P2); // RX 주소 파이프 2부터 5까지의 주소 값을 각각 nrf24_ReadReg 함수를 사용하여 읽어와서 data 배열에 저장
	*(data+21) = nrf24_ReadReg(RX_ADDR_P3);
	*(data+22) = nrf24_ReadReg(RX_ADDR_P4);
	*(data+23) = nrf24_ReadReg(RX_ADDR_P5);

	nrf24_ReadReg_Multi(RX_ADDR_P0, (data+24), 5); // RX 주소 파이프 0의 주소 값을 읽어와서 data 배열에 저장

	for (int i=29; i<38; i++) //  29부터 37까지의 레지스터 값을 순회하며, nrf24_ReadReg 함수를 사용하여 해당 레지스터의 값을 읽어와서 data 배열에 저장
	{
		*(data+i) = nrf24_ReadReg(i-12);
	}

}
```

> 위 코드는 NRF24 모듈의 여러 레지스터 값을 읽어와서 data 배열에 저장하는 기능을 수행. 이렇게 저장된 레지스터 값들은 나중에 필요한 정보를 추출하는 데 사용될 수 있다.


* 데이터 수신가능 여부 확인
```c
uint8_t isDataAvailable (int pipenum) 
{
	uint8_t status = nrf24_ReadReg(STATUS); // 데이터가 수신 가능한지를 판단하기 위해 NRF24 모듈의 상태 레지스터 값을 읽는다.


	if ((status&(1<<6))&&(status&(pipenum<<1))) // RX FIFO가 비어있고 해당 파이프로부터 데이터가 수신되었을 경우
	{

		nrf24_WriteReg(STATUS, (1<<6)); // 상태 레지스터의 6번째 비트를 1로 설정하여 RX FIFO를 비움.

		return 1; // 데이터가 수신 가능한 상태임을 나타내는 1을 반환
	}

	return 0;
}
```
> 이 함수는 NRF24 모듈에서 데이터가 수신 가능한지 여부를 판단하여 그 결과를 반환한다. 이를 통해 데이터 수신 여부에 따라 특정 동작을 수행하는 등의 제어 작업을 할 수 있다.


* 데이터 수신 함수
```c
void NRF24_Receive (uint8_t *data)
{
	uint8_t cmdtosend = 0;

	// NRF24 모듈과 통신하는데 사용되는 CS (Chip Select) 핀을 활성화
	CS_Select();
	
	cmdtosend = R_RX_PAYLOAD; // 데이터 수신을 위해 NRF24 모듈에게 R_RX_PAYLOAD 명령을 보낼 준비.
	HAL_SPI_Transmit(NRF24_SPI, &cmdtosend, 1, 100); // SPI 통신을 사용하여 cmdtosend 변수의 값을 NRF24 모듈에게 보냄.

	HAL_SPI_Receive(NRF24_SPI, data, 32, 1000); // SPI 통신을 사용하여 NRF24 모듈로부터 데이터를 수신.

	CS_UnSelect(); //SPI 통신이 종료되었으므로 CS 핀을 비활성화하여 NRF24 모듈과의 통신을 종료.

	HAL_Delay(1);

	cmdtosend = FLUSH_RX;
	nrfsendCmd(cmdtosend);
}
```
> 이 함수는 NRF24 모듈로부터 데이터를 수신하고, 수신된 데이터를 data 배열에 저장한다. 또한, 데이터 수신 완료 후 RX FIFO 버퍼를 비우는 작업을 수행.

* NRF24 모듈로부터 데이터가 수신 가능한지를 확인하고, 수신 가능한 경우에는 데이터를 수신하여 RxData 배열에 저장하는 코드
```c
	  if (isDataAvailable(2) == 1)
	  {
		  NRF24_Receive(RxData);
	  }
```

> * isDataAvailable 함수를 사용하여 파이프 번호 2에서 데이터가 수신 가능한지를 확인
> * NRF24_Receive 함수를 호출하고, 수신된 데이터를 RxData 배열에 저장

## 기능 구현 영상

![](https://github.com/crasdok/capstone/assets/118472691/4d100a40-bb1d-4ec3-a9a1-659828b5c2a6)

> 핸들을 이용한 앞바퀴 조향

![뒷바퀴 엑셀](https://github.com/qkcvb110/Portfolio/assets/121782690/c9771932-f7c3-4f74-92a0-41dc8a3f20c6)
> 악셀을 이용한 뒷바퀴 속도 조절

## RF 통신중 Live Expression 화면 영상

![nrf-rx](https://github.com/sc11046/adas_with_can_nrf/assets/121782720/b76b45c1-343f-49d7-9c34-c52e42e75971)

<br> [위로](#STM32F411) <br>
