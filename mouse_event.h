#pragma once
#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "HSV.h"




using namespace cv;
using namespace std;


//색공간 변환
void Mouse_Event(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{
		uint r = cpm_img.at<Vec3b>(y, x)[2];
		uint g = cpm_img.at<Vec3b>(y, x)[1];
		uint b = cpm_img.at<Vec3b>(y, x)[0];
		///포인터로 바꾸기
		// uchar __ = __.color.ptr<uchar>(x)
		// uchar __ = __.color.ptr<uchar>(y)
		///데이터 직접접근으로 바꾸기
		// uchar __ = __.data

		std::cout << "     ┌ < Left Mouse Event >┐     " << endl;
		std::cout << "     │     " << r << ", " << g << ", " << b << endl;
		std::cout << "     └────────         " << endl;

		Scalar bgr(b, g, r);
		Scalar hsv;

		Mat mou_color(250, 250, CV_8UC3, Scalar(b, g, r));
		Mat hsv_color;
		cvtColor(mou_color, hsv_color, COLOR_BGR2HSV);

		int h = hsv_color.at<Vec3b>(0, 0)[0];
		int s = hsv_color.at<Vec3b>(0, 0)[1];
		int v = hsv_color.at<Vec3b>(0, 0)[2];
		((Transf_Color_Area*)userdata)->set_Color(h, s, v);

		cv::imshow("컬러 테스트", mou_color);
		cv::imshow("hsv테스트 색", hsv_color);

	}
	if (event == EVENT_RBUTTONDOWN) {
		std::cout << "오른쪽 마우스 버튼 클릭.. 좌표 = (" << x << ", " << y << ")" << endl;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//https://kgh1994.tistory.com/4?category=694173





	
#endif /// !EMOTION_H