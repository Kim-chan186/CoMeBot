#pragma once

#ifndef G_PROJECTION_H	//ground projection
#define G_PROJECTION_H



#include <math.h>
#include "cam.h"

using namespace cv;

namespace cam {
	#define pi 3.141592

	void center_line(cv::Mat& frame, Scalar color = Scalar(0, 255, 0)) {

		line(frame, Point(frame.cols / 2, 0), Point(frame.cols / 2, frame.rows), Scalar(0, 255, 0));
		line(frame, Point(0, frame.rows / 2), Point(frame.cols, frame.rows / 2), Scalar(0, 255, 0));
	}



	void Calibration_line(cv::Mat& frame, int angle = 30, Scalar color = Scalar(0, 0, 255)) {
		Point center(frame.cols / 2, frame.rows / 2);

		//�밢�����̸� �������� ���� �׸�
		int radius = pow(frame.rows*frame.rows + frame.cols*frame.cols, .5);

		for (int i = 0; angle*i <= 180; i++) {
			int y = radius * sin((angle * i * pi) / 180.0);
			int x = radius * cos((angle * i * pi) / 180.0);

			line(frame, center + Point(x, y), center - Point(x, y), color);
			//circle(frame, center + Point(x1, y1), radius, Scalar(255 - i * 30, 0, i * 30), -1);
		}
		//�߾� ���ڼ�
		center_line(frame);
	}//End center_line



	//angle �����ϰ� �����
	void parallel_line(cv::Mat& frame, int angle = 60, Scalar color = Scalar(255, 0, 0)) {
		for (int i = 0; angle*i <= frame.rows; i++) {
			line(frame, Point(0, angle*i), Point(frame.cols, angle*i), color);
			//circle(frame, center + Point(x1, y1), radius, Scalar(255 - i * 30, 0, i * 30), -1);
		}
	}



	//Microsoft_HD_Camera�� _ froto
	bool ground_projection(cv::Mat& frame) {
		parallel_line(frame);

		// Calibration_line(frame, 5, Scalar(0, 0, 255));
		Calibration_line(frame); //, Scalar(255, 0, 255));

		return 1;
	}

	/*
		�߾� �Ʒ��� 3��� ���� ���
		1. ���ϴ� 1.5m ����
		2. �ߴ� 2.25m ���� (y = 480 / 2 + 480 /2 /3 *2 = 400)
		3. ��� ������ ���� 4m�̻�
	*/
}




#endif /// !EMOTION_H