#pragma once
#ifndef IMSHOW_MASTER_H
#define IMSHOW_MASTER_H

#include "mouse_event.h"

#include <opencv2/highgui.hpp>


void Window_HSV_set() {
	//������ ����
	cv::namedWindow("mask", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("frame", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("console", cv::WINDOW_NORMAL);


	cv::resizeWindow("console", 1000, 1000);
	cv::moveWindow("console", 0, 0);


	//"console"�����쿡 Ʈ���� ����, �̺�Ʈ ���
	cv::createTrackbar("hue :", "console", &Variable::track1, 90, On_track::track1, pa);
	cv::createTrackbar("sat :", "console", &Variable::track2, 255, On_track::track2, pa);
	cv::createTrackbar("val :", "console", &Variable::track3, 255, On_track::track3, pa);
	cv::createTrackbar("���� :", "console", &Variable::track4, 20, On_track::track4, pa);
	cv::createTrackbar("��â :", "console", &Variable::track5, 20, On_track::track5, pa);
	//0�϶� �״°� ���� �ؾ���

	//�����쿡 �ݹ��Լ��� ���
	cv::setMouseCallback("���� ����", Mouse_Event, pa);
}


void imshow_master(Mat& frame, Mat& mask){
	//������ ���
	cv::imshow("mask", mask);
	cv::imshow("frame", frame);
}

#endif /// !EMOTION_H