//Gui ������� Gui�� ���� �Լ���

/*main������ ����� �� 
gui_main(Point gradually_emotion, Point immediately_emotion, int pleasantness, int energy);�Լ��� �־��ּ���
brain_typo(EWords text, float hue, int transparency, Point point, int font_scale); Ewords text�� emotion_grid�� �ִ� �ܾ��(ex:���,���...)(������namespace EW),
																				   transparency(����)�� 0(����)~100(������)������ ��,
																				   point�� �ܾ���ġ��ǥ ����(0,0) max��(100,100), min��(-100,-100)��
																				   font_scale�� ������Ʈũ�� (0~50)������ �� ����������� 10~20���� (0~5)������ ��˸��ϰ�, 30�̻��� ��ûŭ ������ ����� ã���ñ�!
																				   (�������Ʈ����� ���������ιۿ� ������ �ȵǼ� ũ�⿡ ���� ���Ⱑ �ٲ�� �س��������� �ҿ������̿��� ���ڿ��������� ����)
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

namespace Gui {
	//Gui���� �Լ�,����
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

	struct rgb {
		double r;       // a fraction between 0 and 1
		double g;       // a fraction between 0 and 1
		double b;       // a fraction between 0 and 1
	};

	struct hsv {
		double h;       // angle in degrees
		double s;       // a fraction between 0 and 1
		double v;       // a fraction between 0 and 1
	};

	static rgb   hsv2rgb(hsv in);
	static hsv  rgb2hsv(rgb in);

	struct EWords
	{
		Point p;	//�����ܾ� ��ǥ
		String text;	//�����ܾ� 
	};

	struct brain_data
	{
		Mat img;
		EWords text;
		float hue;
		int transparency;
		Point point;
		int font_scale;
	};

	Mat readimg(String name, int x, int y);
	Point percent(Point emotion);
	float xy2deg(int x, int y);
	Scalar deg2hue(float degree);
	rgb hsv2rgb(hsv in);
	Mat color_line_chart(Mat img, Point emotion);
	Mat stick_chart(Mat& img, int pleasantness, int energy);
	Mat emotion_word_point(Mat& img, Point word);
	Mat draw_rect(Mat img, int p, int emotion, Scalar color);
	Scalar trans_color(int percent, Scalar color);
	Scalar trans_text_color(int emotion, Scalar color, int bl_n_wh);
	Scalar emotion_word_color(Point word);
	brain_data whale_brain(brain_data data);
	void combine_imshow(Mat circleL, Mat circleS, Mat stick);
}

namespace EW {
	//�����ܾ���� Point
	//���� �̹���(emotion_grid)������ (840,737)
	Point zeroP(424, 368); //�׷��� ����
	//����ܾ�� �׳� �����˻��ؼ� match�ѰŶ� ǰ����� �ϰ������� �ʰ� �߸��Ǿ��������� ���� ����
	Gui::EWords ��� = { { 506, 323 },"surprise" };
	Gui::EWords ��� = { { 521, 219 },"upset" };
	Gui::EWords ���� = { { 604, 368 },"throb" };
	Gui::EWords ���� = { { 626, 333 },"desire" };
	Gui::EWords ��� = { { 668, 330 },"love" };
	Gui::EWords �ӽ� = { { 648, 301 },"refresh" };
	Gui::EWords �ķ� = { { 671, 297 },"relieved" };
	Gui::EWords ��ź = { { 694, 302 },"amazed" };
	Gui::EWords �ڶ� = { { 711, 289 },"proud" };
	Gui::EWords ���� = { { 694, 279 },"pride" };
	Gui::EWords �ѵ� = { { 665, 274 },"brimful" };
	Gui::EWords ���� = { { 676, 261 },"inspiring" };
	Gui::EWords ��� = { { 695, 255 },"fun" };
	Gui::EWords �η� = { { 507, 419 },"envy" };
	Gui::EWords �׸� = { { 480, 537 },"miss" };
	Gui::EWords ���� = { { 446, 662 },"compassion" };
	Gui::EWords ��ƶ = { { 546, 546 },"fond" };
	Gui::EWords ���� = { { 549, 521 },"touch" };
	Gui::EWords ���� = { { 408, 543 },"unbecoming" };
	Gui::EWords �β� = { { 390, 561 },"shy" };
	Gui::EWords �̾� = { { 391, 591 },"sorry" };
	Gui::EWords �ҽ� = { { 373, 629 },"poor" };
	Gui::EWords ��Ƽ = { { 373, 643 },"anti" };
	Gui::EWords ���� = { { 403, 676 },"pity" };
	Gui::EWords �θ� = { { 358, 560 },"awkward" };
	Gui::EWords ��Ȥ = { { 345, 483 },"perplexed" };
	Gui::EWords â�� = { { 340, 521 },"embarrassed" };
	Gui::EWords ��ȸ = { { 321, 513 },"regret" };
	Gui::EWords ��å = { { 319, 533 },"guilty" };
	Gui::EWords ���� = { { 303, 633 },"empty" };
	Gui::EWords ���� = { { 286, 614 },"bitter" };
	Gui::EWords ���� = { { 284, 533 },"nervous" };
	Gui::EWords �Ҿ� = { { 284, 515 },"anxiety" };
	Gui::EWords ���� = { { 280, 484 },"desperate" };
	Gui::EWords ���� = { { 279, 558 },"intricacy" };
	Gui::EWords �㹫 = { { 276, 594 },"notingness" };
	Gui::EWords ���� = { { 259, 489 },"boring" };
	Gui::EWords ��� = { { 257, 567 },"lose" };
	Gui::EWords ���� = { { 259, 583 },"sadness" };
	Gui::EWords ���� = { { 252, 624 },"sorrowful" };
	Gui::EWords ��� = { { 236, 587 },"depression" };
	Gui::EWords �ҿ� = { { 239, 557 },"isolation" };
	Gui::EWords �ɶ� = { { 240, 528 },"disturbed" };
	Gui::EWords �ӻ� = { { 242, 481 },"hurt" };
	Gui::EWords �߼� = { { 240, 378 },"unkind" };
	Gui::EWords ���� = { { 234, 460 },"dismal" };
	Gui::EWords ���� = { { 214, 399 },"bully" };
	Gui::EWords ���� = { { 210, 443 },"misery" };
	Gui::EWords ���� = { { 204, 481 },"frustration" };
	Gui::EWords ���� = { { 192, 428 },"unfortunate" };
	Gui::EWords ���� = { { 196, 409 },"despair" };
	Gui::EWords Ȳ�� = { { 314, 284 },"absurd" };
	Gui::EWords ��ġ = { { 277, 322 },"shame" };
	Gui::EWords �̳� = { { 278, 365 },"scare" };
	Gui::EWords ���� = { { 278, 223 },"creepy" };
	Gui::EWords ���� = { { 261, 243 },"jealousy" };
	Gui::EWords �η� = { { 257, 307 },"afraid" };
	Gui::EWords ���� = { { 240, 256 },"scare" };
	Gui::EWords ��� = { { 233, 205 },"provoking" };
	Gui::EWords �Ǹ� = { { 224, 311 },"disappoint" };
	Gui::EWords ��� = { { 220, 268 },"aggrievement" };
	Gui::EWords ���� = { { 216, 221 },"resent" };
	Gui::EWords ��� = { { 215, 174 },"detestable" };
	Gui::EWords �ݺ� = { { 212, 121 },"outrages" };
	Gui::EWords ȯ�� = { { 200, 153 },"disillusion" };
	Gui::EWords ���� = { { 199, 194 },"offend" };
	Gui::EWords �ݰ� = { { 182, 140 },"antipathy" };
	Gui::EWords ¥�� = { { 178, 186 },"annoyance" };
	Gui::EWords �Ӵ� = { { 168, 129 },"hate" };
	Gui::EWords ���� = { { 163, 113 },"hatred" };
	Gui::EWords ��� = { { 164, 201 },"betrayal" };
	Gui::EWords ���� = { { 121, 106 },"disgusting" };
	Gui::EWords ��� = { { 140, 94 },"contempt" };
}

using namespace EW;

int index = 0;

String pre_text[100];	//brain_typo�Լ� ���� �ܾ������
Gui::brain_data pre_data[100];	//brain_typo�Լ� ���� data�����

Mat color_img = imread("img/whale.jpg", IMREAD_COLOR);	//��������׷��� �̹��� �ҷ�����//Mat color_imgS = Gui::readimg("img/whale.jpg", 380, 380);	//��������׷��� �̹��� �ҷ�����
Mat stick_img = imread("img/stick.jpg", IMREAD_COLOR);	//����׷��� �̹��� �ҷ�����
Mat emotion_grid = imread("img/emotion_grid.jpg", IMREAD_COLOR);	//emotion_grid �̹��� �ҷ�����
Mat black_bg = imread("img/black_bg.jpg", IMREAD_COLOR);	//������� �ҷ�����
Mat white_bg = imread("img/white_bg.jpg", IMREAD_COLOR);	//������� �ҷ�����

void gui_main(Point circleL_emotion, Point circleS_emotion, int stick_pleasantness, int stick_energy)
{
	Mat circleL = Gui::color_line_chart(color_img, circleL_emotion);	//��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�(�������ΰ���)
	Mat circleS = Gui::color_line_chart(color_img, circleS_emotion);	//��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�(�ﰢ���ΰ���)
	Mat stick = Gui::stick_chart(stick_img, stick_pleasantness, stick_energy);		//��ǥ���� ����׷����� ��Ÿ���ִ� �Լ�
	
	Gui::combine_imshow(circleL, circleS, stick);
}

void brain_typo(Gui::EWords text, float hue, int transparency, Point point, int font_scale)
{
	int flag = 0;//flag�ʱ�ȭ

	Mat img = white_bg;	//�⺻��� ����
	Gui::brain_data data = { img,text,hue,transparency,point,font_scale }; //�Է¹��� �� data������ ���

	if (index == 0)
	{	//ó������ ��� data,text�� ��� ����
		pre_data[index] = Gui::whale_brain(data);
		pre_text[index] = data.text.text;
		index++;
	}
	else
	{
		//���� �ܾ��� ���Ͽ� �̹� �ִ� �ܾ�� �����ܾ�data�� ���ο� data����
		for (int i = 0; i < index; i++)
		{
			if (pre_text[i] == data.text.text)
			{
				pre_data[i] = Gui::whale_brain(data);
				flag = 1;
				//printf("already\n");
			}
		}
		if (flag == 0)//������ ���� �ܾ�� data,text����
		{
			pre_data[index] = Gui::whale_brain(data);
			pre_text[index] = data.text.text;
			//printf("not yet\n");
			index++;
		}
	}

	//img����� ���� for��
	for (int i = 0; i < index; i++)
	{
		if (i == 0)
		{
			//ùindex img�ҷ�����
			img = pre_data[i].img;
		}
		else
		{
			//�ܾ� �߰��ؼ� img����
			pre_data[i].img = img;
			pre_data[i] = Gui::whale_brain(pre_data[i]);
			img = pre_data[i].img;
		}
	}
	imshow("whale brain", img);
	//waitKey(500);
}

//�̹������� �ҷ����� ������ �����ϴ� �Լ�
Mat Gui::readimg(String name, int x, int y)  //(���ϸ�, x��size, y��size)
{
	Mat img = imread(name, IMREAD_COLOR);
	resize(img, img, Size(x, y));

	return img;
}

//������ whale�̹����� �°� ���� ���ߴ� �Լ�
Point Gui::percent(Point emotion)
{
	emotion *= (379.0 / 100.0);
	
	return emotion;
}

//��ǥ->������ ��ȯ�ϴ� �Լ�
float Gui::xy2deg(int x, int y)
{
	float degree = atan2(x, y) * 180 / PI;

	return degree;
}

//���������� ������ ��ȯ���ִ� �Լ�
Scalar Gui::deg2hue(float degree)
{
	//��ǥ����->hue������ ��ȯ
	int hue;

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
	Scalar color;
	Point center, result;
	
	Mat img_circle = img.clone();

	emotion = percent(emotion);

	center.x = img_circle.cols / 2;			//�̹��� �߽���ǥ����
	center.y = img_circle.rows / 2;

	result.x = center.x + emotion.x;				//�������� ������ �̹��� �߽����� �ű�
	result.y = img_circle.cols - (center.y + emotion.y);	//y��ǥ�� 0���� ȭ���ܿ��� �����ϹǷ� ������Ŵ

	
	color = deg2hue(xy2deg(emotion.x, emotion.y));			//��ǥ�� ���� bgr�� �޾ƿ�

	circle(img_circle, Point(center.x, center.y), 6, gray, -1);	//�׷��� �߽�
	arrowedLine(img_circle, Point(center.x, center.y), Point(result.x, result.y), color, 5, CV_8UC3, 0, 0.1);	//ȭ��ǥ�׸���

	//imshow("grdual_emotion", img_circle);

	return img_circle;
}

//��ǥ���� ����׷����� ��Ÿ���ִ� �Լ�
Mat Gui::stick_chart(Mat& img, int pleasantness, int energy) //(����׷����̹���, ������)
{
	Point result;
	Mat img_stick = img.clone();

	//������(-100~100������ ����׷��� -180~180������ ��Ÿ���½�
	result.x = (int)(pleasantness * (180.0/100.0));
	result.y = (int)(energy * (180.0 / 100.0));

	if (pleasantness >= 0) //������ x��ǥ(����,����)�� ����̸� �ʷϻ����� ����׷��� ä��
	{
		img_stick = draw_rect(img_stick, 250, result.x, green);
	}
	else                //������ x��ǥ(����,����)�� �����̸� ��������� ����׷��� ä��
	{
		img_stick = draw_rect(img_stick, 250, result.x, purple);
	}

	if (energy >= 0) //������ y��ǥ(������)�� ����̸� ���������� ����׷��� ä��
	{
		img_stick = draw_rect(img_stick, 420, result.y, red);
	}
	else                //������ y��ǥ(������)�� �����̸� �Ķ������� ����׷��� ä��
	{
		img_stick = draw_rect(img_stick, 420, result.y, blue);
	}

	//imshow("stick_emotion", img_stick);

	return img_stick;
}

//�Է¹��� �����ܾ emotion_grid�� ������ִ� �Լ�
Mat Gui::emotion_word_point(Mat& img, Point word) 
{
	Mat img_grid = img.clone();
	
	Scalar color = emotion_word_color(word);	//word�� ��ġ��ǥ�� �´� ������ ��ȯ�ϴ� �Լ�
	circle(img_grid, Point(word.x, word.y), 4, color, -1);

	//imshow("emotion_grid", emotion_grid);

	return img_grid;
}

//stick_chart�� ���Ǵ� ���簢�� �׸��� �Լ�
Mat Gui::draw_rect(Mat img, int p ,int emotion ,Scalar color)
{
	Mat img_clone = img.clone();
	int center_y = 200;	//����׷��� �߽���

	if (emotion >= 0)
		color = trans_color((emotion / 2), color);

	else
		color = trans_color((emotion / 2), color);

	rectangle(img_clone, Point(p+2, center_y), Point(p + 88, center_y - emotion), color, -1);

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

//text �� ä�� ���� �Լ�
Scalar Gui::trans_text_color(int emotion, Scalar color, int bl_n_wh)
{
	double percent = (double)emotion / 100.0;

	//���������� ���� color ä�� ��ȭ
	rgb R;
	hsv P;
	int r, g, b;
	double h, s, v;
	R.b = (double)color[0] / 255.0;
	R.g = (double)color[1] / 255.0;
	R.r = (double)color[2] / 255.0;
	P = Gui::rgb2hsv(R);
	if (bl_n_wh == 1) //�����϶�
	{
		P.h = P.h;
		P.s = P.s * percent;
		P.v = P.v;
	}
	else             //��������϶�
	{
		P.h = P.h;
		P.s = P.s;
		P.v = P.v * percent;
	}
	R = Gui::hsv2rgb(P);
	r = (int)(R.r * 255);
	g = (int)(R.g * 255);
	b = (int)(R.b * 255);

	return Scalar(b, g, r);
}

//�ܾ��� ��ǥ ��ġ�� ���� ���� ��ȯ�Լ�
Scalar Gui::emotion_word_color(Point word)
{
	Point word_color = word;	//word_color�� deg2hue�Լ��� ����ϱ� ���� ��ǥ���� ���� ����

	//word���� emotion������ ��ȭ�����ִ� ����
	word_color.x -= zeroP.x;
	word_color.y -= zeroP.y;
	word_color.y = word_color.y * (-1);

	//printf("after: %d, %d\n", word_color.x, word_color.y);
	return deg2hue(xy2deg(word_color.x, word_color.y)); 
}

//�� ���� �ؽ�Ʈ�� ��Ÿ���� �Լ�
Gui::brain_data Gui::whale_brain(brain_data data)
{
	Mat brain = data.img.clone();
	String text = data.text.text;

	float hue = 450 - (data.hue * 2);	//360 - (hue*2 - 90) �׷����� ������ hue����ȭ�� �ݴ���⿡ 90�� ȸ���Ͽ� ��Ÿ���־� �����ϴ� ���
	if (hue > 360)
		hue -= 360;
	Scalar color = deg2hue(hue);

	color = trans_text_color(data.transparency, color, 1);//����(0~100),��,(�������=0,����=1)

	//�߽���ǥ�� ��Ÿ���� ��
	Point p = data.point;
	p *= 3;
	p.x += 300;
	p.y = 600 - (p.y + 300);

	int font = FONT_HERSHEY_SIMPLEX;  // normal size sans-serif font
	double scale = (double)data.font_scale / 10.0;
	double fontScale = scale;
	int thickness = (int)scale + 1;
	int baseLine;
	Size size = getTextSize(text, font, fontScale, thickness, &baseLine);	//text�������� �Լ�
	Point word_center;	//text�� �߽���ǥ�� word��ǥ�� ��ġ��Ű������ ����
	word_center.x = p.x - (size.width / 2);
	word_center.y = p.y + (size.height / 2);

	putText(brain, text, word_center, font, fontScale, color, thickness);
	data.img = brain;
	//imshow("whale brain", brain);

	return data;
}

//�������� â�� �ϳ��� ��ġ�� �Լ�
void Gui::combine_imshow(Mat circleL, Mat circleS, Mat stick)
{
	Mat vertical, horizontal;

	resize(circleL, circleL, Size(600,600));
	resize(circleS, circleS, Size(380,380));
	resize(stick, stick, Size(380, 220));

	vconcat(circleS, stick, vertical);
	hconcat(circleL, vertical, horizontal);
	
	imshow("result", horizontal);
}