# 라이다센서 STM32

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)


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
    tfStatus = TFL_READY;    // clear status of any error condition

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Step 1 - Use the `HAL_I2C_MASTER_Receive` function `readReg` to fill the six byte
    // `dataArray` from the contiguous sequence of registers `TFL_DIST_LO`
    // to `TFL_TEMP_HI` that declared in the header file 'tfluna_i2c.h`.
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    for (uint8_t reg = TFL_DIST_LO; reg <= TFL_TEMP_HI; reg++)
    {
      if( !readReg(tf_luna, reg)) return false;
          else dataArray[ reg] = regReply;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Step 2 - Shift data from read array into the three variables
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   *dist = dataArray[ 0] + ( dataArray[ 1] << 8);
   *flux = dataArray[ 2] + ( dataArray[ 3] << 8);
   *temp = dataArray[ 4] + ( dataArray[ 5] << 8);



    // Convert temperature from hundredths
    // of a degree to a whole number
   *temp = *temp / 100;
  //  *temp = *temp * 9 / 5 + 32;
    // Then convert Celsius to degrees Fahrenheit


    // - - Evaluate Abnormal Data Values - -
    // Signal strength <= 100
    if( *flux < (int16_t)100)
    {
      tfStatus = TFL_WEAK;
      return false;
    }
    // Signal Strength saturation
    else if( *flux == (int16_t)0xFFFF)
    {
      tfStatus = TFL_STRONG;
      return false;
    }
    else
    {
      tfStatus = TFL_READY;
      return true;
    }

}
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
