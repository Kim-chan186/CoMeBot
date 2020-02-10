/*Gui �������
  Gui�� ���� �Լ���*/

//main������ ����� �� Gui::readimg�Լ��� ���׷��� ������(384,384),����׷��� ������(190,220) �̹��� �о���� ex)color_img = Gui::readimg("whale.jpg", 384, 384), stick_img = Gui::readimg("stick.jpg", 190, 220)
//Gui::color_line_chart�Լ��� ���׷��� �̹����� �������� �־��ָ� �̹����� ȭ��ǥ�� ǥ���� ex)Gui::color_line_chart(color_img, emotion)
//Gui::stick_chart�Լ��� �������� �־� ����׷����� ǥ�� ex)Gui::stick_chart(stick_img, emotion)

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

//��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�
void Gui::color_line_chart(Mat img, Point emotion)  //(whale�����׷����̹���, ������)
{
	int hue;
	Scalar color;
	Point center, result;
	Point max(348, 345), min(39, 38);
	double R;
	double theta;

	emotion = Gui::percent(emotion);

	center.x = img.rows / 2;			//�̹��� �߽���ǥ����
	center.y = img.cols / 2;

	//������ǥ�� �����׷��� ���� ���� ������ ��ȯ�ϴ°��� ���� ������ �ʿ���
	result.x = center.x + emotion.x;				//�������� ������ �̹��� �߽����� �ű�
	result.y = img.cols - (center.y + emotion.y);	//y��ǥ�� 0���� ȭ���ܿ��� �����ϹǷ� ������Ŵ

	color = deg2hue(emotion.x, emotion.y);			//��ǥ�� ���� bgr�� �޾ƿ�

	circle(img, Point(center.x, center.y), 4, gray, -1);	//�׷��� �߽�
	line(img, Point(min.x, center.y), Point(max.x, center.y), gray, 1);	//x��
	line(img, Point(center.x, min.y), Point(center.x, max.y), gray, 1);	//y��
	arrowedLine(img, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//ȭ��ǥ�׸���

	imshow("grdual_emotion", img);
}

//��ǥ���� ����׷����� ��Ÿ���ִ� �Լ�
void Gui::stick_chart(Mat img, Point emotion) //(����׷����̹���, ������)
{
	int center_y = 100; //����׷��� �߽���
	Point result;

	//������(-100~100������ ����׷��� -90~90������ ��Ÿ���½�
	result.x = (int)((emotion.x / 10) * 9);
	result.y = (int)((emotion.y / 10) * 9);

	if (emotion.x >= 0) //������ x��ǥ(����,����)�� ����̸� �ʷϻ����� ����׷��� ä��
	{
		rectangle(img, Point(31, center_y), Point(74, center_y - result.x), green, -1);
	}
	else                //������ x��ǥ(����,����)�� �����̸� ��������� ����׷��� ä��
		rectangle(img, Point(31, center_y), Point(74, center_y - result.x), purple, -1);

	if (emotion.y >= 0) //������ y��ǥ(������)�� ����̸� ���������� ����׷��� ä��
	{
		rectangle(img, Point(116, center_y), Point(159, center_y - result.y), red, -1);
	}
	else                //������ y��ǥ(������)�� �����̸� �Ķ������� ����׷��� ä��
		rectangle(img, Point(116, center_y), Point(159, center_y - result.y), blue, -1);

	imshow("stick_emotion", img);
}