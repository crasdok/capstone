# 라이다센서 STM32

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)

## HardWare

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

## SoftWare
* getData 함수를 이용해 거리, 온도, 신호 강도를 측정
```c
getData(&TF_Luna_1, &tfDist, &tfFlux, &tfTemp);
```

* 측정 값을 FDCAN을 통해 송신 하는 코드
```c
sprintf ((char *)TxData_Node1_To_Node3," %d%d%d",Dist1,Dist2,Dist3);
  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData_Node1_To_Node3)!= HAL_OK)
	  	{
	  		  		 Error_Handler();
	  	}
	  	HAL_Delay(100);
```

<br> [위로](#라이다센서-STM32) <br>
