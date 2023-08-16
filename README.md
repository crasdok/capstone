# FutureLink: Smart Communication in ADAS-Equipped Electric Vehicle with CAN and NRF
![header](https://capsule-render.vercel.app/api?type=shark&color=auto&height=300&section=header&text=Future%20Link&fontSize=90)

##  1.목차 

- [포트폴리오 개요](#포트폴리오-개요)
- [프로젝트 개요](#프로젝트-개요)
- [프로젝트 구상도](#프로젝트-구상도)
- [기능 구현](#기능구현)
  - [CAN,CAN-FD통신](#-CAN,CAN-FD-통신)
  - [RF통신](#2-RF-통신)
  - [Lane Keeping Assist](#2-Lane-Keeping-Assist)
  - [라이다 충돌경고]()
  - [초음파 충돌경고](#1초음파-센서를-이용해-장애물에-가까워졌을-때-부저-알림)
  - [자동 헤드라이트](#2조도-센서를-이용해-라이트-밝기-조절)
- [기능별 코드](#기능별-코드)

---

## 포트폴리오 개요

 **프로젝트:** Smart Communication in ADAS-Equipped Electric Vehicle with CAN-FD, CAN and RF

 **기획 및 제작:** 최원진, 강민성, 오승찬

 **제작 기간:** 2023.01 ~ 08.

 **주요 기능:** 원거리 무선조종, 라이다, 초음파 충돌경고

 **사용 기술:** CAN-FD, CAN, RF Communication

 **문의:** 1304ccy@naver.com

 <br> [목차로 돌아가기](#1목차) <br>

---

## 프로젝트 개요

---
## 프로젝트 구상도

![구상도](https://github.com/crasdok/capstone/assets/118472691/957f725b-7a13-4690-96ac-83ff8acd3b38)

- 2개의 STM32H7A3ZI-Q 보드와 1개의 라즈베리파이, 1개의 STM32F411보드로 1개의 STM32H7A3ZI-Q 보드에 정보를 보냄

---

## 기술 및 도구

<img src="https://img.shields.io/badge/STM32-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white"> <img src="https://img.shields.io/badge/raspberrypi-A22846?style=for-the-badge&logo=raspberrypi&logoColor=white"> <img src="https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=C&logoColor=white"> <img src="https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=Python&logoColor=white"> <img src="https://img.shields.io/badge/GITHUB-181717?style=for-the-badge&logo=github&logoColor=white"> 

## 프로젝트 목표

### STM32 와 라즈베리파이를 이용하여 무선조종과 자율주행이 가능한 RC카를 구현했습니다.
### 또한 실제차량에 들어가 있는 ADAS시스템을 구현해보았습니다.

<img width="40%" src="https://github.com/crasdok/capstone/assets/118472691/78160e14-c080-440e-9248-77b9b9e72d66"/>

<br> [목차로 돌아가기](#1목차) <br>

---

## 기능구현

  ### 1.초음파 센서를 이용해 장애물에 가까워졌을 때 부저 알림
   >기능 구현 영상
  ### 2.조도 센서를 이용해 라이트 밝기 조절
   >기능 구현 영상
  ### 3.라즈베리파이 의 OpenCv를 이용한 차선인식후 방향조절
   >기능 구현 영상
  ### 4.NRF24L01을 이용한 원거리 통신
   >기능 구현 영상
  ### 5.라이다센서를 이용해 앞쪽에 장애물이 있을 시 모터 정지
   >기능 구현 영상

<br> [목차로 돌아가기](#1목차) <br>


---



## 기능별 코드

  ### [라즈베리파이 송신부](https://github.com/crasdok/capstone/blob/main/RaspberryPi_Tx/RaspberryPi_Tx.py)
: 차선인식 후 CAN통신

  ### [STM32 RF통신](https://github.com/crasdok/capstone/tree/main/STM32F411_TX)
: RF통신

  ### [STM32 모터](https://github.com/crasdok/capstone/tree/main/STM32H7A3ZI_1_RX)
: 받은 정보로 기능구현

  ### [STM32 초음파,조도](https://github.com/crasdok/capstone/tree/main/STM32H7A3ZI_2_TX)
: 초음파 조도 값을 CAN통신

  ### [STM32 라이다](https://github.com/crasdok/capstone/tree/main/STM32H7A3ZI_3_TX)
: 라이다값 CAN통신




