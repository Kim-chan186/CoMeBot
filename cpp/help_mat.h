#pragma once
#ifndef HELP_MAT_H
#define HELP_MAT_H

#include <opencv2/core.hpp>

//���1 : �ִ밪�� 255�� ����ȭ V
//���2 : ������4 �ϴ� ���
void check_8UC1(cv::Mat& data){
	if (data.type() == CV_8UC1) {
		return;
	}
	cv::normalize(data, data, 0, 255, cv::NORM_MINMAX, CV_8UC1, cv::noArray());

}


#endif /// !EMOTION_H