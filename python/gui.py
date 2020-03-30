"""
※main함수에서 사용법
pip install pillow로 모듈설치
from gui import gui_main,brain_typo 함수를 추가한다
이미지 파일 경로(대략 22~26줄)은 자신의 폴더에 맞게 수정바람,폰트(28줄)도 프로젝트 경로에 추가해주세요!
gui_main((mode좌표),(emotion좌표),mood값,energy값)    ex)gui_main((50,50),(-30,-70),-60,30)
brain_typo('문장',hue값,투명도,(문장위치좌표),폰트크기) ex)brain_typo('반가워',70,100,(70,-30),40)
-->투명도는 0(투명)~100(불투명)사이의값
-->단어위치좌표 원점(0,0) max는(100,100), min는(-100,-100)임
-->글자 폰트 크기는 30~50정도가 적당한 사이즈 딱히 범위 제한은 없음 필요한 사이즈로 조절하시길
(del_typo함수는 print_typo 함수 실행 이후에 모든 리스트가 초기화되므로 필요가 없어졌음)
참고)458줄에 __main__ 코드 작성(cv.Waitkey(0)있어서 화면넘기면서 변화확인)
"""
from math import atan2
from numpy import double
from typing import NamedTuple
from PIL import ImageFont, ImageDraw, Image
import numpy as np
import cv2 as cv


whale_path = "img/whale.jpg"    #원형그래프 이미지 경로
stick_path = "img/stick.jpg"    #막대그래프 이미지 경로
emotion_grid_path = "img/emotion_grid.jpg"  #emotion_grid 이미지 경로
white_bg_path = "img/white_bg.jpg"  #흰배경 이미지 경로
black_bg_path = "img/black_bg.jpg"  #검은배경 이미지 경로

font_path = "./malgun.ttf"  #폰트경로

PI = 3.14
#scalar 색 정의(b,g,r)
black = (0, 0, 0)
red = (0, 0, 255)
green = (0, 180, 0)
blue = (255, 0, 0)
white = (255, 255, 255)
yellow = (0, 255, 255)
cyan = (255, 255, 0)
magenta = (255, 0, 255)
gray = (150, 150, 150)
purple = (255, 0, 127)


class Point(NamedTuple):
    x: int
    y: int


class hsv(NamedTuple):
    h: double = 0.0
    s: double = 0.0
    v: double = 0.0


class rgb(NamedTuple):
    r: double = 0.0
    g: double = 0.0
    b: double = 0.0


class EWords(NamedTuple):
    p: Point
    text: str


class brain_data(NamedTuple):
    img: np.array
    word: str
    hue: float
    transparency: int
    point: Point
    font_scale: int


color_img = cv.imread(whale_path, cv.IMREAD_COLOR) #원형색상그래프 이미지 불러오기
if color_img is None:
    print("Can't open whale.jpg")
stick_img = cv.imread(stick_path, cv.IMREAD_COLOR) #막대그래프 이미지 불러오기
if stick_img is None:
    print("Can't open stick.jpg")
emotion_grid = cv.imread(emotion_grid_path, cv.IMREAD_COLOR)   #emotion_grid 이미지 불러오기
if emotion_grid is None:
    print("Can't open emotion_grid.jpg")
black_bg = cv.imread(black_bg_path, cv.IMREAD_COLOR)   #검정배경 불러오기
if black_bg is None:
    print("Can't open black_bg.jpg")
white_bg = cv.imread(white_bg_path, cv.IMREAD_COLOR)   #하얀배경 불러오기
if white_bg is None:
    print("Can't open white_bg.jpg")


pre_text = []
pre_data = []


def gui_main(mode, emotion, mood, energy):
    circleL = color_line_chart(color_img, mode)
    circleS = color_line_chart(color_img, emotion)
    stick = stick_chart(stick_img, mood, energy)
    combine_imshow(circleL, circleS, stick)


def brain_typo(text, hue, transparency, point, font_scale):
    img = white_bg  # 기본배경 설정
    # print(type(img))
    data = brain_data(img, text, hue, transparency, point, font_scale)  # 입력받은 값 data변수에 담기

    if text not in pre_text:
        after_data = whale_brain(data)
        pre_data.append(after_data)
        pre_text.append(text)
    else:
        for i in range(len(pre_data)):
            if pre_text[i] == text:
                after_data = whale_brain(data)
                pre_data[i] = after_data
                # print("중복있음")


def print_typo():
    for i in range(len(pre_data)):
        if i == 0:
            brain_img = pre_data[i].img
        else:
            data = pre_data[i]
            data = brain_data(brain_img, data.word, data.hue, data.transparency, data.point, data.font_scale)#img파일 수정
            pre_data[i] = whale_brain(data)
            brain_img = pre_data[i].img

    cv.imshow("whale brain", brain_img)
    del pre_data[:] #리스트 초기화
    del pre_text[:] #리스트 초기화


def del_typo(text):
    flag = 0
    for i in range(len(pre_text)):
        if pre_text[i] == text:
            data = pre_data[i]
            data = brain_data(white_bg, '', data.hue, 0, data.point, 0)
            pre_data[i] = whale_brain(data)
            print_typo()
            #print("삭제완료")
            flag = 1
    if flag == 0:
        print("error: 입력하신 단어는 존재하지 않습니다")


#감정값 whale이미지에 맞게 비율 맞추는 함수
def percent(emotion):
    x = emotion[0]
    y = emotion[1]
    x = float(x) * (379.0/100.0)
    y = float(y) * (379.0/100.0)
    return (x,y)


#좌표값->각도로 변환하는 함수
def xy2deg(x, y):
    degree = float(atan2(x, y) * 180 / PI)
    return degree


#좌표각도->hue값으로 변환해주는 함수
def deg2hue(degree):
    #각도값이 -180~180 범위로 표현되기 때문에 0~360도로 변환하기위한 조건문
    if degree >= 0:
        hue = int(degree)/2
    else:
        degree += 360.0
        hue = int(degree)/2

    #hue값 color(bgr)로 변환
    Phsv = hsv(hue*2, 1, 1)
    Prgb = hsv2rgb(Phsv)
    r = int(Prgb.r * 255)
    g = int(Prgb.g * 255)
    b = int(Prgb.b * 255)

    return (b,g,r)


#hsv->rgb로 바꾸는 함수
def hsv2rgb(hsv):
    input = hsv

    if input.s <= 0.0:
        output = rgb(input.v, input.v, input.v)
        return output

    hh = input.h
    if hh >= 360.0:
        hh = 0.0
    hh /= 60.0
    i = int(hh)
    ff = hh - i
    p = input.v * (1.0 - input.s)
    q = input.v * (1.0 - (input.s * ff))
    t = input.v * (1.0 - (input.s * (1.0 - ff)))

    if i == 0:
        output = rgb(input.v, t, p)
    elif i == 1:
        output = rgb(q, input.v, p)
    elif i == 2:
        output = rgb(p, input.v, t)
    elif i == 3:
        output = rgb(p, q, input.v)
    elif i == 4:
        output = rgb(t, p, input.v)
    else:
        output = rgb(input.v, p, q)

    return output


#rgb->hsv로 바꾸는 함수
def rgb2hsv(rgb):
    input = rgb

    if input.r < input.g:
        min = input.r
    else:
        min = input.g

    if min <input.b:
        min = min
    else:
        min = input.b

    if input.r > input.g:
        max = input.r
    else:
        max = input.g

    if max > input.b:
        max = max
    else:
        max = input.b

    v = max
    delta = max - min

    if max > 0.0:
        s = delta / max
    else:
        s = 0.0
        h = 0.0
        return hsv(h, s, v)

    if input.r >= max:
        if delta == 0:
            h = 0.0
        else:
            h = (input.g - input.b) / delta
    else:
        if input.g >= max:
            h = 2.0 + (input.b - input.r) / delta
        else:
            h = 4.0 + (input.r - input.g) / delta

    h *= 60.0

    if h < 0.0:
        h += 360.0

    return hsv(h, s, v)


#좌표값에 따라 화살표그려주는 함수
def color_line_chart(img, emotion):
    img_circle = img.copy()
    emotion = percent(emotion)
    emotion_x = emotion[0]
    emotion_y = emotion[1]
    h , w = img_circle.shape[:2]
    #이미지 중심좌표저장
    center_x = int(w / 2)
    center_y = int(h / 2)

    result_x = int(center_x + emotion_x) #감정값의 원점을 이미지 중심으로 옮김
    result_y = int(w - (center_y + emotion_y))   #y좌표의 0점이 화면상단에서 시작하므로 반전시킴

    color = deg2hue(xy2deg(emotion_x, emotion_y))    #좌표에 따른 bgr값 받아옴

    cv.circle(img_circle, (center_x, center_y), 6, gray, -1) #그래프 중심
    cv.arrowedLine(img_circle, (center_x, center_y), (result_x, result_y), color, 5, cv.CV_8UC3, 0, 0.1)    #화살표그리기

    return img_circle


#좌표값을 막대그래프로 나타내주는 함수
def stick_chart(img, mood, energy):
    img_stick = img.copy()
    #감정값(-100~100범위를 막대그래프 -180~180범위로 나타내는식
    result_x = int(mood * (180.0/100.0))
    result_y = int(energy * (180.0/100.0))

    if mood >= 0:   #감정의 x좌표(mood)가 양수이면 초록색으로 막대그래프 채움
        img_stick = draw_rect(img_stick, 250, result_x, green)
    else:   #감정의 x좌표(mood)가 음수이면 보라색으로 막대그래프 채움
        img_stick = draw_rect(img_stick, 250, result_x, purple)
    if energy >= 0: #감정의 y좌표(energy)가 양수이면 빨간색으로 막대그래프 채움
        img_stick = draw_rect(img_stick, 420, result_y, red)
    else:   #감정의 y좌표(energy)가 음수이면 파란색으로 막대그래프 채움
        img_stick = draw_rect(img_stick, 420, result_y, blue)

    return img_stick


#stick_chart에 사용되는 직사각형 그리기 함수
def draw_rect(img, p, emotion, color):
    img_clone = img.copy()
    center_y = 200  #막대그래프 중심축

    if emotion >= 0:
        color = trans_color((emotion / 2), color)
    else:
        color = trans_color((emotion / 2), color)

    cv.rectangle(img_clone, (p+2,center_y), (p+88,center_y - emotion), color, -1)

    return img_clone


#막대바 색 채도 변경 함수
def trans_color(emotion, color):
    if emotion < 0:
        emotion *= (-1)

    percent = double(emotion) / 100.0
    percent = (percent / 2 + 0.5)
    #감정정도(percent)에 따라 color 채도 변화
    Prgb = rgb(double(color[2]) / 255.0, double(color[1]) / 255.0, double(color[0]) / 255.0)
    Phsv = rgb2hsv(Prgb)
    Phsv = hsv(Phsv.h, Phsv.s * percent, Phsv.v)
    Prgb = hsv2rgb(Phsv)
    r = (int)(Prgb.r * 255)
    g = (int)(Prgb.g * 255)
    b = (int)(Prgb.b * 255)

    return (b, g, r)


#text 색 채도 변경 함수
def trans_text_color(emotion, color, b_n_w):
    percent = double(emotion) / 100.0
    #감정정도(percent)에 따라 color 채도 변화
    Prgb = rgb(double(color[2]) / 255.0, double(color[1]) / 255.0, double(color[0]) / 255.0)
    Phsv = rgb2hsv(Prgb)

    if b_n_w == 1:  #흰배경일때
        Phsv = hsv(Phsv.h, Phsv.s * percent, Phsv.v)
    else:   #검은배경일때
        Phsv = hsv(Phsv.h, Phsv.s, Phsv.v * percent)

    Prgb = hsv2rgb(Phsv)
    r = (int)(Prgb.r * 255)
    g = (int)(Prgb.g * 255)
    b = (int)(Prgb.b * 255)

    return (b, g, r)


#입력받은 감정단어를 emotion_grid에 점찍어주는 함수
def emotion_word_point(img, p):
    img_grid = img.copy()

    color = emotion_word_color(p)   #word의 위치좌표에 맞는 색상을 반환하는 함수
    cv.circle(img_grid,(p.x,p.y),4,color,-1)

    return img_grid


#단어의 좌표 위치에 따른 색상 변환함수
def emotion_word_color(p):
    zeroP = Point(424, 368)
    #word_x,word_y은 deg2hue함수를 사용하기 위한 좌표값을 위한 변수
    word_x = p.x
    word_y = p.y
    #word값을 emotion값으로 변화시켜주는 계산식
    word_x -= zeroP.x
    word_y -= zeroP.y
    word_y *= (-1)

    return deg2hue(xy2deg(word_x, word_y))


def whale_brain(data):
    brain = data.img.copy()

    hue = float(450 - (data.hue *2))
    if hue > 360:
        hue -= 360

    color = deg2hue(hue)    #hue값 두배해서 넣어주는거임(==degree)
    color = trans_text_color(data.transparency, color, 1) #투명도(0~100),색,(검은배경=0,흰배경=1)

    p = data.point
    if p[0] == 0:
        x = 0
    else:
        x = p[0] * 3
    if p[1] == 0:
        y = 0
    else:
        y = p[1] * 3
    x += 300
    y = 600 - (y + 300)

    text = data.word
    font_scale = data.font_scale
    fontpath = font_path
    font = ImageFont.truetype(fontpath, font_scale)
    img_pil = Image.fromarray(brain)
    draw = ImageDraw.Draw(img_pil)
    size = font.getsize(text)
    #print(size)
    draw.text((x - (size[0] / 2), y - (size[1] / 2)), text, font=font, fill=color)
    brain = np.array(img_pil)
    #cv.circle(brain,(x,y),5,black,-1)

    '''
    #puttext사용
    #fontScale = int(data.font_scale / 10.0)
    #thickness = fontScale+1
    font = cv.FONT_HERSHEY_SIMPLEX
    size = cv.getTextSize(text,font,fontScale,thickness)
    #textRow, textCol = size
    x = x - int(size[0][0]/2)
    y = y + int(size[0][1]/2)
    #word_center = Point(int(x - textRow/2),int(y + textCol/2))
    cv.putText(brain,text,(x,y),font,fontScale,color,thickness)
    '''

    data = brain_data(brain, text, data.hue, data.transparency, data.point, font_scale)
    #cv.imshow("whale brain",brain)

    return data


def hangulfont(data):
    brain = white_bg.copy()
    text_img = np.zeros((600, 600, 3), np.uint8)
    fontpath = "malgun.ttf"
    font = ImageFont.truetype(fontpath, 20)
    img_pil = Image.fromarray(text_img)
    draw = ImageDraw.Draw(img_pil)
    draw.text((60, 60), "안녕하세요", font=font, fill = (255, 255, 255, 50))

    cv.imshow("hangul", text_img)

#세가지의 창을 하나로 합치는 함수
def combine_imshow(circleL, circleS, stick)->object :
    circleL = cv.resize(circleL, (600, 600))
    circleS = cv.resize(circleS, (380, 380))
    stick = cv.resize(stick, (380, 220))
    #cv.imshow("circleL",circleL)
    #cv.imshow("circleS",circleS)
    #cv.imshow("stick",stick)
    #opencv함수인 vconcat,hconcat함수가 제대로 작동하지 않아 numpy의 vstack,hstack함수로 변경
    #vertical = cv.vconcat(circleS,stick)
    #horizontal = cv.hconcat(circleL,vertical)
    vertical = np.vstack((circleS, stick))
    horizontal = np.hstack((circleL, vertical))
    cv.imshow("result", horizontal)


if __name__ == "__main__":
    gui_main((50, 50), (-30, -70), -60, 30)
    brain_typo('잘되는구만', 20, 50, (0, 0), 100)
    brain_typo('반가워', 70, 100, (70, -30), 40)
    print_typo()
    cv.waitKey(0)
    brain_typo('봄이다', 0, 80, (70, 30), 70)
    brain_typo('안녕', 70, 100, (70, -30), 40)
    brain_typo('잘되는구만', 30, 50, (0, 0), 60)
    print_typo()
    cv.waitKey(0)
    gui_main((-50, 50), (45, -40), 10, -90)
    brain_typo('반가워', 120, 50, (-70, -30), 20)
    brain_typo('봄이다', 0, 80, (70, -30), 20)
    print_typo()
    cv.waitKey(0)
    #brain_typo('잘되는구만', 70, 20, (0, 0), 10)
    brain_typo('잘되는구만', 70, 100, (0, 50), 50)
    print_typo()
    cv.waitKey(0)
