#pragma once
#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "HSV.h"
#include "cam.h"



using namespace cv;
using namespace std;


//������ ��ȯ
void Mouse_Event(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		//cpm_img�� HSV.h���� HSV_set���� �ּҸ� ������
		uint r = cam::cam_img->at<Vec3b>(y, x)[2];
		uint g = cam::cam_img->at<Vec3b>(y, x)[1];
		uint b = cam::cam_img->at<Vec3b>(y, x)[0];
		///�����ͷ� �ٲٱ�
		// uchar __ = __.color.ptr<uchar>(x)
		// uchar __ = __.color.ptr<uchar>(y)
		///������ ������������ �ٲٱ�
		// uchar __ = __.data

		std::cout << "     �� < Left Mouse Event >��     " << endl;
		std::cout << "     ��     " << r << ", " << g << ", " << b << endl;
		std::cout << "     ������������������         " << endl;

		pa->set_bgr(b, g, r);
	}

	if (event == EVENT_RBUTTONDOWN) {
		//std::cout << "Point = (" << x << ", " << y << ")," << endl;
		std::cout << "Rect (" << x << " - 40, " << y << " - 40, 80, 80);" << endl;
	}
}



void ME_target(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		Mat _img = cv::imread("img/color.jpg");

		resize(_img, _img, Size(280, 300), 0, 0, INTER_LINEAR);


		uint r = _img.at<Vec3b>(y, x)[2];

		uint g = _img.at<Vec3b>(y, x)[1];

		uint b = _img.at<Vec3b>(y, x)[0];


		pa->set_bgr(b, g, r);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//https://kgh1994.tistory.com/4?category=694173





	
#endif /// !EMOTION_H