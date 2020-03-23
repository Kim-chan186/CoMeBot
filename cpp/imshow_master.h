#pragma once
#ifndef IMSHOW_MASTER_H
#define IMSHOW_MASTER_H

#include "mouse_event.h"
#include "HSV.h"
#include "cam.h"
#include <opencv2/highgui.hpp>


void Window_set() {
	//윈도우 생성
	//cv::namedWindow("mask", cv::WINDOW_AUTOSIZE);

	cv::namedWindow("frame", cv::WINDOW_AUTOSIZE);		//main, 마우스 이벤트

	cv::namedWindow("Con", cv::WINDOW_NORMAL);			//console로 생성할 경우 의도치 않은 오류 가능성

	cv::namedWindow("target", cv::WINDOW_NORMAL);



	cv::resizeWindow("con", 300, 300);

	cv::resizeWindow("target", 280, 300);



	cv::moveWindow("Con", 0, 0);

	cv::moveWindow("target", 0, 700);

	cv::moveWindow("frame", 350, 0);


	
	//"console"윈도우에 트렉바 생성, 이벤트 등록
	cv::createTrackbar("hue :", "Con", &Variable::track1, 180, On_track::track1, pa);

	cv::createTrackbar("sat :", "Con", &Variable::track2, 255, On_track::track2, pa);

	cv::createTrackbar("val :", "Con", &Variable::track3, 255, On_track::track3, pa);

	cv::createTrackbar("ero :", "Con", &Variable::track4, 10, On_track::track4, pa); // 수축

	cv::createTrackbar("dil :", "Con", &Variable::track5, 10, On_track::track5, pa); // 팽창
	//0일때 죽는거 수정 해야함
	

	//윈도우에 콜백함수를 등록
	cv::setMouseCallback("frame", Mouse_Event, pa);

	cv::setMouseCallback("target", ME_target, pa);
}


void imshow_master(Mat& frame, Mat& mask){
	//윈도우 출력
	//cv::imshow("mask", mask);
	Mat show;

	cvtColor(mask, mask, cv::COLOR_GRAY2BGR);

	cv::hconcat(frame, mask, mask);

	cv::imshow("frame", mask);
}

#endif /// !EMOTION_H