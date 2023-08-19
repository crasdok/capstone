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



* 악셀값과 핸들값 송신부 사진

  <img width="50%" img src="https://github.com/crasdok/capstone/assets/118472691/a5820f42-f078-459c-b509-c5edebdb0df2">

| 기능           | 설명                                                         |
| -------------- | ------------------------------------------------------------ |
| NRF24L01 | 토글 스위치와 가변저항 각각의 값을 RF통신으로 STM32H7A3ZI보드에 보낸다. |
| 토글 스위치 | 전후진 과 주행모드를 선택할 수 있게 해준다. |
| 가변저항  | 악셀이 어느정도 밟았는지 와 핸들이 어느정도 위치에 있는지 값으로 나타내준다. |



> TX주소와 값의 길이 설정
```c
uint8_t TxAddress[] = {0xEE,0xDD,0xCC,0xBB,0xAA};
uint8_t TxData[32];
```

> TX모드 설정
```c
NRF24_TxMode(TxAddress, 10);
```

> ADC값 변환 후 가공
```c
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
TxData[0] = HAL_ADC_GetValue(&hadc1)/27;
TxData[1] = HAL_ADC_GetValue(&hadc1)/40;
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
