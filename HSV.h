#pragma once
#ifndef HSV_H
#define HSV_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "cam.h"


//<���� ����>//
#define HUE_8B_MAX 180
#define SAT_OR_VAL_8B_MAX 255

#define HUE_8B_MIN 0
#define SAT_OR_VAL_8B_MIN 0


//<���� ����>//
#define  HUE_GRAVITY         10
#define  SATURATION_GRAVITY  80
#define  VALUE_GRAVITY       90



class Transf_Color_Area {
	int High1[3], Low1[3], High2[3], Low2[3], key = 0;
	int vaule[3];
	unsigned int Transfer_Error[3] = { HUE_GRAVITY, SATURATION_GRAVITY, VALUE_GRAVITY };

public:
	Transf_Color_Area() {}
	Transf_Color_Area(const int& vaule1, const int& vaule2, const int& vaule3) {//������ > ���� �� set ��, �� ��������
		this->set_Color(vaule1, vaule2, vaule3);
	}
	void set_Color(const int& vaule1, const int& vaule2, const int& vaule3) {//���� �� set ��, �� ��������
		vaule[0] = vaule1;
		vaule[1] = vaule2;
		vaule[2] = vaule3;

		std::cout << endl << endl
			<< "----------< set Standard Color >-----------" << endl
			<< "       hue : " << vaule1 << endl
			<< "saturation : " << vaule2 << endl
			<< "     value : " << vaule1 << endl
			<< "------------------------------------------- "
			<< endl << endl;

		this->set_Color_Area();
	}

	void Set_Tolerance_Error(int vaule, int Dimen) {//����ġ ���� ��, �� ���� ����
		if ((Dimen > -1) || (Dimen < 3))
			Transfer_Error[Dimen] = vaule;
		else
			std::cout << endl << "error! piz recording!" << endl;


		this->set_Color_Area();//���� 1���� ���ϴµ� 3�� �����
	}

	void set_Color_Area() { //�� ��������

		for (int i = 0; i < 3; i++) {
			int high = vaule[i] + Transfer_Error[i];
			int low = vaule[i] - Transfer_Error[i];

			//Hue ����
			if (i == 0)
				if (low < HUE_8B_MIN) {
					std::cout << "Hue <1st> set" << endl;
					key = 1;

					High1[0] = HUE_8B_MAX;
					Low1[0] = low + HUE_8B_MAX;
					High2[0] = high;
					Low2[0] = HUE_8B_MIN;
				}
				else if (high > HUE_8B_MAX) {
					std::cout << "Hue <2nd> set" << endl;
					key = 1;

					High1[0] = HUE_8B_MAX;
					Low1[0] = low;
					High2[0] = high - HUE_8B_MAX;
					Low2[0] = HUE_8B_MIN;
				}
				else {
					std::cout << "Hue <3rd> set" << endl;
					key = 0;

					High1[0] = high;
					Low1[0] = low;
					High2[0] = this->High1[0];
					Low2[0] = this->High1[0];
				}///������ 1�� �ΰ�츦 else�� ���� ������ 1������ ǥ��
				///�� �Ǵ� �Ʒ��� ������ �Ѿ ���(180�Ǵ� 0) ����2���� �̿��ؼ� ǥ��

			 //saturation, value ����
			 //���� �Ѿ�� �Ʒ��������� ä��� �ڵ�� ��������
			else {
				if (low < SAT_OR_VAL_8B_MIN) {

					High1[i] = high;
					Low1[i] = SAT_OR_VAL_8B_MIN;

					High2[i] = High1[i];
					Low2[i] = Low1[i];
				}
				else if (high > SAT_OR_VAL_8B_MAX) {
					key = 1;

					High1[i] = SAT_OR_VAL_8B_MAX;
					Low1[i] = low;

					High2[i] = High1[i];
					Low2[i] = Low1[i];
				}
				else {

					High1[i] = high;
					Low1[i] = low;

					High2[i] = this->High1[i];
					Low2[i] = this->Low1[i];
				}
			}

		}
		this->print();

	}
	void print() {

		if (this->get_key() == 0) {
			std::cout << "hue = " << vaule[0] << "   //  saturation = " << vaule[1] << "   //  value = " << vaule[2] << endl;
			std::cout << "**************************< key : 0 >***************************" << endl;
			std::cout << "hue = " << this->High1[0] << "   //  saturation = " << this->High1[1] << "   //  value = " << this->High1[2] << endl;
			std::cout << "hue = " << this->Low1[0] << "   //  saturation = " << this->Low1[1] << "   //  value = " << this->Low1[2] << endl;
			std::cout << "***************************************************************" << endl << endl << endl;
		}
		else if (this->get_key() == 1) {
			std::cout << "hue = " << vaule[0] << "   //  saturation = " << vaule[1] << "   //  value = " << vaule[2] << endl;
			std::cout << "**************************< key : 1 >***************************" << endl;
			std::cout << "hue = " << this->High1[0] << "   //  saturation = " << this->High1[1] << "   //  value = " << this->High1[2] << endl;
			std::cout << "hue = " << this->Low1[0] << "   //  saturation = " << this->Low1[1] << "   //  value = " << this->Low1[2] << endl << endl;

			std::cout << "hue = " << this->High2[0] << "   //  saturation = " << this->High2[1] << "   //  value = " << this->High2[2] << endl;
			std::cout << "hue = " << this->Low2[0] << "   //  saturation = " << this->Low2[1] << "   //  value = " << this->Low2[2] << endl << endl;
			std::cout << "***************************************************************" << endl << endl << endl;
		}
		else {
			std::cout << "\n\n\n     Key Error!!     \n\n\n";
		}
	}

	int get_High(const int dimen, int key = 0) {//
		if (key == 0)
			return High1[dimen];
		else
			return High2[dimen];
	}
	int get_Low(const int dimen, int key = 0) {
		if (key == 0)
			return Low1[dimen];
		else
			return Low2[dimen];
	}

	bool get_key() {//
		return key;
	}


	Scalar High_Scalar(int key = 0) {//
		return Scalar(get_High(0, key), get_High(1, key), get_High(2, key));
	}
	Scalar Low_Scalar(int key = 0) {
		return Scalar(get_Low(0, key), get_Low(1, key), get_Low(2, key));
	}


	~Transf_Color_Area() {//�Ҹ���

	}

};




class Variable {
public:
	static int track1;
	static int track2;
	static int track3;
	static int track4;
	static int track5;
	//static Mat cpm_img;

};
//Mat Variable::cpm_img(1, 1, CV_8UC3, Scalar(0, 0, 0));
int Variable::track1 = HUE_GRAVITY;
int Variable::track2 = SATURATION_GRAVITY;
int Variable::track3 = VALUE_GRAVITY;
int Variable::track4 = 3; // ����
int Variable::track5 = 3; // ��â


class On_track {
public:
	static void track1(int track_vaule, void* userdata) {
		((Transf_Color_Area*)userdata)->Set_Tolerance_Error(track_vaule, 0);
	}
	static void track2(int track_vaule, void* userdata) {
		((Transf_Color_Area*)userdata)->Set_Tolerance_Error(track_vaule, 1);
	}
	static void track3(int track_vaule, void* userdata) {
		((Transf_Color_Area*)userdata)->Set_Tolerance_Error(track_vaule, 2);
	}
	static void track4(int track_vaule, void* userdata) {
		if (track_vaule == 0) {
			track_vaule = 1;
			return;
		}
		track_vaule = (*(int*)userdata);
	}
	static void track5(int track_vaule, void* userdata) {
		if (track_vaule == 0) {
			track_vaule = 1;
			return;
		}
		track_vaule = (*(int*)userdata);
	}
};


Transf_Color_Area Area;				//������ ���� �������� ��� ������ ����
Transf_Color_Area* pa = &Area;		//������ ���� ���� ������ ���� ����ȯ Ƚ�� ����

//////////////////////////////////////////////////////////////////////////////////////////////

Point point_past[2] = { Point(0,0),Point(420,0) };
Point point[2];

Mat cpm_img;

bool HSV_set() {
	int range_count = 0;         //�Ϲ� ����

	Scalar red(0, 0, 255);         //���� ����
	Scalar blue(255, 0, 0);
	Scalar green(0, 255, 0);

	Mat bgr_color = Mat(250, 250, CV_8UC3, red);
	//�̹��� ��ü ����  1*1�ȼ�ũ��
	//CV_8UC3 >> 8��Ʈ 1ä��(180�� 255 255)
	//(0, 0, 255)�� �ʱ�ȭ
	cv::imshow("�÷� �׽�Ʈ", bgr_color);
	Mat hsv_color;

	cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);
	//���� ����ϸ鼭 �����ϴ� ���
	//��ȯ ���� ��ü�� �޶�� ���������� �۵�

	int hue = (int)hsv_color.at<Vec3b>(0, 0)[0];       //������ ������ ����
	int saturation = (int)hsv_color.at<Vec3b>(0, 0)[1];
	int value = (int)hsv_color.at<Vec3b>(0, 0)[2];


	Area = Transf_Color_Area(hue, saturation, value);
	return 1;
}//End HSV_set()

Mat img_hsv;
Mat img_mask1;
Mat img_mask2;

bool HSV(Mat& _cpm_img){
	cpm_img = _cpm_img;
	cvtColor(cpm_img, img_hsv, COLOR_BGR2HSV);	//������ hsv ������ �̿��Ͽ� ������ ����ȭ

	inRange(img_hsv, pa->Low_Scalar(), pa->High_Scalar(), img_mask1);
	if (pa->get_key()) {
		inRange(img_hsv, pa->Low_Scalar(true), pa->High_Scalar(true), img_mask2);
		img_mask1 |= img_mask2;
	}///����ȭ�� �� ������ ������ 2���� �κ����� ������ ���(������) 2��° ���������� ����ȭ�� �� ����
	//�����κ��� ���� 0������ ��Ʈ�������� ���ϸ��� �κ��� ������ ��ħ


	////morphological opening ���� ������ ����(����)
	erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track4, Variable::track4)));

	/*dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track4 + Variable::track5, Variable::track4)));*/


		////morphological closing ������ ���� �޿��(��â)
	dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track5, Variable::track5)));

	/*erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track5, Variable::track5)));*/



	Mat img_labels, stats, centroids;
	int numoflables = connectedComponentsWithStats(img_mask1, img_labels,
		stats, centroids, 8, CV_32S);

	//�󺧸�
	int max[2] = { -1 - 1 }, idx[2] = { 0, 0 };

	for (int j = 1; j < numoflables; j++) {
		int area = stats.at<int>(j, CC_STAT_AREA);

		if (area > 100) {

			if (*max < area) {
				*(max + 1) = *max;
				*(idx + 1) = *idx;

				*max = area;
				*idx = j;
			}
			else if (*(max + 1) < area) {
				*(max + 1) = area;
				*(idx + 1) = j;
			}
		}
	}


	//��ȿ �� ã��
	for (int j = 1; j < numoflables; j++) {

		//��� �� �׸���
		int left = stats.at<int>(j, CC_STAT_LEFT);
		int top = stats.at<int>(j, CC_STAT_TOP);
		int width = stats.at<int>(j, CC_STAT_WIDTH);
		int height = stats.at<int>(j, CC_STAT_HEIGHT);


		if ((j == *idx) | (j == *(idx + 1))) {
			//����� �� �׸���
			rectangle(cpm_img, Point(left, top), Point(left + width, top + height),
				Scalar(255, 0, 255), 2);

		}
		else {
			//������� �ʴ� �� �׸���
			rectangle(cpm_img, Point(left, top), Point(left + width, top + height),
				Scalar(0, 0, 170), 1);
		}

	}



	for (int i = 0; (i < numoflables)&(i < 2); i++) {
		int left = stats.at<int>(*(idx + i), CC_STAT_LEFT);
		int top = stats.at<int>(*(idx + i), CC_STAT_TOP);
		int width = stats.at<int>(*(idx + i), CC_STAT_WIDTH);
		int height = stats.at<int>(*(idx + i), CC_STAT_HEIGHT);

		//int y = top + height;
		//int x = left + width / 2;
		point[i] = Point(left + width / 2, top + height);
	}

	if ((point[0] * point_past[0]) >
		(point[0] * point_past[1])) {

		Point point_buf = point[0];
		point[0] = point[1];
		point[1] = point_buf;
	}

	//�߰���
	rectangle(cpm_img, point[0], point[0] + 10,
		Scalar(255, 255, 0), 2);
	rectangle(cpm_img, point[1], point[1] + 10,
		Scalar(255, 0, 255), 2);
	/*
	Mat img_show = imread("�����.PNG");

	cvtColor(img_mask1 + 70, img_mask1, COLOR_GRAY2BGR);
	img_show = ~(~img_mask1 + ~img_show);
	*/
	return 1;
}// END_HSV


#endif /// !EMOTION_H