<<<<<<< HEAD
//Gui ������� Gui�� ���� �Լ���
=======
//Gui 헤더파일 Gui를 위한 함수들
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179

//main������ ����� �� gui_main(Point emotion, int pleasantness, int energy);�Լ��� �־��ּ���

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>


using namespace cv;
using namespace std;

namespace Gui {
	const double PI = 3.14;
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
	void color_line_chart(Mat& img, Point emotion);
	void stick_chart(Mat& img, int pleasantness, int energy);
	Scalar trans_color(int percent, Scalar color);
}

Point circle_emotionn;
int stick_pleasantness;
int stick_energy;

<<<<<<< HEAD
Mat color_img = Gui::readimg("img/whale.jpg", 384, 384);	//��������׷��� �̹��� �ҷ�����
Mat stick_img = Gui::readimg("img/stick.jpg", 190, 220);	//����׷��� �̹��� �ҷ�����

void gui_main(Point circle_emotion, int stick_pleasantness, int stick_energy)
{
	Gui::color_line_chart(color_img, circle_emotion);	//��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�
	Gui::stick_chart(stick_img, stick_pleasantness, stick_energy);		//��ǥ���� ����׷����� ��Ÿ���ִ� �Լ�
=======
Mat color_img = Gui::readimg("img/whale.jpg", 384, 384);	//원형색상그래프 이미지 불러오기
Mat stick_img = Gui::readimg("img/stick.jpg", 190, 220);	//막대그래프 이미지 불러오기

void gui_main(Point circle_emotion, int stick_pleasantness, int stick_energy)
{
	Gui::color_line_chart(color_img, circle_emotion);	//좌표값에 따라 화살표그려주는 함수
	Gui::stick_chart(stick_img, stick_pleasantness, stick_energy);		//좌표값을 막대그래프로 나타내주는 함수
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
}

//�̹������� �ҷ����� ������ �����ϴ� �Լ�
Mat Gui::readimg(String name, int x, int y)  //(���ϸ�, x��size, y��size)
{
	Mat img;

	img = imread(name, IMREAD_COLOR);
	resize(img, img, Size(x, y));

	return img;
}

//������ whale�̹����� �°� ���� ���ߴ� �Լ�
Point Gui::percent(Point emotion)  //(������)
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

//��ǥ�� �޾Ƽ� ���������� ������ ��ȯ���ִ� �Լ�
Scalar Gui::deg2hue(int x, int y) //(���� x��ǥ, ���� y��ǥ)
{
	//��ǥ����->hue������ ��ȯ
	float degree;
	int hue;

	degree = atan2(x, y) * 180 / PI;

	//�������� -180~180 ������ ǥ���Ǳ� ������ 0~360���� ��ȯ�ϱ����� ���ǹ�
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

	//hue�� color�� ��ȯ
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

//hsv->rgb�� �ٲٴ� �Լ�
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

<<<<<<< HEAD
//rgb->hsv�� �ٲٴ� �Լ�
=======
//rgb->hsv로 바꾸는 함수
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
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

<<<<<<< HEAD
//��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�
void Gui::color_line_chart(Mat& img, Point emotion)  //(whale�����׷����̹���, ������)
=======
//좌표값에 따라 화살표그려주는 함수
void Gui::color_line_chart(Mat& img, Point emotion)  //(whale원형그래프이미지, 감정값)
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
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

	center.x = img.rows / 2;			//�̹��� �߽���ǥ����
	center.y = img.cols / 2;

	//������ǥ�� �����׷��� ���� ���� ������ ��ȯ�ϴ°��� ���� ������ �ʿ���
	result.x = center.x + emotion.x;				//�������� ������ �̹��� �߽����� �ű�
	result.y = img.cols - (center.y + emotion.y);	//y��ǥ�� 0���� ȭ���ܿ��� �����ϹǷ� ������Ŵ

	color = deg2hue(emotion.x, emotion.y);			//��ǥ�� ���� bgr�� �޾ƿ�

<<<<<<< HEAD
	circle(img_circle, Point(center.x, center.y), 4, gray, -1);	//�׷��� �߽�
	line(img_circle, Point(min_x, center.y), Point(max_x, center.y), gray, 1);	//x��
	line(img_circle, Point(center.x, min_y), Point(center.x, max_y), gray, 1);	//y��
	arrowedLine(img_circle, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//ȭ��ǥ�׸���
=======
	circle(img_circle, Point(center.x, center.y), 4, gray, -1);	//그래프 중심
	line(img_circle, Point(min_x, center.y), Point(max_x, center.y), gray, 1);	//x축
	line(img_circle, Point(center.x, min_y), Point(center.x, max_y), gray, 1);	//y축
	arrowedLine(img_circle, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//화살표그리기
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179

	imshow("grdual_emotion", img_circle);
}

<<<<<<< HEAD
//��ǥ���� ����׷����� ��Ÿ���ִ� �Լ�
void Gui::stick_chart(Mat& img, int pleasantness, int energy) //(����׷����̹���, ������)
=======
//좌표값을 막대그래프로 나타내주는 함수
void Gui::stick_chart(Mat& img, int pleasantness, int energy) //(막대그래프이미지, 감정값)
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
{
	int center_y = 100; //����׷��� �߽���
	Point result;
	Mat img_stick = img.clone();
<<<<<<< HEAD
	//������(-100~100������ ����׷��� -90~90������ ��Ÿ���½�
=======
	//감정값(-100~100범위를 막대그래프 -90~90범위로 나타내는식
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
	result.x = (int)((pleasantness / 10) * 9);
	result.y = (int)((energy / 10) * 9);

	if (pleasantness >= 0) //������ x��ǥ(����,����)�� ����̸� �ʷϻ����� ����׷��� ä��
	{
		green = Gui::trans_color(pleasantness, green);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), green, -1);
	}
	else                //������ x��ǥ(����,����)�� �����̸� ��������� ����׷��� ä��
	{
<<<<<<< HEAD
		purple = Gui::trans_color(pleasantness, purple);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), purple, -1);
	}
=======
		green = Gui::trans_color(pleasantness, green);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), green, -1);
	}
	else                //감정의 x좌표(긍정,부정)가 음수이면 보라색으로 막대그래프 채움
	{
		purple = Gui::trans_color(pleasantness, purple);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), purple, -1);
	}
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179

	if (energy >= 0) //������ y��ǥ(������)�� ����̸� ���������� ����׷��� ä��
	{
		red = Gui::trans_color(energy, red);
		rectangle(img_stick, Point(116, center_y), Point(159, center_y - result.y), red, -1);
	}
<<<<<<< HEAD
	else                //������ y��ǥ(������)�� �����̸� �Ķ������� ����׷��� ä��
=======
	else                //감정의 y좌표(에너지)가 음수이면 파란색으로 막대그래프 채움
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
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

<<<<<<< HEAD
	//���������� ���� color ä�� ��ȭ
=======
	//감정정도에 따라 color 채도 변화
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
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
