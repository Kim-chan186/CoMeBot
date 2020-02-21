#pragma once
#ifndef IMSHOW_MASTER_H
#define IMSHOW_MASTER_H

#include "mouse_event.h"

#include <opencv2/highgui.hpp>


void Window_HSV_set() {
	//������ ����
	cv::namedWindow("����ȭ ����", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("���� ����", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Ʈ����", cv::WINDOW_AUTOSIZE);


	//"Ʈ����"�����쿡 Ʈ���� ����, �̺�Ʈ ���
	cv::createTrackbar("hue :", "Ʈ����", &Variable::track1, 90, On_track::track1, pa);
	cv::createTrackbar("sat :", "Ʈ����", &Variable::track2, 255, On_track::track2, pa);
	cv::createTrackbar("val :", "Ʈ����", &Variable::track3, 255, On_track::track3, pa);
	cv::createTrackbar("���� :", "Ʈ����", &Variable::track4, 20, On_track::track4, pa);
	cv::createTrackbar("��â :", "Ʈ����", &Variable::track5, 20, On_track::track5, pa);
	//0�϶� �״°� ���� �ؾ���

	//�����쿡 �ݹ��Լ��� ���
	cv::setMouseCallback("���� ����", Mouse_Event, pa);
}
void imshow_master() {
	//������ ���
	cv::imshow("����ȭ ����", img_mask1);
	cv::imshow("���� ����", cpm_img);
}


#endif /// !EMOTION_H