import cv2
import numpy as np
import can
import time
# create a bus instance
# many other interfaces are supported as well (see documentation)
bus = can.Bus(interface='socketcan',
              channel='can0',
              receive_own_messages=True)

def DetectLineSlope(src):
    # 흑백화
    gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)

    # 모서리 검출
    can = cv2.Canny(gray, 50, 200, None, 3)

    # 관심 구역 설정
    # height = can.shape[0]
    # rectangle = np.array([[(0, height), (120, 300), (520, 300), (640, height)]])
    # mask = np.zeros_like(can)
    # cv2.fillPoly(mask, rectangle, 255)
    # masked_image = cv2.bitwise_and(can, mask)
    # ccan = cv2.cvtColor(masked_image, cv2.COLOR_GRAY2BGR)
    
    height, width = can.shape[:2]
    top_left = (width // 12, height)
    top_right = (width * 11 // 12, height)
    bottom_right = (width * 11 // 12, height * 2 // 3)
    bottom_left = (width // 12, height * 2 // 3)
    # roi_vertices = [(0, height), (width // 3, height // 3), (width*2 // 3, height*2 // 3), (width, height)]
    roi_vertices = [top_left, top_right, bottom_right, bottom_left]

    mask = np.zeros_like(can)
    cv2.fillPoly(mask, np.int32([roi_vertices]), 255)
    roi = cv2.bitwise_and(can, mask)
    ccan = cv2.cvtColor(roi, cv2.COLOR_GRAY2BGR)
    lower_white = np.array([0, 0, 180])
    upper_white = np.array([255, 30, 255])
    hsv = cv2.cvtColor(src, cv2.COLOR_BGR2HSV)
    white_mask = cv2.inRange(hsv, lower_white, upper_white)
    filtered_roi = cv2.bitwise_and(roi, roi, mask=white_mask)
    
    #가우시안 블러(추가한 것 성능차이 잘 모르겠음)
    # ccan = cv2.GaussianBlur(ccan, (5, 5), 0)
    

    # 직선 검출
    line_arr = cv2.HoughLinesP(roi, 1, np.pi / 180, 20, minLineLength=10, maxLineGap=10)

    # line color
    # color = [0, 0, 255]
    # thickness = 5
    # for line in line_arr:
    #   for x1, y1, x2, y2 in line:
    #        cv2.line(ccan, (x1, y1), (x2, y2), color, thickness)

    # 중앙을 기준으로 오른쪽, 왼쪽 직선 분리
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

    # 원본에 합성
    mimg = cv2.addWeighted(src, 1, ccan, 1, 0)
    return mimg, degree_L, degree_R


cap = cv2.VideoCapture(0)

while cap.isOpened():

    ret, frame = cap.read()
    # 상하좌우 반전
    # frame = cv2.flip(frame, -1)

    if ret:
        frame = cv2.resize(frame, (480, 270))
        cv2.imshow('ImageWindow', DetectLineSlope(frame)[0])
        l, r = DetectLineSlope(frame)[1], DetectLineSlope(frame)[2]

        if l == 0 or r == 0:
            if (105 <= l and l<= 112) or (105 <= r and r<= 112) :
                print('right')
                print(abs(l),abs(r))
                print(l,r)
                # message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x72])
                message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x52])
                bus.send(message, timeout=0.2)
            elif (112 < l and l<= 200) or (112 < r and r<= 200) :
                print('Hard right')
                print(abs(l),abs(r))
                print(l,r)
                message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x52])
                bus.send(message, timeout=0.2)
            elif (90 < abs(l) and abs(l)< 105) or (90 < abs(r) and abs(r)< 105) :
                print('go')
                print(abs(l),abs(r))
                print(l,r)
                message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x47])
                bus.send(message, timeout=0.2)
                
            elif ( l < -113 ) or ( r < -113) :
                print('Hard left')
                print(abs(l),abs(r))
                print(l,r)
                message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x4C])
                bus.send(message, timeout=0.2)
            else :
                print('left')
                print(abs(l),abs(r))
                print(l,r)
                # message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x6C])
                message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x4C])
                bus.send(message, timeout=0.2)
        elif l == 0 and r == 0  :
            print('go')
            print(abs(l),abs(r))
            print(l,r)
            message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x47])
            bus.send(message, timeout=0.2)
        elif (90 < abs(l) and abs(l)< 105) and (90 < abs(r) and abs(r)< 105) :
            print('go')
            print(abs(l),abs(r))
            print(l,r)
            message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x47])
            bus.send(message, timeout=0.2)
        
        
        # elif (110 <= l and l<= 200) and (110 <= r and r<= 200) :
        elif (105 <= l and l<= 112) and (95 <= r and r<= 112) :
            print('right')
            print(abs(l),abs(r))
            print(l,r)
            message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x52])
            bus.send(message, timeout=0.2)
        elif (112 < l and l<= 200) and (98 <= r and r<= 200) :
            print('Hard right')
            print(abs(l),abs(r))
            print(l,r)
            message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x52])
            bus.send(message, timeout=0.2)
        elif ( l < -113 ) and ( r < -113) :
                print('Hard left')
                print(abs(l),abs(r))
                print(l,r)
                message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x4C])
                bus.send(message, timeout=0.2)
        else :
            print('left')
            print(abs(l),abs(r))
            print(l,r)
            message = can.Message(arbitration_id=0x44, is_extended_id=False,data=[0x4C])
            bus.send(message, timeout=0.2)

        if cv2.waitKey(1) & 0xff == ord('q'):
            break

cv2.destroyAllWindows()

#sudo ifconfig can0 txqueuelen 1000