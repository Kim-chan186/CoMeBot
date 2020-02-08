/*Gui 헤더파일
  main문을 간결하게 표현하기 위해 사용하는 헤더파일들과 변수들 저장*/

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
	Scalar deg2hue(int x, int y);
	rgb hsv2rgb(hsv in);
	void color_line_chart(Mat img, Point emotion);
	void stick_chart(Mat img, Point emotion);
}