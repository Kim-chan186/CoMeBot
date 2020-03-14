#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "Debug.h"
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



namespace cam { //										<< namespace "cam" >>
	
	//new�� ��������
	cv::VideoCapture _cam = VideoCapture();

	Mat* cam_img = nullptr;

	int cam_num;

	bool cam_Initialize(cv::Mat& frame, string str) {

		cam_img = &frame;

		_cam.open(str);
		if (!_cam.isOpened()) {
			cout << "\n ** ����! \n";
			return 0;
		}

		_cam.read(frame);

		std::cout << str << " ������� �����մϴ�. \n";

		return 1;

	}//End cam_Initialize



	bool cam_Initialize(cv::Mat& frame, int cam_num = 186) {

		cam_img = &frame;

		if (cam_num == 186) {

			frame = Mat(Size(640/3, 1), CV_8UC3, Scalar::all(1));

			bool cam_flag[5] = { 0 };

			VideoCapture Video[5];


			for (int i = 0; i < 5; i++) {
				
				Video[i] = VideoCapture(i);


				if (!Video[i].isOpened()) {
					continue;
					//:cout << i << " cam is failing \n";
				}
				else {
					cout << i << ". cam Opened \n";
					Mat buf;
					
					cam_flag[i] = true;
					Video[i] >> buf;
					
					resize(buf, buf,Size(buf.cols / 3, buf.rows / 3));
				

					string text = format("cam num : %d", i);

					putText(buf, text,
						Point(10, buf.rows - 10),
						FONT_HERSHEY_PLAIN,
						1,							//fontScale
						cv::Scalar(255, 255, 0),	//color
						1.5);						//thickness

					vconcat(frame, buf, frame);
				}
			}

			imshow("Con", frame);

			
			while (1) {
				char c = waitKey(0);
				if (c >= 48 & c <= 57 & cam_flag[c - 48]) { //1~9
					cam_num = c-48;
					cout << "cam_num" << cam_num << endl;
					_cam = Video[cam_num];
					break;
				}
			}
		}
		

		while (1) {
			_cam.open(cam_num);
			if (!_cam.isOpened()) {
				cout << "\n ** cam.h : cam open failing! \n";
				//return 0;
			}
			else {
				break;
			}
		}
		

		_cam.read(frame);
		_cam.set(cv::CAP_PROP_FPS, 30);
		_cam.set(cv::CAP_PROP_ZOOM, 0);
		_cam.set(cv::CAP_PROP_BRIGHTNESS, 30);
		_cam.set(cv::CAP_PROP_EXPOSURE, -6);

		std::cout << endl << cam_num << "�� ī�޶�� �����մϴ�. \n"
			<< "cam FPS        : " << _cam.get(cv::CAP_PROP_FPS) << endl			//
			<< "cam Width      : " << _cam.get(cv::CAP_PROP_FRAME_WIDTH) << endl	//
			<< "cam hight      : " << _cam.get(cv::CAP_PROP_FRAME_HEIGHT) << endl	//
			<< "cam Zoom       : " << _cam.get(cv::CAP_PROP_ZOOM) << endl			// 0 ~ 10
			<< "cam Focus      : " << _cam.get(cv::CAP_PROP_FOCUS) << endl			// -1
			<< "cam Brightness : " << _cam.get(cv::CAP_PROP_BRIGHTNESS) << endl	// 30 ~ 255
			<< "cam Contrast   : " << _cam.get(CAP_PROP_CONTRAST) << endl			// -1
			<< "cam Exposure   : " << _cam.get(cv::CAP_PROP_EXPOSURE) << "\n\n";	// -11 ~ 1
		
		return 1;

	}//End cam_Initialize



	// 0 : zoom out
	//1 : zoom in
	bool cam_zoom(bool add) {
		double num = _cam.get(CAP_PROP_ZOOM);

		if (num == -1) {
			std::cout << " ** cam error : zoom�� ������ �� ���� cam �Դϴ�." << std::endl;
			return 0;
		}
		else if (add) {//zoom in
			num++;
		}
		else {//zoom out
			num--;
		}

		bool result = _cam.set(CAP_PROP_ZOOM, num);
		std::cout << "cam_Zoom : " << _cam.get(CAP_PROP_ZOOM) << std::endl;

		return result; //���� ����

	}



	// 0 : brightness_up
	// 1 : brightness_down
	bool cam_brightness(bool add) {
		double num = _cam.get(CAP_PROP_BRIGHTNESS);


		if (num == -1) {
			std::cout << " ** cam error : brightness�� ������ �� ���� cam �Դϴ�." << std::endl;
			return 0;
		}
		else if (add) {//Brightness up
			num++;
		}
		else {//Brightness down
			num--;
		}

		bool result = _cam.set(CAP_PROP_BRIGHTNESS, num);
		std::cout << "cam_Brightness : " << _cam.get(CAP_PROP_BRIGHTNESS) << std::endl;

		return result; //���� ����

	}



	// 0 : Contrast_up ��Ϻ� XX
	// 1 : Contrast_down
	bool cam_contrast(bool add) {
		double num = _cam.get(CAP_PROP_CONTRAST);


		if (num == -1) {
			std::cout << " ** cam error : contrast�� ������ �� ���� cam �Դϴ�." << std::endl;
			return 0;
		}
		else if (add) {//Contrast up
			num++;
		}
		else {//Contrast down
			num--;
		}

		bool result = _cam.set(CAP_PROP_CONTRAST, num);
		std::cout << "cam_Contrast   : " << _cam.get(CAP_PROP_CONTRAST) << std::endl;

		return result; //���� ����

	}



	// 0 : Contrast_up ����
	// 1 : Contrast_down
	bool cam_exposure(bool add) {
		double num = _cam.get(CAP_PROP_EXPOSURE);


		if (num == -1) {
			std::cout << " ** cam error : exposure�� ������ �� ���� cam �Դϴ�." << std::endl;
			return 0;
		}
		else if (add) {//exposure up
			num++;
		}
		else {//exposure down
			num--;
		}

		bool result = _cam.set(CAP_PROP_EXPOSURE, num);
		std::cout << "cam_Exposure   : " << _cam.get(CAP_PROP_EXPOSURE) << std::endl;

		return result; //���� ����

	}



	// 0 : gain_up ����
	// 1 : gain_down
	bool cam_gain(bool add) {
		double num = _cam.get(CAP_PROP_GAIN);


		if (num == -1) {
			std::cout << " ** cam error : gain�� ������ �� ���� cam �Դϴ�." << std::endl;
			return 0;
		}
		else if (add) {//exposure up
			num++;
		}
		else {//exposure down
			num--;
		}

		bool result = _cam.set(CAP_PROP_GAIN, num);
		std::cout << "cam_Exposure   : " << _cam.get(CAP_PROP_GAIN) << std::endl;

		return result; //���� ����

	}
}	


#endif /// !EMOTION_H