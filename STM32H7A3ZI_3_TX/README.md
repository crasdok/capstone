# 라이다센서 STM32

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)

## 라이다 센서의 작동원리 및 동작과정

1. 레이저 신호 발사: 라이다 센서는 레이저 빛을 생성하여 주변 환경으로 방사한다. 이 레이저 빛은 빛의 속도로 빠르게 이동하며, 센서에서 발사된 레이저 신호는 주변 대상과 상호작용하게 된다.

2. 신호 반사 및 수집: 발사된 레이저 신호는 주변 대상(장애물, 지형 등)과 만나면 반사되어 센서로 되돌아온다. 이 반사 신호는 센서에 내장된 수신기로 수집된다.

3. 시간 측정: 수신된 반사 신호의 시간 차이를 측정함으로써 레이저 신호가 대상까지 이동하고 다시 센서로 돌아오는 데 걸린 시간을 측정한다. 이 시간 차이를 "Time of Flight (TOF)"라고 한다.

4. 거리 계산: 시간 측정을 통해 측정된 TOF 값을 사용하여 거리를 계산한다. 빛의 속도는 알려져 있기 때문에 TOF 값을 이용하여 대상과 센서 사이의 거리를 정확하게 계산할 수 있다.

### 사용 보드 및 모듈
<img width="20%" img src="https://github.com/crasdok/capstone/assets/118472691/cfe702d3-d627-4ac1-bfec-9163f5f86fbb">
<img width="25%" img src="https://github.com/crasdok/capstone/assets/118472691/989a92d2-c85b-40db-a826-f4c484fbc64b">
<img width="35%" img src="https://github.com/crasdok/capstone/assets/118472691/78d4aa5e-f855-47ce-a841-347e114cb7a8">

### 메인 박스 

<img width="40%" img src="https://github.com/crasdok/capstone/assets/118472691/7f0f0740-1cec-481f-b73c-b1ab1f477958">

> 위쪽의 오른쪽 보드가 이 보드

| 기능           | 설명                                                         |
| -------------- | ------------------------------------------------------------ |
| CAN Transciever | CAN 버스 네트워크에서 신호 변환, 신호 강화 및 수신, 전송제어 등의 역할을 수행한다. |
| Lidar | 초음파 센서보다 정확한 거리 측정이 가능하다 |

## 코드부분

* getData 함수를 이용해 거리, 온도, 신호 강도를 측정
```c
getData(&TF_Luna_1, &tfDist, &tfFlux, &tfTemp);
```

* 이 함수는 TF-Luna 라이다 센서로부터 거리, 플럭스, 온도 데이터를 읽어와서 변환하고, 특정 조건에 따라 상태를 업데이트하며 이상 데이터를 처리하는 역할을 합니다.
```c
bool getData(TF_Luna_Lidar *tf_luna, int16_t *dist, int16_t *flux, int16_t *temp)
{
    tfStatus = TFL_READY;    // 이전에 발생한 오류 상태를 초기화


    for (uint8_t reg = TFL_DIST_LO; reg <= TFL_TEMP_HI; reg++) // 센서의 레지스터 TFL_DIST_LO부터 TFL_TEMP_HI까지 순회해 데이터를 읽음
    {
      if( !readReg(tf_luna, reg)) return false; // readReg 함수를 호출하여 레지스터에서 데이터를 읽는데 실패하면 false를 반환, 함수 종료
          else dataArray[ reg] = regReply; // 데이터를 읽어오는데 성공하면 dataArray 배열에 해당 레지스터의 데이터를 저장
    }


   *dist = dataArray[ 0] + ( dataArray[ 1] << 8);
   *flux = dataArray[ 2] + ( dataArray[ 3] << 8);
   *temp = dataArray[ 4] + ( dataArray[ 5] << 8);


   *temp = *temp / 100;
  //  *temp = *temp * 9 / 5 + 32;
    // Then convert Celsius to degrees Fahrenheit


    // 신호 강도가 100 이하인 경우 tfStatus를 TFL_WEAK로 설정하고 false를 반환
    if( *flux < (int16_t)100)
    {
      tfStatus = TFL_WEAK;
      return false;
    }
    신호 강도가 0xFFFF인 경우 (데이터가 saturation되었을 경우) tfStatus를 TFL_STRONG로 설정하고 false를 반환
    else if( *flux == (int16_t)0xFFFF)
    {
      tfStatus = TFL_STRONG;
      return false;
    }
    else // 그 외의 경우 tfStatus를 TFL_READY로 설정하고 true를 반환
    {
      tfStatus = TFL_READY;
      return true;
    }

}
```
> *dist = dataArray[0] + (dataArray[1] << 8);: dataArray의 0번째와 1번째 인덱스의 데이터를 읽어 dist 변수에 저장한다. 두 데이터를 16비트로 합쳐 거리 값을 생성. 나머지 데이터 처리는 사용하지 않기 때문에 설명X


* 측정 값을 FDCAN을 통해 송신 하는 코드
```c
sprintf ((char *)TxData_Node1_To_Node3," %d%d%d",Dist1,Dist2,Dist3);
  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData_Node1_To_Node3)!= HAL_OK)
	  	{
	  		  		 Error_Handler();
	  	}
	  	HAL_Delay(100);
```
> CAN에 대한 자세한 사항은 CAN.md에서 확인

<br> [위로](#라이다센서-STM32) <br>
