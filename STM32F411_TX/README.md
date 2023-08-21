# STM32F411

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)

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

## 주요 코드 



> TX주소와 값의 길이 설정
```c
uint8_t TxAddress[] = {0xEE,0xDD,0xCC,0xBB,0xAA};
uint8_t TxData[32];
```
> NRF24 Init
```c
void NRF24_Init (void)
{
	// disable the chip before configuring the device
	CE_Disable();


	// reset everything
	nrf24_reset (0);

	nrf24_WriteReg(CONFIG, 0);  // will be configured later

	nrf24_WriteReg(EN_AA, 0);  // No Auto ACK

	nrf24_WriteReg (EN_RXADDR, 0);  // Not Enabling any data pipe right now

	nrf24_WriteReg (SETUP_AW, 0x03);  // 5 Bytes for the TX/RX address

	nrf24_WriteReg (SETUP_RETR, 0);   // No retransmission

	nrf24_WriteReg (RF_CH, 0);  // will be setup during Tx or RX

	nrf24_WriteReg (RF_SETUP, 0x0E);   // Power= 0db, data rate = 2Mbps

	// Enable the chip after configuring the device
	CE_Enable();

}
```

> TX모드 설정
```c

void NRF24_TxMode (uint8_t *Address, uint8_t channel)
{
	// disable the chip before configuring the device
	CE_Disable();

	nrf24_WriteReg (RF_CH, channel);  // select the channel

	nrf24_WriteRegMulti(TX_ADDR, Address, 5);  // Write the TX address


	// power up the device
	uint8_t config = nrf24_ReadReg(CONFIG);
	config = config | (1<<1);   // write 1 in the PWR_UP bit
//	config = config & (0xF2);    // write 0 in the PRIM_RX, and 1 in the PWR_UP, and all other bits are masked
	nrf24_WriteReg (CONFIG, config);

	// Enable the chip after configuring the device
	CE_Enable();
}
```

> ADC값 변환 후 가공
```c
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
TxData[0] = HAL_ADC_GetValue(&hadc1)/27;
TxData[1] = HAL_ADC_GetValue(&hadc1)/40;
```

> 모드변경 스위치 부분
```c
if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==1)
	    {
	    	button = 1;
	    	TxData[2] = button;
	    }
	    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==0)
	    {
	    	button = 0;
	    	TxData[2] = button;
	    }
```

> 전송하는지 확인하기 위한 led 작동


```c
if (NRF24_Transmit(TxData) == 1)
	  	  {
	  		  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	  		  HAL_Delay(100);
	  	  }
```

<br> [위로](#STM32F411) <br>
