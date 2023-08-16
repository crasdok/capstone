# FutureLink: Smart Communication in ADAS-Equipped Electric Vehicle with CAN and NRF
![header](https://capsule-render.vercel.app/api?type=shark&color=auto&height=300&section=header&text=Future%20Link&fontSize=90)

##  1.목차 

-   [프로젝트 소개](#2.-프로젝트-소개)
-   [기술 및 도구](#-기술-및-도구)
-   [작품 소개](#-작품-소개)
-   [기능 구현 영상](#-기능-구현-영상)
  - [CAN,CAN-FD통신](#1-CAN,CAN-FD-통신)
  - [RF통신](#2-RF-통신)
  - [Lane Keeping Assist](#2-Lane-Keeping-Assist)
  - [라이다 충돌경고](#2-라이다-충돌경고)
  - [초음파 충돌경고](#2-초음파-충돌경고)
  - [자동 헤드라이트](#2-자동-헤드라이트)


## 2.프로젝트 소개
### STM32 와 라즈베리파이를 이용하여 무선조종과 자율주행이 가능한 RC카를 구현했습니다.

<img width="40%" src="https://github.com/crasdok/capstone/assets/118472691/78160e14-c080-440e-9248-77b9b9e72d66"/>

## 기능들
  ### 1.초음파 센서를 이용해 장애물에 가까워졌을 때 부저 알림
  ### 2.조도 센서를 이용해 라이트 밝기 조절
  ### 3.라즈베리파이 의 OpenCv를 이용한 차선인식후 방향조절
  ### 4.NRF24L01을 이용한 원거리 통신
  ### 5.라이다센서를 이용해 앞쪽에 장애물이 있을 시 모터 정지

## 기능별

  ### -[라즈베리파이 송신부](https://github.com/crasdok/capstone/blob/main/RaspberryPi_Tx/RaspberryPi_Tx.py)
: 차선인식 후 CAN통신

  ### -[STM32 RF통신](https://github.com/crasdok/capstone/tree/main/STM32F411_TX)
: RF통신

  ### -[STM32 모터](https://github.com/crasdok/capstone/tree/main/STM32H7A3ZI_1_RX)
: 받은 정보로 기능구현

  ### -[STM32 초음파,조도](https://github.com/crasdok/capstone/tree/main/STM32H7A3ZI_2_TX)
: 초음파 조도 값을 CAN통신

  ### -[STM32 라이다](https://github.com/crasdok/capstone/tree/main/STM32H7A3ZI_3_TX)
: 라이다값 CAN통신


