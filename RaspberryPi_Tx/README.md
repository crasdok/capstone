# 라즈베리파이 

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)

## 사용 보드 및 모듈

<img width="30%" src="https://github.com/crasdok/capstone/assets/118472691/1c0dd492-0efd-4748-a8c1-77e61ac538ec"/> <img width="30.7%" src="https://github.com/crasdok/capstone/assets/118472691/2d89bfdc-80fc-41d4-bb9b-ff89ef32365d"/>

<img width="23%" src="https://github.com/crasdok/capstone/assets/118472691/3fff296b-3e77-458b-8395-f9b87f5abd2a"/> <img width="30%" src="https://github.com/crasdok/capstone/assets/118472691/601356d0-afae-4b8b-b301-c7f703c0714d"/>

> 라즈베리파이 4B+,   MCP2515 CAN BUS module, 웹캠, 라즈베리파이용 디스플레이

### 순서도

<img width="70%" src="https://github.com/crasdok/capstone/assets/118472691/dbffaf9c-016d-4d24-91bf-44b46e3e6d16"/>


###  주요 기능 
------------
* 차선 인식
> 흑백화
```python
gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
```
> 모서리 검출
```python
can = cv2.Canny(gray, 50, 200, None, 3)
```
> 관심구역 설정
```python
height, width = can.shape[:2]
top_left = (width // 12, height)
top_right = (width * 11 // 12, height)
bottom_right = (width * 11 // 12, height * 2 // 3)
bottom_left = (width // 12, height * 2 // 3)
roi_vertices = [top_left, top_right, bottom_right, bottom_left]


> 직선 검출
```python
line_arr = cv2.HoughLinesP(masked_image, 1, np.pi / 180, 20, minLineLength=10, maxLineGap=10)
```
> 원본에 합성
```python
mimg = cv2.addWeighted(src, 1, ccan, 1, 0)
```

<br>

* 방향제어
> arctan2 함수를 사용하여 각도를 계산하여 각도에 따른 진행 방향을 결정
```python
 line_arr2[i] = np.append(line_arr[i], np.array((np.arctan2(l[1] - l[3], l[0] - l[2]) * 180) / np.pi))
```

* CAN통신
> 값 전달
```python
message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x52])
bus.send(message, timeout=0.2)
```


> 방향제어 예시

<img width="50%" src="https://github.com/crasdok/capstone/assets/118472691/580509e4-9024-4171-8904-21c613dff041"/>

### 사용 기술
------------
* Raspberry Pi
* Python
* OpenCV
  > OpenCV(Open Source Computer Vision Library)는 컴퓨터 비전 및 이미지 처리를 위한 오픈 소스 라이브러리입니다. 주로 실시간 컴퓨터 비전 작업, 영상 처리, 객체 감지, 얼굴 인식, 모션 추적 등 다양한 응용 분야에서 사용됩니다.
* Hough Transform
  > 허프 변환(Hough Transform)은 이미지에서 직선이나 다른 형태의 기하학적 모양을 검출하는데 사용되는 컴퓨터 비전 기술입니다. 주로 이미지에서 선이나 원과 같은 모양을 감지하는 데 활용되며, 특히 이미지에서 노이즈나 각도 변화 등으로 인해 정확한 픽셀 위치로부터 모양을 찾기 어려운 경우에 유용합니다.
* Canny Edge Detection
  > Canny Edge Detection은 이미지에서 엣지(경계)를 검출하는데 사용되는 컴퓨터 비전 기술 중 하나로, 엣지가 뚜렷하게 검출되며 노이즈에 강한 결과를 얻을 수 있는 방법으로 널리 사용됩니다.
* 이미지 가져오기 및 전처리
* 색상 및 채도 필터링

<br> [위로](#라즈베리파이) <br>

