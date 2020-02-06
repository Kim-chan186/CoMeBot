#include "gui_test.h"

Mat img,mask,result;
Point P[1000];
int n = 0;

void mouse_callback(int event, int x, int y, int flags, void *userdata) 
{
	if (event == EVENT_LBUTTONDOWN)
	{
		P[n].x = x;
		P[n].y = y;
		circle(img, Point(P[n].x, P[n].y), 2, black, -1);
		printf("%d, %d\n", P[n].x, P[n].y);
		imshow("click_emotion", img);
		n++;
	}
}

int main()
{
	Point center;

	img = imread("color6.jpg", IMREAD_COLOR);
	resize(img, img, Size(384, 384));
	if (img.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	center.x = img.rows / 2;//192
	center.y = img.cols / 2;//192

	//circle(img, Point(192, 192), 100, Scalar(200,200,200,100), -1);
	circle(img, Point(192,192), 4, white, -1);
	line(img, Point(30, 192), Point(350, 192), white, 1);
	line(img, Point(192, 30), Point(192, 350), white, 1);
	
	imshow("click_emotion", img);
	
	while (1)
	{
		setMouseCallback("click_emotion", mouse_callback, 0);
		
		if (waitKey(1) == 27) break;
	}

	return 0;
}
