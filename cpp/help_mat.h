#pragma once
#ifndef HELP_MAT_H
#define HELP_MAT_H

#include <opencv2/core.hpp>

//방법1 : 최대값을 255로 정규화 V
//방법2 : 나누기4 하는 방법
void check_8UC1(cv::Mat& data){
	if (data.type() == CV_8UC1) {
		return;
	}
	cv::normalize(data, data, 0, 255, cv::NORM_MINMAX, CV_8UC1, cv::noArray());

}


#endif /// !EMOTION_H