//Gui 헤더파일 Gui를 위한 함수들

//main문에서 사용할 때 gui_main(Point emotion, int pleasantness, int energy);함수를 넣어주세요

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#define PI 3.14

using namespace cv;
using namespace std;

namespace Gui {
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
	static hsv  rgb2hsv(rgb in);

	Mat readimg(String name, int x, int y);
	Point percent(Point emotion);
	Scalar deg2hue(int x, int y);
	rgb hsv2rgb(hsv in);
	void color_line_chart(Mat img, Point emotion);
	void stick_chart(Mat img, int pleasantness, int energy);
	Scalar trans_color(int percent, Scalar color);
}

Point circle_emotionn;
int stick_pleasantness;
int stick_energy;

Mat color_img = Gui::readimg("img/whale.jpg", 384, 384);	//원형색상그래프 이미지 불러오기
Mat stick_img = Gui::readimg("img/stick.jpg", 190, 220);	//막대그래프 이미지 불러오기

void gui_main(Point circle_emotion, int stick_pleasantness, int stick_energy)
{
	Gui::color_line_chart(color_img, circle_emotion);	//좌표값에 따라 화살표그려주는 함수
	Gui::stick_chart(stick_img, stick_pleasantness, stick_energy);		//좌표값을 막대그래프로 나타내주는 함수
	waitKey(0);
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
Gui::rgb Gui::hsv2rgb(hsv in)
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

//rgb->hsv로 바꾸는 함수
Gui::hsv Gui::rgb2hsv(rgb in)
{
	hsv        out;
	double      min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max  > in.b ? max : in.b;

	out.v = max;                                // v
	delta = max - min;
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0
		// s = 0, v is undefined
		out.s = 0.0;
		out.h = 0.0;                            // its now undefined
		return out;
	}
	if (in.r >= max)                           // > is bogus, just keeps compilor happy
		if (delta == 0) {
			out.h = 0.0;
		}
		else {
			out.h = (in.g - in.b) / delta;        // between yellow & magenta
		}
	else
		if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
		else
			out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

	out.h *= 60.0;                              // degrees

	if (out.h < 0.0)
		out.h += 360.0;

	return out;
}

//좌표값에 따라 화살표그려주는 함수
void Gui::color_line_chart(Mat img, Point emotion)  //(whale원형그래프이미지, 감정값)
{
	int hue;
	Scalar color;
	Point center, result;
	int max_x = 348;
	int max_y = 345;
	int min_x = 39;
	int min_y = 38;
	
	double R;
	double theta;
	Mat img_circle = img.clone();
	emotion = Gui::percent(emotion);

	center.x = img.rows / 2;			//이미지 중심좌표저장
	center.y = img.cols / 2;

	//감정좌표를 원형그래프 범위 내의 값으로 변환하는것은 아직 수정이 필요함
	result.x = center.x + emotion.x;				//감정값의 원점을 이미지 중심으로 옮김
	result.y = img.cols - (center.y + emotion.y);	//y좌표의 0점이 화면상단에서 시작하므로 반전시킴

	color = deg2hue(emotion.x, emotion.y);			//좌표에 따른 bgr값 받아옴

	circle(img_circle, Point(center.x, center.y), 4, gray, -1);	//그래프 중심
	line(img_circle, Point(min_x, center.y), Point(max_x, center.y), gray, 1);	//x축
	line(img_circle, Point(center.x, min_y), Point(center.x, max_y), gray, 1);	//y축
	arrowedLine(img_circle, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//화살표그리기

	imshow("grdual_emotion", img_circle);
}

//좌표값을 막대그래프로 나타내주는 함수
void Gui::stick_chart(Mat img, int pleasantness, int energy) //(막대그래프이미지, 감정값)
{
	int center_y = 100; //막대그래프 중심축
	Point result;
	Mat img_stick = img.clone();
	//감정값(-100~100범위를 막대그래프 -90~90범위로 나타내는식
	result.x = (int)((pleasantness / 10) * 9);
	result.y = (int)((energy / 10) * 9);

	if (pleasantness >= 0) //감정의 x좌표(긍정,부정)가 양수이면 초록색으로 막대그래프 채움
	{
		green = Gui::trans_color(pleasantness, green);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), green, -1);
	}
	else                //감정의 x좌표(긍정,부정)가 음수이면 보라색으로 막대그래프 채움
	{
		purple = Gui::trans_color(pleasantness, purple);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), purple, -1);
	}

	if (energy >= 0) //감정의 y좌표(에너지)가 양수이면 빨간색으로 막대그래프 채움
	{
		red = Gui::trans_color(energy, red);
		rectangle(img_stick, Point(116, center_y), Point(159, center_y - result.y), red, -1);
	}
	else                //감정의 y좌표(에너지)가 음수이면 파란색으로 막대그래프 채움
	{
		blue = Gui::trans_color(energy, blue);
		rectangle(img_stick, Point(116, center_y), Point(159, center_y - result.y), blue, -1);
	}

	imshow("stick_emotion", img_stick);
}

Scalar Gui::trans_color(int emotion, Scalar color)
{
	if (emotion < 0)
	{
		emotion = emotion*(-1);
	}
	double percent = (double)emotion / 100.0;

	//감정정도에 따라 color 채도 변화
	rgb R;
	hsv P;
	int r, g, b;
	double h, s, v;
	R.b = (double)color[0]/255.0;
	R.g = (double)color[1]/255.0;
	R.r = (double)color[2]/255.0;
	P = Gui::rgb2hsv(R);
	P.h = P.h;
	P.s = P.s*percent;
	P.v = P.v;
	R = Gui::hsv2rgb(P);
	r = (int)(R.r * 255);
	g = (int)(R.g * 255);
	b = (int)(R.b * 255);

	return Scalar(b, g, r);
}
