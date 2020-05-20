import cv2

font = cv2.FONT_ITALIC

try:
    cam = cv2.VideoCapture(1)
    cam.set(cv2.CAP_PROP_CONVERT_RGB, 1)
    face_cascade = cv2.CascadeClassifier("./haarcascade_frontalface_default.xml")  # 얼굴찾기 haar 파일

except:
    print("camera loading error")

# https://dodo-it.tistory.com/30

flag = 0
rect = (0, 0, 0, 0)
area = 0

def faceDetect():
    global flag, rect

    ret, frame = cam.read()

    if not ret:
        return
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    if len(faces) == 0:
        x, y, w, h = rect

        # 480 * 640
        if x < 80 or x > 400 or y < 40 or y > 600:
            flag -= 5
        else:
            flag -= 1

        if flag > 0:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)  # 사각형 범위
        else:
            return 0



    else:
        # cam.get(cv2.CAP_PROP_FPS) == 30 기준
        flag = 50
        rect = faces[0]
        # for (x, y, w, h) in faces:
        #     cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)  # 사각형 범위
        #     cv2.putText(frame, str(flag), (x - 5, y - 5), font, 0.5, (255, 255, 0), 2)  # 얼굴찾았다는 메시지

    return 1

    # # 카메라 영상 왼쪽위에 위에 셋팅된 info 의 내용 출력
    # cv2.putText(frame, str(flag), (5, 15), font, 0.5, (255, 0, 255), 1)
    #
    # # 출력
    # cv2.imshow("frame", frame)





if __name__ == '__main__':

    while True:
        print(faceDetect())

        # k = cv2.waitKey(1)
        # if k == 27:
        #     break

    # cam.release()
    # cv2.destroyAllWindows()


