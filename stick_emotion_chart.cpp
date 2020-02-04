#include "gui_test.h"

int main()
{
	int width = 310;
	int height = 220;
	int p1, p2, p3, p4;

	Mat img(height, width, CV_8UC3, Scalar(0, 0, 0));

	printf("네개의 값을 입력 : ");
	scanf_s("%d %d %d %d", &p1, &p2, &p3, &p4);

	if (p1 > 90 | p1 < -90 | p2 > 90 | p2 < -90 | p3 > 90 | p3 < -90 | p4 > 90 | p4 < -90 )
	{
		printf("값이 표현범위를 초과하였습니다.\n");
	}
	else
	{
		rectangle(img, Point(0, 0), Point(310, 220), white, -1);

		rectangle(img, Point(20, 10), Point(65, 190),green, 1);
		rectangle(img, Point(95, 10), Point(140, 190), magenta, 1);
		rectangle(img, Point(170, 10), Point(215, 190), red, 1);
		rectangle(img, Point(245, 10), Point(290, 190), blue, 1);

		rectangle(img, Point(20, 100), Point(65, 100 - p1), green, -1);
		rectangle(img, Point(95, 100), Point(140, 100 - p2), magenta, -1);
		rectangle(img, Point(170, 100), Point(215, 100 - p3), red, -1);
		rectangle(img, Point(245, 100), Point(290, 100 - p4), blue, -1);

		int font = FONT_HERSHEY_SIMPLEX;
		double fontScale = 0.4;
		putText(img, "Positive", Point(20,210), font, fontScale, green, 0.5);
		putText(img, "Negative", Point(92, 210), font, fontScale, magenta, 0.5);
		putText(img, "High Energy", Point(155, 210), font, fontScale, red, 0.4);
		putText(img, "Low Energy", Point(234, 210), font, fontScale, blue, 0.4);

		imshow("stick emotion", img);

		waitKey(0);
	}
}
