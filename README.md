# FutureLink: Smart Communication in ADAS-Equipped Electric Vehicle with CAN and NRF
![header](https://capsule-render.vercel.app/api?type=shark&color=auto&height=300&section=header&text=Future%20Link&fontSize=90)

##  1.목차 

- [포트폴리오 개요](#포트폴리오-개요)
- [Function Diagram](#Function-Diagram)
- [프로젝트 구상도](#프로젝트-구상도)
- [기능 구현](#기능구현)
  - CAN,CAN-FD통신
  - RF통신
  - 차선인식
  - 라이다 충돌경고
  - 초음파 충돌경고
  - 자동 헤드라이트
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

## 기술 및 도구

<img src="https://img.shields.io/badge/STM32-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white"> <img src="https://img.shields.io/badge/raspberrypi-A22846?style=for-the-badge&logo=raspberrypi&logoColor=white"> <img src="https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=C&logoColor=white"> <img src="https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=Python&logoColor=white"> <img src="https://img.shields.io/badge/GITHUB-181717?style=for-the-badge&logo=github&logoColor=white"> 

## 프로젝트 목표

### STM32 와 라즈베리파이를 이용하여 ADAS시스템을 구현하고 무선조종과 자율주행이 가능한 차량을 만들었습니다

---

## Function Diagram

<img src="https://github.com/crasdok/capstone/assets/118472691/7d55455e-2d82-4735-b75a-c6cfbd8d08fb">

> CAN통신 값을 통해 실행 되는 센서


<br> [목차로 돌아가기](#1목차) <br>


---

## 프로젝트 구상도

![구상도](https://github.com/crasdok/capstone/assets/118472691/957f725b-7a13-4690-96ac-83ff8acd3b38)

> 조도,초음파센서 값을 보내는 STM32와 차선인식 값을 보내는 라즈베리파이, 주행모드,악셀,핸들 값을 보내는 1개의 STM32F411보드, 모든 값을 받는 STM32로 이루어져 있다.

<br> [목차로 돌아가기](#1목차) <br>

---

## 기능구현

  ### 1.초음파 센서를 이용해 장애물에 가까워졌을 때 부저 알림

<img width="30%" src="https://github.com/crasdok/capstone/assets/118472691/69ab3329-fc5e-4d6d-9518-1cb6826d1e31"/><img width="30%" src="https://github.com/crasdok/capstone/assets/118472691/2b596211-5e08-41d8-b036-ee912e681f41"/><img width="30%" src="https://github.com/crasdok/capstone/assets/118472691/a3dd97c7-925e-4b5d-aa1d-d159df591aff"/>

![부저 (1)](https://github.com/qkcvb110/Portfolio/assets/121782690/8dcb642f-5dad-4d8f-a2a7-7644101f6e36)
> 동영상으로 표현을 위해 LED로 대체 좌,우,뒷 부분에 각각 초음파 센서 1개 씩
  
  ### 2.조도 센서를 이용해 라이트 밝기 조절

   ![조도_AdobeExpress_AdobeExpress](https://github.com/crasdok/capstone/assets/118472691/c5a6ff02-2a8e-413e-a355-f17a2049f019)
   
  ### 3.라즈베리파이 의 OpenCv를 이용한 차선인식후 방향조절

  ![차선인식1_AdobeExpress_AdobeExpress](https://github.com/crasdok/capstone/assets/118472691/1354c4d5-52e4-4307-b605-7505c764afac) ![차선인식2_AdobeExpress_AdobeExpress](https://github.com/crasdok/capstone/assets/118472691/87baa6f4-f911-4d27-be59-0a3b4d04b713)

  

  
  ### 4.NRF24L01을 이용한 원거리 통신

  ![NRF영상_AdobeExpress_AdobeExpress](https://github.com/crasdok/capstone/assets/118472691/0e8da015-865e-457a-8531-79db8783a3ea)

  
  
  ### 5.라이다센서를 이용해 앞쪽에 장애물이 있을 시 모터 정지

 ![라이다_AdobeExpress_AdobeExpress](https://github.com/qkcvb110/Portfolio/assets/121782690/8e93a8c7-8ef5-458a-87c5-c8af9d7982b0)

<br> [목차로 돌아가기](#1목차) <br>


---



## 기능별 코드

  * [라즈베리파이 송신부](https://github.com/crasdok/capstone/blob/main/RaspberryPi_Tx)
> 차선인식 후 CAN통신

  * [STM32 RF통신](https://github.com/crasdok/capstone/tree/main/STM32F411_TX)
> RF통신

  * [STM32 모터](https://github.com/crasdok/capstone/tree/main/STM32H7A3ZI_1_RX)
> 받은 정보로 기능구현

  * [STM32 초음파,조도](https://github.com/crasdok/capstone/tree/main/STM32H7A3ZI_2_TX)
> 초음파 조도 값을 CAN통신

  * [STM32 라이다](https://github.com/crasdok/capstone/tree/main/STM32H7A3ZI_3_TX)
> 라이다값 CAN통신

* [CAN 통신](https://github.com/crasdok/capstone/blob/main/CAN.md)
> CANFD,CAN에 관하여

<br> [목차로 돌아가기](#1목차) <br>

---


