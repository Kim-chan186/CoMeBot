#pragma once
#ifndef HSV_H
#define HSV_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "cam.h"


//<수정 금지>//
#define HUE_8B_MAX 180
#define SAT_OR_VAL_8B_MAX 255

#define HUE_8B_MIN 0
#define SAT_OR_VAL_8B_MIN 0


//<수정 가능>//
#define  HUE_GRAVITY         10
#define  SATURATION_GRAVITY  80
#define  VALUE_GRAVITY       90



class Transf_Color_Area {
	int High1[3], Low1[3], High2[3], Low2[3], key = 0;
	int vaule[3];
	unsigned int Transfer_Error[3] = { HUE_GRAVITY, SATURATION_GRAVITY, VALUE_GRAVITY };

public:
	Transf_Color_Area() {}
	Transf_Color_Area(const int& vaule1, const int& vaule2, const int& vaule3) {//생성자 > 기준 값 set 후, 색 영역지정
		this->set_Color(vaule1, vaule2, vaule3);
	}
	void set_Color(const int& vaule1, const int& vaule2, const int& vaule3) {//기준 값 set 후, 색 영역지정
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

	void Set_Tolerance_Error(int vaule, int Dimen) {//가중치 변경 후, 색 영역 변경
		if ((Dimen > -1) || (Dimen < 3))
			Transfer_Error[Dimen] = vaule;
		else
			std::cout << endl << "error! piz recording!" << endl;


		this->set_Color_Area();//값은 1개만 변하는데 3번 계산함
	}

	void set_Color_Area() { //색 영역지정

		for (int i = 0; i < 3; i++) {
			int high = vaule[i] + Transfer_Error[i];
			int low = vaule[i] - Transfer_Error[i];

			//Hue 설정
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
				}///범위가 1개 인경우를 else로 지정 범위가 1개임을 표시
				///위 또는 아래로 영역이 넘어간 경우(180또는 0) 범위2개를 이용해서 표기

			 //saturation, value 설정
			 //영역 넘어가면 아래영역으로 채우는 코드로 수정하자
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


	~Transf_Color_Area() {//소멸자

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
int Variable::track4 = 3; // 수축
int Variable::track5 = 3; // 팽창


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


Transf_Color_Area Area;				//설정한 색을 기준으로 허용 영역을 만듦
Transf_Color_Area* pa = &Area;		//설정한 색의 영역 범위에 따라서 색변환 횟수 지정

//////////////////////////////////////////////////////////////////////////////////////////////

Point point_past[2] = { Point(0,0),Point(420,0) };
Point point[2];

Mat cpm_img;

bool HSV_set() {
	int range_count = 0;         //일반 변수

	Scalar red(0, 0, 255);         //색상 지정
	Scalar blue(255, 0, 0);
	Scalar green(0, 255, 0);

	Mat bgr_color = Mat(250, 250, CV_8UC3, red);
	//이미지 객체 생성  1*1픽셀크기
	//CV_8UC3 >> 8비트 1채널(180도 255 255)
	//(0, 0, 255)로 초기화
	cv::imshow("컬러 테스트", bgr_color);
	Mat hsv_color;

	cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);
	//값을 계산하면서 저장하는 방식
	//변환 전후 객체가 달라야 정상적으로 작동

	int hue = (int)hsv_color.at<Vec3b>(0, 0)[0];       //값들을 변수에 저장
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
	cvtColor(cpm_img, img_hsv, COLOR_BGR2HSV);	//지정한 hsv 범위를 이용하여 영상을 이진화

	inRange(img_hsv, pa->Low_Scalar(), pa->High_Scalar(), img_mask1);
	if (pa->get_key()) {
		inRange(img_hsv, pa->Low_Scalar(true), pa->High_Scalar(true), img_mask2);
		img_mask1 |= img_mask2;
	}///이진화를 할 색상이 영역이 2개의 부분으로 지정된 경우(빨간색) 2번째 영역에대한 이진화를 해 더함
	//검정부분의 값이 0임으로 비트연산으로 더하면흰 부분의 색상이 겹침


	////morphological opening 작은 점들을 제거(수축)
	erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track4, Variable::track4)));

	/*dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track4 + Variable::track5, Variable::track4)));*/


		////morphological closing 영역의 구멍 메우기(팽창)
	dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track5, Variable::track5)));

	/*erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track5, Variable::track5)));*/



	Mat img_labels, stats, centroids;
	int numoflables = connectedComponentsWithStats(img_mask1, img_labels,
		stats, centroids, 8, CV_32S);

	//라벨링
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


	//유효 라벨 찾기
	for (int j = 1; j < numoflables; j++) {

		//모든 라벨 그리기
		int left = stats.at<int>(j, CC_STAT_LEFT);
		int top = stats.at<int>(j, CC_STAT_TOP);
		int width = stats.at<int>(j, CC_STAT_WIDTH);
		int height = stats.at<int>(j, CC_STAT_HEIGHT);


		if ((j == *idx) | (j == *(idx + 1))) {
			//사용할 라벨 그리기
			rectangle(cpm_img, Point(left, top), Point(left + width, top + height),
				Scalar(255, 0, 255), 2);

		}
		else {
			//사용하지 않는 라벨 그리기
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

	//중간점
	rectangle(cpm_img, point[0], point[0] + 10,
		Scalar(255, 255, 0), 2);
	rectangle(cpm_img, point[1], point[1] + 10,
		Scalar(255, 0, 255), 2);
	/*
	Mat img_show = imread("물고기.PNG");

	cvtColor(img_mask1 + 70, img_mask1, COLOR_GRAY2BGR);
	img_show = ~(~img_mask1 + ~img_show);
	*/
	return 1;
}// END_HSV


#endif /// !EMOTION_H