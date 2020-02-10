/*Gui 헤더파일
  Gui를 위한 함수들*/

//main문에서 사용할 때 Gui::readimg함수로 고래그래프 사이즈(384,384),막대그래프 사이즈(190,220) 이미지 읽어오고 ex)color_img = Gui::readimg("whale.jpg", 384, 384), stick_img = Gui::readimg("stick.jpg", 190, 220)
//Gui::color_line_chart함수로 고래그래프 이미지에 감정값을 넣어주면 이미지에 화살표로 표현됨 ex)Gui::color_line_chart(color_img, emotion)
//Gui::stick_chart함수에 감정값을 넣어 막대그래프로 표현 ex)Gui::stick_chart(stick_img, emotion)

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#define PI 3.14

using namespace cv;
using namespace std;

Scalar black(0, 0, 0);
Scalar red(0, 0, 255);
Scalar green(0, 180, 0);
Scalar blue(255, 0, 0);
Scalar white(255, 255, 255);
Scalar yellow(0, 255, 255);
Scalar cyan(255, 255, 0);
Scalar magenta(255, 0, 255);
Scalar gray(150, 150, 150);
Scalar purple(255, 0, 127);

typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;

static rgb   hsv2rgb(hsv in);

namespace Gui {
	Mat readimg(String name, int x, int y);
	Point percent(Point emotion);
	Scalar deg2hue(int x, int y);
	rgb hsv2rgb(hsv in);
	void color_line_chart(Mat img, Point emotion);
	void stick_chart(Mat img, Point emotion);
}

//이미지파일 불러오고 사이즈 조절하는 함수
Mat Gui::readimg(String name, int x, int y)  //(파일명, x축size, y축size)
{
	Mat img;

	img = imread(name, IMREAD_COLOR);
	resize(img, img, Size(x, y));

	return img;
}

//감정값 whale이미지에 맞게 비율 맞추는 함수
Point Gui::percent(Point emotion)  //(감정값)
{
	if (emotion.x >= 0)
	{
		emotion.x = (int)(emotion.x *(155.0 / 100.0));
	}
	else if (emotion.x < 0)
	{
		emotion.x = (int)(emotion.x *(152.0 / 100.0));
	}

	if (emotion.y >= 0)
	{
		emotion.y = (int)(emotion.y *(153.0 / 100.0));
	}
	else if (emotion.y < 0)
	{
		emotion.y = (int)(emotion.y *(152.0 / 100.0));
	}

	return emotion;
}

//좌표값 받아서 각도에따른 색으로 변환해주는 함수
Scalar Gui::deg2hue(int x, int y) //(감정 x좌표, 감정 y좌표)
{
	//좌표각도->hue값으로 변환
	float degree;
	int hue;

	degree = atan2(x, y) * 180 / PI;

	//각도값이 -180~180 범위로 표현되기 때문에 0~360도로 변환하기위한 조건문
	if (degree >= 0)
	{
		hue = (int)degree / 2;
	}
	else if (degree < 0)
	{
		degree += 360.0;
		//printf("%.f\n", degree);
		hue = (int)degree / 2;
	}

	//hue값 color로 변환
	rgb R;
	hsv P;
	int r, g, b;
	P.h = hue * 2;
	P.s = 1;
	P.v = 1;
	R = Gui::hsv2rgb(P);
	r = (int)(R.r * 255);
	g = (int)(R.g * 255);
	b = (int)(R.b * 255);

	return Scalar(b, g, r);
}

//hsv->rgb로 바꾸는 함수
rgb Gui::hsv2rgb(hsv in)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}

//좌표값에 따라 화살표그려주는 함수
void Gui::color_line_chart(Mat img, Point emotion)  //(whale원형그래프이미지, 감정값)
{
	int hue;
	Scalar color;
	Point center, result;
	Point max(348, 345), min(39, 38);
	double R;
	double theta;

	emotion = Gui::percent(emotion);

	center.x = img.rows / 2;			//이미지 중심좌표저장
	center.y = img.cols / 2;

	//감정좌표를 원형그래프 범위 내의 값으로 변환하는것은 아직 수정이 필요함
	result.x = center.x + emotion.x;				//감정값의 원점을 이미지 중심으로 옮김
	result.y = img.cols - (center.y + emotion.y);	//y좌표의 0점이 화면상단에서 시작하므로 반전시킴

	color = deg2hue(emotion.x, emotion.y);			//좌표에 따른 bgr값 받아옴

	circle(img, Point(center.x, center.y), 4, gray, -1);	//그래프 중심
	line(img, Point(min.x, center.y), Point(max.x, center.y), gray, 1);	//x축
	line(img, Point(center.x, min.y), Point(center.x, max.y), gray, 1);	//y축
	arrowedLine(img, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//화살표그리기

	imshow("grdual_emotion", img);
}

//좌표값을 막대그래프로 나타내주는 함수
void Gui::stick_chart(Mat img, Point emotion) //(막대그래프이미지, 감정값)
{
	int center_y = 100; //막대그래프 중심축
	Point result;

	//감정값(-100~100범위를 막대그래프 -90~90범위로 나타내는식
	result.x = (int)((emotion.x / 10) * 9);
	result.y = (int)((emotion.y / 10) * 9);

	if (emotion.x >= 0) //감정의 x좌표(긍정,부정)가 양수이면 초록색으로 막대그래프 채움
	{
		rectangle(img, Point(31, center_y), Point(74, center_y - result.x), green, -1);
	}
	else                //감정의 x좌표(긍정,부정)가 음수이면 보라색으로 막대그래프 채움
		rectangle(img, Point(31, center_y), Point(74, center_y - result.x), purple, -1);

	if (emotion.y >= 0) //감정의 y좌표(에너지)가 양수이면 빨간색으로 막대그래프 채움
	{
		rectangle(img, Point(116, center_y), Point(159, center_y - result.y), red, -1);
	}
	else                //감정의 y좌표(에너지)가 음수이면 파란색으로 막대그래프 채움
		rectangle(img, Point(116, center_y), Point(159, center_y - result.y), blue, -1);

	imshow("stick_emotion", img);
}