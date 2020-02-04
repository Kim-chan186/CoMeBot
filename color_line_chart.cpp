#include "gui_test.h"
#include <cmath>
#define PI 3.14

float Degree(int x, int y)
{
	float result;
	result = atan2(x, y) * 180 / PI;
	return result;
}

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

rgb hsv2rgb(hsv in)
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

int main()
{
	Mat img;
	Point center, emotion, result, max, min;
	float degree;
	int hue;
	int r, g, b;
	
	max.x = 160, max.y = 160, min.x = -160, min.y = -160;

	img = imread("color6.jpg", IMREAD_COLOR);

	resize(img, img, Size(384, 384));

	if (img.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	center.x = img.rows / 2;
	center.y = img.cols / 2;
	printf("변화량을 입력 : ");
	scanf_s("%d %d", &emotion.x, &emotion.y);

	degree = Degree(emotion.x, emotion.y);

	if (degree >= 0)
	{
		hue = (int)degree / 2;
	}
	else if(degree < 0)
	{
		degree += 360.0;
		printf("%.f\n", degree);
		hue = (int)degree / 2;
	}
	printf("%d\n", hue);
	
	rgb R;
	hsv P;
	P.h = hue*2;
	P.s = 1;
	P.v = 1;
	R = hsv2rgb(P);
	
	printf("%f, %f, %f \n", R.r, R.g, R.b);
	r =(int)(R.r * 255);
	g =(int)(R.g * 255);
	b =(int)(R.b * 255);

	printf("%.f, %.f, %.f\n", R.r*255, R.g*255, R.b*255);
	printf("%d, %d, %d\n", r, g, b);
	result.x = center.x + emotion.x;
	result.y = 384 - (center.y + emotion.y);

	circle(img, Point(center.x, center.y), 4, white, -1);
	line(img, Point(30, 192), Point(350, 192), white, 1);
	line(img, Point(192, 30), Point(192, 350), white, 1);
	arrowedLine(img, Point(center.x, center.y), Point(result.x, result.y), Scalar(b,g,r) , 2, CV_8UC3, 0, 0.1);

	if (emotion.x > max.x | emotion.x < min.x | emotion.y > max.y | emotion.y < min.y)
	{
		printf("값이 표현범위를 초과하였습니다.\n");
	}
	else
	{
		imshow("grdual_emotion", img);
		waitKey(0);
	}
	
}