#pragma once
#ifndef IMSHOW_MASTER_H
#define IMSHOW_MASTER_H

#include "mouse_event.h"
#include "HSV.h"
#include "cam.h"
#include <opencv2/highgui.hpp>


void Window_set() {
	//������ ����
	//cv::namedWindow("mask", cv::WINDOW_AUTOSIZE);

	cv::namedWindow("frame", cv::WINDOW_AUTOSIZE);		//main, ���콺 �̺�Ʈ

	cv::namedWindow("Con", cv::WINDOW_NORMAL);			//console�� ������ ��� �ǵ�ġ ���� ���� ���ɼ�

	cv::namedWindow("target", cv::WINDOW_NORMAL);



	cv::resizeWindow("con", 300, 300);

	cv::resizeWindow("target", 280, 300);



	cv::moveWindow("Con", 0, 0);

	cv::moveWindow("target", 0, 700);

	cv::moveWindow("frame", 350, 0);


	
	//"console"�����쿡 Ʈ���� ����, �̺�Ʈ ���
	cv::createTrackbar("hue :", "Con", &Variable::track1, 180, On_track::track1, pa);

	cv::createTrackbar("sat :", "Con", &Variable::track2, 255, On_track::track2, pa);

	cv::createTrackbar("val :", "Con", &Variable::track3, 255, On_track::track3, pa);

	cv::createTrackbar("ero :", "Con", &Variable::track4, 10, On_track::track4, pa); // ����

	cv::createTrackbar("dil :", "Con", &Variable::track5, 10, On_track::track5, pa); // ��â
	//0�϶� �״°� ���� �ؾ���
	

	//�����쿡 �ݹ��Լ��� ���
	cv::setMouseCallback("frame", Mouse_Event, pa);

	cv::setMouseCallback("target", ME_target, pa);
}


void imshow_master(Mat& frame, Mat& mask){
	//������ ���
	//cv::imshow("mask", mask);
	Mat show;

	cvtColor(mask, mask, cv::COLOR_GRAY2BGR);

	cv::hconcat(frame, mask, mask);

	cv::imshow("frame", mask);
}

#endif /// !EMOTION_H