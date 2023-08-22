# 라즈베리파이 

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)

## 사용 보드 및 모듈

<img width="30%" src="https://github.com/crasdok/capstone/assets/118472691/1c0dd492-0efd-4748-a8c1-77e61ac538ec"/> <img width="30.7%" src="https://github.com/crasdok/capstone/assets/118472691/2d89bfdc-80fc-41d4-bb9b-ff89ef32365d"/>

<img width="23%" src="https://github.com/crasdok/capstone/assets/118472691/3fff296b-3e77-458b-8395-f9b87f5abd2a"/> <img width="30%" src="https://github.com/crasdok/capstone/assets/118472691/601356d0-afae-4b8b-b301-c7f703c0714d"/>

> 라즈베리파이 4B+,   MCP2515 CAN BUS module, 웹캠, 라즈베리파이용 디스플레이

## 각각의 보드와 모듈을 사용한 이유
|보드, 모듈| 설명                                                         |
| ------- | ------------------------------------------------------------ |
| 라즈베리파이 4B+ | 실시간으로 차선을 인식 해야하기 때문에 높은 램을 가진 이 사용했다. |
| MCP2515 | CAN 버스 네트워크에서 신호 변환, 신호 강화 및 수신, 전송제어 등의 역할을 수행한다. |
| 웹캠 | 실시간 영상을 촬영하기 위해 사용한다. |
| 라즈베리파이용 디스플레이  | 촬영한 영상을 화면에 나타내기 위해 사용한다. |

### 순서도

<img width="70%" src="https://github.com/crasdok/capstone/assets/118472691/dbffaf9c-016d-4d24-91bf-44b46e3e6d16"/>


###  주요 기능 
------------
## 차선 인식

* 흑백화
```python
gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
```
* 모서리 검출
```python
can = cv2.Canny(gray, 50, 200, None, 3)
```
* 관심구역 설정
```python
height, width = can.shape[:2]
top_left = (width // 12, height)
top_right = (width * 11 // 12, height)
bottom_right = (width * 11 // 12, height * 2 // 3)
bottom_left = (width // 12, height * 2 // 3)
roi_vertices = [top_left, top_right, bottom_right, bottom_left]
```

* 직선 검출
```python
line_arr = cv2.HoughLinesP(masked_image, 1, np.pi / 180, 20, minLineLength=10, maxLineGap=10)
```

* Hough 변환을 사용하여 왼쪽과 오른쪽차선을 구분, 기울기를 계산
```python
    line_R = np.empty((0, 5), int)
    line_L = np.empty((0, 5), int)
    if line_arr is not None:
        line_arr2 = np.empty((len(line_arr), 5), int)
        for i in range(0, len(line_arr)):
            temp = 0
            l = line_arr[i][0]
            line_arr2[i] = np.append(line_arr[i], np.array((np.arctan2(l[1] - l[3], l[0] - l[2]) * 180) / np.pi))
            if line_arr2[i][1] > line_arr2[i][3]:
                temp = line_arr2[i][0], line_arr2[i][1]
                line_arr2[i][0], line_arr2[i][1] = line_arr2[i][2], line_arr2[i][3]
                line_arr2[i][2], line_arr2[i][3] = temp
            if line_arr2[i][0] < 320 and (abs(line_arr2[i][4]) < 170 and abs(line_arr2[i][4]) > 95):
                line_L = np.append(line_L, line_arr2[i])
            elif line_arr2[i][0] > 320 and (abs(line_arr2[i][4]) < 170 and abs(line_arr2[i][4]) > 95):
                line_R = np.append(line_R, line_arr2[i])
    line_L = line_L.reshape(int(len(line_L) / 5), 5)
    line_R = line_R.reshape(int(len(line_R) / 5), 5)

    # 중앙과 가까운 오른쪽, 왼쪽 선을 최종 차선으로 인식
    try:
        line_L = line_L[line_L[:, 0].argsort()[-1]]
        degree_L = line_L[4]
        cv2.line(ccan, (line_L[0], line_L[1]), (line_L[2], line_L[3]), (255, 0, 0), 10, cv2.LINE_AA)
    except:
        degree_L = 0
    try:
        line_R = line_R[line_R[:, 0].argsort()[0]]
        degree_R = line_R[4]
        cv2.line(ccan, (line_R[0], line_R[1]), (line_R[2], line_R[3]), (255, 0, 0), 10, cv2.LINE_AA)
    except:
        degree_R = 0
```
> * line_R과 line_L은 각각 오른쪽 차선과 왼쪽 차선을 저장할 배열.
> * line_arr이 비어있지 않으면, line_arr2 배열을 생성하고 각 직선의 정보에 각도 정보를 추가. 이 각도는 np.arctan2를 사용하여 계산된다.
> * line_arr2[i][1] > line_arr2[i][3] 조건을 통해 차선의 두 점을 x 좌표에 따라 정렬. 이는 왼쪽과 오른쪽을 구분하기 위해 필요한 조작이다.
> * line_arr2[i][0]의 위치에 따라 차선을 왼쪽과 오른쪽으로 구분하며, 각도 조건을 만족하는 경우 해당 차선을 각각 line_L과 line_R에 추가.



* 원본에 합성
```python
mimg = cv2.addWeighted(src, 1, ccan, 1, 0)
```

<br>

* 방향제어
> arctan2 함수를 사용하여 각도를 계산하여 각도에 따른 진행 방향을 결정

```python
 line_arr2[i] = np.append(line_arr[i], np.array((np.arctan2(l[1] - l[3], l[0] - l[2]) * 180) / np.pi))
```

* 여러차례 테스트를 통해 차선 좌,우 인식률을 개선한 코드
```python
    if ret: // 카메라에서 프레임을 읽어오는데 성공했을 경우
        frame = cv2.resize(frame, (480, 270)) // 프레임의 크기를 (480, 270)으로 조정
        cv2.imshow('ImageWindow', DetectLineSlope(frame)[0]) // 화면에 이미지를 표시, DetectLineSlope 함수는 이미지 내에서 차선을 감지하고 그 결과를 반환. [0] 인덱스를 사용하여 이미지 결과를 선택
        l, r = DetectLineSlope(frame)[1], DetectLineSlope(frame)[2] // 함수의 반환값 중에서 왼쪽 차선의 기울기를 나타내는 degree_L과 오른쪽 차선의 기울기를 나타내는 degree_R을 가져옴.

        if l == 0 or r == 0:
            if (105 <= l and l<= 112) or (105 <= r and r<= 112) :
                print('right')
            elif (112 < l and l<= 200) or (112 < r and r<= 200) :
                print('Hard right')
            elif (90 < abs(l) and abs(l)< 105) or (90 < abs(r) and abs(r)< 105) :
                print('go')
            elif ( l < -113 ) or ( r < -113) :
                print('Hard left')
            else :
                print('left')

        elif l == 0 and r == 0  :
            print('go')
        elif (90 < abs(l) and abs(l)< 105) and (90 < abs(r) and abs(r)< 105) :
            print('go')
         elif (105 <= l and l<= 112) and (95 <= r and r<= 112) :
            print('right')
        elif (112 < l and l<= 200) and (98 <= r and r<= 200) :
            print('Hard right')
        elif ( l < -113 ) and ( r < -113) :
            print('Hard left')

        else :
            print('left')
```
> 차선 감지 결과인 degree_L과 degree_R 값을 기반으로 다양한 상황에 따라 조향 명령을 생성


* 'q' 키가 눌렸을 때 루프를 종료하기 위한 코드
```python
if cv2.waitKey(1) & 0xff == ord('q'): 
   break
```


> 방향제어 예시

<img width="50%" src="https://github.com/crasdok/capstone/assets/118472691/580509e4-9024-4171-8904-21c613dff041"/>

### 문제점 및 개선점

<img width="60%" src="https://github.com/crasdok/capstone/assets/118472691/f9c37564-282a-4767-b809-39d998f18080"/>

> * 차량의 보닛에 카메라를 달자 1m 정도 앞의 차선을 인식.
> * 차량과 차선의 크기를 고려하여 높은 위치에서 앞의 차선을 인식하게 함.

### 사용 기술
------------
* Raspberry Pi
* Python
* OpenCV
  > OpenCV(Open Source Computer Vision Library)는 컴퓨터 비전 및 이미지 처리를 위한 오픈 소스 라이브러리이다. 주로 실시간 컴퓨터 비전 작업, 영상 처리, 객체 감지, 얼굴 인식, 모션 추적 등 다양한 응용 분야에서 사용.
* Hough Transform
  > 허프 변환(Hough Transform)은 이미지에서 직선이나 다른 형태의 기하학적 모양을 검출하는데 사용되는 컴퓨터 비전 기술이다. 주로 이미지에서 선이나 원과 같은 모양을 감지하는 데 활용되며, 특히 이미지에서 노이즈나 각도 변화 등으로 인해 정확한 픽셀 위치로부터 모양을 찾기 어려운 경우에 유용하다.
* Canny Edge Detection
  > Canny Edge Detection은 이미지에서 엣지(경계)를 검출하는데 사용되는 컴퓨터 비전 기술 중 하나로, 엣지가 뚜렷하게 검출되며 노이즈에 강한 결과를 얻을 수 있는 방법으로 널리 사용된다.
* 이미지 가져오기 및 전처리
* 색상 및 채도 필터링


### 차선 인식 라즈베리 영상

<br> [위로](#라즈베리파이) <br>

