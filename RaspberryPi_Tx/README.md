# 라즈베리파이 

### [목차로](https://github.com/crasdok/capstone/blob/main/README.md)

## 사용 보드

<img width="30%" src="https://github.com/crasdok/capstone/assets/118472691/1c0dd492-0efd-4748-a8c1-77e61ac538ec"/>

###  주요 기능 
------------
* 차선 인식
> 흑백화
```
gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
```
> 모서리 검출
```
can = cv2.Canny(gray, 50, 200, None, 3)
```
> 관심구역 설정
```
height, width = can.shape[:2]
top_left = (width // 12, height)
top_right = (width * 11 // 12, height)
bottom_right = (width * 11 // 12, height * 2 // 3)
bottom_left = (width // 12, height * 2 // 3)
roi_vertices = [top_left, top_right, bottom_right, bottom_left]

```
> 직선 검출
```
line_arr = cv2.HoughLinesP(masked_image, 1, np.pi / 180, 20, minLineLength=10, maxLineGap=10)
```
> 원본에 합성
```
mimg = cv2.addWeighted(src, 1, ccan, 1, 0)
```

<br>

* 방향제어
> arctan2 함수를 사용하여 각도를 계산하여 각도에 따른 진행 방향을 결정
```
 line_arr2[i] = np.append(line_arr[i], np.array((np.arctan2(l[1] - l[3], l[0] - l[2]) * 180) / np.pi))
```

> 방향제어 예시

<img width="50%" src="https://github.com/crasdok/capstone/assets/118472691/580509e4-9024-4171-8904-21c613dff041"/>

### 사용 기술
------------
* Raspberry Pi
* Python
* OpenCV
* Hough Transform
* Canny Edge Detection
* 이미지 가져오기 및 전처리
* 색상 및 채도 필터링

<br> [위로](#라즈베리파이) <br>

