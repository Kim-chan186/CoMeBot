//Gui ������� Gui�� ���� �Լ���

//main������ ����� �� gui_main(Point gradually_emotion, Point immediately_emotion, int pleasantness, int energy);�Լ��� �־��ּ���

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
	Mat color_line_chart(Mat img, Point emotion);
	Mat color_line_chart2(Mat& img, Point emotion);
	Mat stick_chart(Mat& img, int pleasantness, int energy);
	Mat draw_rect(Mat img, int p, int emotion, Scalar color);
	Scalar trans_color(int percent, Scalar color);
	void combine_imshow(Mat circleL, Mat circleS, Mat stick);
}

Point circle_emotionn;
int stick_pleasantness;
int stick_energy;

Mat color_img = Gui::readimg("img/whale.jpg", 600, 600);	//��������׷��� �̹��� �ҷ�����
Mat color_img2 = Gui::readimg("img/whale.jpg", 380, 380);	//��������׷��� �̹��� �ҷ�����
Mat stick_img = Gui::readimg("img/stick.jpg", 380, 220);	//����׷��� �̹��� �ҷ�����

void gui_main(Point circleL_emotion, Point circleS_emotion, int stick_pleasantness, int stick_energy)
{
	Mat circleL = Gui::color_line_chart(color_img, circleL_emotion);	//��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�(�������ΰ���)
	Mat circleS = Gui::color_line_chart2(color_img2, circleS_emotion);	//��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�(�ﰢ���ΰ���)
	Mat stick = Gui::stick_chart(stick_img, stick_pleasantness, stick_energy);		//��ǥ���� ����׷����� ��Ÿ���ִ� �Լ�

	Gui::combine_imshow(circleL, circleS, stick);
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

//rgb->hsv�� �ٲٴ� �Լ�
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

//��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�
Mat Gui::color_line_chart(Mat img, Point emotion)  //(whale�����׷����̹���, ������)
{
	int hue;
	Scalar color;
	Point center, result;
	
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

	circle(img_circle, Point(center.x, center.y), 4, gray, -1);	//�׷��� �߽�
	arrowedLine(img_circle, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//ȭ��ǥ�׸���

	//imshow("grdual_emotion", img_circle);

	return img_circle;
}

Mat Gui::color_line_chart2(Mat& img, Point emotion)  //(whale�����׷����̹���, ������)
{
	int hue;
	Scalar color;
	Point center, result;

	double R;
	double theta;
	Mat img_circle2 = img.clone();
	emotion = Gui::percent(emotion);

	center.x = img.rows / 2;			//�̹��� �߽���ǥ����
	center.y = img.cols / 2;

	//������ǥ�� �����׷��� ���� ���� ������ ��ȯ�ϴ°��� ���� ������ �ʿ���
	result.x = center.x + emotion.x;				//�������� ������ �̹��� �߽����� �ű�
	result.y = img.cols - (center.y + emotion.y);	//y��ǥ�� 0���� ȭ���ܿ��� �����ϹǷ� ������Ŵ

	color = deg2hue(emotion.x, emotion.y);			//��ǥ�� ���� bgr�� �޾ƿ�

	circle(img_circle2, Point(center.x, center.y), 4, gray, -1);	//�׷��� �߽�
	arrowedLine(img_circle2, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//ȭ��ǥ�׸���

	//imshow("immediate_emotion", img_circle2);

	return img_circle2;
}

//��ǥ���� ����׷����� ��Ÿ���ִ� �Լ�
Mat Gui::stick_chart(Mat& img, int pleasantness, int energy) //(����׷����̹���, ������)
{
	Point result;
	Mat img_stick = img.clone();
	//������(-100~100������ ����׷��� -90~90������ ��Ÿ���½�
	result.x = (int)((pleasantness / 10) * 9);
	result.y = (int)((energy / 10) * 9);

	if (pleasantness >= 0) //������ x��ǥ(����,����)�� ����̸� �ʷϻ����� ����׷��� ä��
	{
		img_stick = draw_rect(img_stick, 125, result.x, green);
	}
	else                //������ x��ǥ(����,����)�� �����̸� ��������� ����׷��� ä��
	{
		img_stick = draw_rect(img_stick, 125, result.x, purple);
	}

	if (energy >= 0) //������ y��ǥ(������)�� ����̸� ���������� ����׷��� ä��
	{
		img_stick = draw_rect(img_stick, 210, result.y, red);
	}
	else                //������ y��ǥ(������)�� �����̸� �Ķ������� ����׷��� ä��
	{
		img_stick = draw_rect(img_stick, 210, result.y, blue);
	}

	//imshow("stick_emotion", img_stick);
	return img_stick;
}

//stick_chart�� ���Ǵ� ���簢�� �׸��� �Լ�
Mat Gui::draw_rect(Mat img, int p ,int emotion ,Scalar color)
{
	Mat img_clone = img.clone();
	int center_y = 100;	//����׷��� �߽���

	if (emotion >= 0)
		color = trans_color(emotion + 5, color);

	else
		color = trans_color(emotion - 5, color);

	rectangle(img_clone, Point(p+1, center_y), Point(p + 44, center_y - emotion), color, -1);

	return img_clone;
}

//����� �� ä�� ���� �Լ�
Scalar Gui::trans_color(int emotion, Scalar color)
{
	if (emotion < 0)
	{
		emotion = emotion*(-1);
	}
	double percent = (double)emotion / 100.0;

	percent = (percent / 2 + 0.5);

	//���������� ���� color ä�� ��ȭ
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

//�������� â�� �ϳ��� ��ġ�� �Լ�
void Gui::combine_imshow(Mat circleL, Mat circleS, Mat stick)
{
	Mat vertical, horizontal;

	vconcat(circleS, stick, vertical);
	hconcat(circleL, vertical, horizontal);
	
	imshow("result", horizontal);
}