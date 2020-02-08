/*������ ��ȭ�� �����׷����� ����׷����� ��Ÿ���� �ڵ�
  ���Ŀ� �����������κ��� ���� �޾ƿ� ǥ���ϴ� �ɷ� �����Ұ�*/

#include "gui_test.h"	//gui�� ���õ� �������

int main()
{
	Mat color_img, stick_img;
	Point emotion;

	color_img = Gui::readimg("whale.jpg", 384, 384);	//��������׷��� �̹��� �ҷ�����
	stick_img = Gui::readimg("stick.jpg", 190, 220);	//����׷��� �̹��� �ҷ�����
	
	printf("��ȭ���� �Է� : ");					
	scanf_s("%d %d", &emotion.x, &emotion.y);	//�Է°� emotion�� ����

	Gui::color_line_chart(color_img, emotion);	//��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�
	Gui::stick_chart(stick_img, emotion);		//��ǥ���� ����׷����� ��Ÿ���ִ� �Լ�
	waitKey(0);
	
}

Mat Gui::readimg(String name, int x, int y) //�̹������� �ҷ����� ������ �����ϴ� �Լ�
{
	Mat img;

	img = imread(name, IMREAD_COLOR);
	resize(img, img, Size(x, y));

	return img;
}

Scalar Gui::deg2hue(int x, int y)//��ǥ�� �޾Ƽ� ���������� ������ ��ȯ���ִ� �Լ�
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

rgb Gui::hsv2rgb(hsv in)//hsv->rgb�� �ٲٴ� �Լ�
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

void Gui::color_line_chart(Mat img, Point emotion) //��ǥ���� ���� ȭ��ǥ�׷��ִ� �Լ�
{
	int hue;
	Scalar color;
	Point center, result;

	center.x = img.rows / 2;			//�̹��� �߽���ǥ����
	center.y = img.cols / 2;			
	result.x = center.x + emotion.x;				//�������� ������ �̹��� �߽����� �ű�
	result.y = img.cols - (center.y + emotion.y);	//y��ǥ�� 0���� ȭ���ܿ��� �����ϹǷ� ������Ŵ

	color = deg2hue(emotion.x, emotion.y);			//��ǥ�� ���� bgr�� �޾ƿ�

	circle(img, Point(center.x, center.y), 4, gray, -1);	//�׷��� �߽�
	line(img, Point(39, 192), Point(348, 192), gray, 1);	//x��
	line(img, Point(192, 38), Point(192, 345), gray, 1);	//y��
	arrowedLine(img, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//ȭ��ǥ�׸���
	
	imshow("grdual_emotion", img);
}

void Gui::stick_chart(Mat img, Point emotion) //��ǥ���� ����׷����� ��Ÿ���ִ� �Լ�
{
	int center_y = 100; //����׷��� �߽���

	if (emotion.x >= 0) //������ x��ǥ(����,����)�� ����̸� �ʷϻ����� ����׷��� ä��
	{
		rectangle(img, Point(31, center_y), Point(74, center_y - emotion.x), green, -1);
	}
	else                //������ x��ǥ(����,����)�� �����̸� ��������� ����׷��� ä��
		rectangle(img, Point(31, center_y), Point(74, center_y - emotion.x), purple, -1);
	
	if (emotion.y >= 0) //������ y��ǥ(������)�� ����̸� ���������� ����׷��� ä��
	{
		rectangle(img, Point(116, center_y), Point(159, center_y - emotion.y), red, -1);
	}
	else                //������ y��ǥ(������)�� �����̸� �Ķ������� ����׷��� ä��
		rectangle(img, Point(116, center_y), Point(159, center_y - emotion.y), blue, -1);
	
	imshow("stick_emotion", img);
}