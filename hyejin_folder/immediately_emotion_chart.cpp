#include "gui_test.h"

int main()
{
	Mat img;
	Point center, emotion, result, max, min;

	int font = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.4;

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

	result.x = center.x + emotion.x;
	result.y = 384 - (center.y + emotion.y);

	circle(img, Point(center.x, center.y), 4, white, -1);
	line(img, Point(30, 192), Point(350, 192), white, 1);
	line(img, Point(192, 30), Point(192, 350), white, 1);
	circle(img, Point(result.x, result.y), 2, black, -1);
	if (emotion.x > 0 & emotion.y > 0)
	{
		putText(img, "happy", Point(result.x + 5, result.y - 5), font, fontScale, red, 0.5);
	}
	else if (emotion.x > 0 & emotion.y < 0)
	{
		putText(img, "miss", Point(result.x + 5, result.y + 10), font, fontScale, green, 0.5);
	}
	else if (emotion.x < 0 & emotion.y < 0)
	{
		putText(img, "sad", Point(result.x - 20, result.y + 15), font, fontScale, blue, 0.5);
	}
	else if (emotion.x < 0 & emotion.y > 0)
	{
		putText(img, "angry", Point(result.x - 25, result.y - 10), font, fontScale, magenta, 0.5);
	}

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
