#pragma once
#ifndef IMSHOW_MASTER_H
#define IMSHOW_MASTER_H

#include "mouse_event.h"

#include <opencv2/highgui.hpp>


void Window_HSV_set() {
	//윈도우 생성
	cv::namedWindow("이진화 영상", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("원본 영상", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("트렉바", cv::WINDOW_AUTOSIZE);


	//"트렉바"윈도우에 트렉바 생성, 이벤트 등록
	cv::createTrackbar("hue :", "트렉바", &Variable::track1, 90, On_track::track1, pa);
	cv::createTrackbar("sat :", "트렉바", &Variable::track2, 255, On_track::track2, pa);
	cv::createTrackbar("val :", "트렉바", &Variable::track3, 255, On_track::track3, pa);
	cv::createTrackbar("수축 :", "트렉바", &Variable::track4, 20, On_track::track4, pa);
	cv::createTrackbar("팽창 :", "트렉바", &Variable::track5, 20, On_track::track5, pa);
	//0일때 죽는거 수정 해야함

	//윈도우에 콜백함수를 등록
	cv::setMouseCallback("원본 영상", Mouse_Event, pa);
}
void imshow_master() {
	//윈도우 출력
	cv::imshow("이진화 영상", img_mask1);
	cv::imshow("원본 영상", cpm_img);
}


#endif /// !EMOTION_H