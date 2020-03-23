#pragma once
#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "HSV.h"
#include "cam.h"



using namespace cv;
using namespace std;


//색공간 변환
void Mouse_Event(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		//cpm_img은 HSV.h에의 HSV_set에서 주소를 가져옴
		uint r = cam::cam_img->at<Vec3b>(y, x)[2];
		uint g = cam::cam_img->at<Vec3b>(y, x)[1];
		uint b = cam::cam_img->at<Vec3b>(y, x)[0];
		///포인터로 바꾸기
		// uchar __ = __.color.ptr<uchar>(x)
		// uchar __ = __.color.ptr<uchar>(y)
		///데이터 직접접근으로 바꾸기
		// uchar __ = __.data

		std::cout << "     ┌ < Left Mouse Event >┐     " << endl;
		std::cout << "     │     " << r << ", " << g << ", " << b << endl;
		std::cout << "     └────────         " << endl;

		pa->set_bgr(b, g, r);
	}

	if (event == EVENT_RBUTTONDOWN) {
		std::cout << "오른쪽 마우스 버튼 클릭.. 좌표 = (" << x << ", " << y << ")" << endl;
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