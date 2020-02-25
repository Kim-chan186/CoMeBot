#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

//������ �����ε�
bool operator < (const Point& p1, const Point& p2) {
	return (p1.x < p2.x) & (p1.y < p2.y);
}
double operator * (const Point& p1, const Point& p2) {
	return pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2);
}
Point operator + (Point& p1, const int& n) {
	Point p = p1;
	p.x += n;
	p.y += n;
	return p;
}
Point operator - (Point& p1, const int& n) {
	Point p = p1;
	p.x -= n;
	p.y -= n;
	return p;
}

namespace cam {
	
	cv::VideoCapture* _cam = nullptr;

	int cam_num;

	bool cam_Initialize(cv::VideoCapture& cam, cv::Mat& frame, string str) {

		cam = cv::VideoCapture(str);
		if (!cam.isOpened()) {
			cout << "\n ** ����! \n";
			return 0;
		}
		_cam = &cam;
		cam >> frame;

		std::cout << str << " ������� �����մϴ�. \n";

		return 1;
	}//End cam_Initialize

	bool cam_Initialize(cv::VideoCapture& cam, cv::Mat& frame, int cam_num = 186) {
		//cam ��ȣ�� �־����� ���� ��� ã�´�.
		//���� ��ȣ�� �־����� �ʾƵ� ã�� var.�� �����
		if (cam_num == 186) {
			for (int i = -1; i < 5; i++) {
				cv::VideoCapture cam_test(i);
				if (!cam_test.isOpened()) {
					std::cout << i << " cam is opened \n";
				}
				else {
					std::cout << i << "\n cam failing \n";
					cam_num = i;
				}
			}
		}
		cam = cv::VideoCapture(cam_num);
		if (!cam.isOpened()) {
			cout << "\n ** cam.h : cam open failing! \n";
			return 0;
		}
		_cam = &cam;
		cam >> frame;

		cam.set(cv::CAP_PROP_ZOOM, 0);

		std::cout << cam_num << "�� ī�޶�� �����մϴ�. \n"
			<< "cam Zoom : " << cam.get(cv::CAP_PROP_ZOOM) << std::endl //0 ~ 10
			<< "cam Focus : " << cam.get(cv::CAP_PROP_FOCUS) << std::endl; //-1

		return 1;
	}//End cam_Initialize

	// 0 : zoom out, 1 : zoom in
	bool cam_set_zoom(bool add) {
		double num = _cam->get(CAP_PROP_ZOOM);
		if (add) {//zoom in
			num++;
		}
		else {//zoom out
			num--;
		}
		bool result = _cam->set(CAP_PROP_ZOOM, num);
		std::cout << "cam_Zoom : " << _cam->get(CAP_PROP_ZOOM) << std::endl;
		return result; //���� ����
	}
}


#endif /// !EMOTION_H