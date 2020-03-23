#pragma once
#ifndef HSV_H
#define HSV_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "cam.h"

#include "Debug.h"

//<수정 금지>//
#define HUE_8B_MAX		180
#define SAT_VAL_8B_MAX	255

#define HUE_8B_MIN		0
#define SAT_VAL_8B_MIN	0

#define ENABLE			1
#define DISABLE			0

//---- control factor------//
//#define BRIGHTNESS		1
//#define GAIN			20
//#define WHITE_BALANCE	300
//
//#define BRIGHTNESS0		1
//#define GAIN0			20
//#define WHITE_BALANCE0	300
//
#define HUE_GRAVITY         4
#define SATURATION_GRAVITY  50
#define VALUE_GRAVITY       50
//
//#define WIDTH 848
//#define HEIGHT 480
//#define FPS 60
// --------------------///



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




class Color_Area {

	int High1[3], Low1[3], 
		High2[3], Low2[3], 
		key = 0;

	int vaule[3];

	Size _dilate = Size(3,3),   //팽창
		_erode = Size(3, 3);    //수축

	Scalar hsv_Scalar[4];								//0:H1  1:L1  2:H2  3:L2
	

	//허용 오차; 범위의 크기
	unsigned int Transfer_Error[3] = { HUE_GRAVITY,
										SATURATION_GRAVITY,
										VALUE_GRAVITY };



	void set_color_Area(int Dimen) {					//Dimen : 0(h),1(s),2(v)    3(모두)


		if (Dimen == 3) {
			this->set_color_Area(0);

			this->set_color_Area(1);

			this->set_color_Area(2);

			return;
		}

		else if ((Dimen > 3) | (Dimen < 0)) {
			cout << "not correct Dimen !!" << endl;
			return;
		}

		
		int high = vaule[Dimen] + Transfer_Error[Dimen];

		int low = vaule[Dimen] - Transfer_Error[Dimen];
		
		//Hue 설정
		if (Dimen == 0) {
			cout << setfill('0');

			if (low < HUE_8B_MIN) {
				this->key = 1;

				High1[0] = HUE_8B_MAX;
				Low1[0] = low + HUE_8B_MAX;
				High2[0] = high;
				Low2[0] = HUE_8B_MIN;
			}
			else if (high > HUE_8B_MAX) {
				this->key = 2;

				High1[0] = HUE_8B_MAX;
				Low1[0] = low;
				High2[0] = high - HUE_8B_MAX;
				Low2[0] = HUE_8B_MIN;
			}
			else {
				this->key = 3;

				High1[0] = high;
				Low1[0] = low;
				High2[0] = this->High1[0];
				Low2[0] = this->High1[0];
			}
			///범위가 1개 인경우를 else로 지정 범위가 1개임을 표시
			///위 또는 아래로 영역이 넘어간 경우(180또는 0) 범위2개를 이용해서 표기
		}
		//saturation, value 설정
		else {
			if (low < SAT_VAL_8B_MIN) {

				High1[Dimen] = high;
				Low1[Dimen] = SAT_VAL_8B_MIN;

				High2[Dimen] = High1[Dimen];
				Low2[Dimen] = Low1[Dimen];
			}
			else if (high > SAT_VAL_8B_MAX) {

				High1[Dimen] = SAT_VAL_8B_MAX;
				Low1[Dimen] = low;

				High2[Dimen] = High1[Dimen];
				Low2[Dimen] = Low1[Dimen];
			}
			else {

				High1[Dimen] = high;
				Low1[Dimen] = low;

				High2[Dimen] = this->High1[Dimen];
				Low2[Dimen] = this->Low1[Dimen];

			}
		}
	}//set_color_Area End



	void set_color_bgr(int b, int g, int r) {

		Scalar sca_hsv = bgr2hsv(b, g, r);
		
		
		this->vaule[0] = sca_hsv.val[0];
		
		this->vaule[1] = sca_hsv.val[1];

		this->vaule[2] = sca_hsv.val[2];
		
	}//End set_color_bgr



	void sca2hsv() {

		hsv_Scalar[0] = Scalar(High1[0], High1[1], High1[2]);

		hsv_Scalar[1] = Scalar(Low1[0], Low1[1], Low1[2]);

		hsv_Scalar[2] = Scalar(High2[0], High2[1], High2[2]);

		hsv_Scalar[3] = Scalar(Low2[0], Low2[1], Low2[2]);

	}//End sca2hsv()()



	void show_target() const {

		Mat bgr_color_PNG = imread("img/color.jpg"),
			img_mask,
			gray_PNG;

		gray_PNG = this->binarization(bgr_color_PNG);

		cvtColor(gray_PNG + 70, img_mask, COLOR_GRAY2BGR);
		img_mask = ~(~img_mask + ~bgr_color_PNG);

		resize(img_mask, bgr_color_PNG, Size(280, 300), 0, 0, INTER_LINEAR);
		cv::imshow("target", bgr_color_PNG);

	}//show_target



public:

	Color_Area() {}



	Color_Area(const int& vaule1, const int& vaule2, const int& vaule3) {						//BGR색으로 초기화
		this->set_bgr(vaule1, vaule2, vaule3);
	}



	Color_Area(Scalar s) {
		this->set_bgr(s.val[0], s.val[1], s.val[2]);
	}


	void set_bgr(const Scalar s) {
		//서순서순
		this->set_bgr(s[2], s[1], s[0]);
	}




	void set_bgr(const int& vaule1, const int& vaule2, const int& vaule3) {						//외부 호출용
		
		printf("                         (b,g,r) : (%.3d, %.3d, %.3d)    \n", vaule1, vaule2, vaule3);
		
		this->set_color_bgr(vaule1, vaule2, vaule3);
		
		this->set_color_Area(3);
		
		this->print_Area();
		
		this->sca2hsv();
		
		this->show_target();

	}//End set_bgr



	void set_Gravity(int vaule, int Dimen) {			//Dimen : 0(h),1(s),2(v)    3(모두)
		
		if ((Dimen > -1) || (Dimen < 3))
			Transfer_Error[Dimen] = vaule;
		else
			//예외처리
			std::cout << endl << "\n ** set_Gravity error! piz recording! \n" << endl;


		this->set_color_Area(Dimen);

		this->print_Area();

		this->sca2hsv();

		this->show_target();

	}//End set_Gravity



	//0 : 팽창, 1: 수축
	void set_morpholog(bool flag, int val) {

		if (flag == 0) {
			this->_erode = Size(val, val); //수축
		}
		else
			this->_dilate = Size(val, val); //팽창

		this->show_target();
	}



	void print_Area() const {

		std::cout
			<< "HSV Area Type <" << this->key << ">       (h,s,v) : (" << setw(3) << vaule[0] << ", " << setw(3) << vaule[1] << ", " << setw(3) << vaule[2] << ")" << endl
			<< "*******************< key : " << this->key << " >*******************" << endl;

		if (this->key == 3) {
			std::cout
				<< "                     hue : " << setw(3) << this->Low1[0] << "  ~  " << setw(3) << this->High1[0] << endl;
		}

		else if ((this->key == 1) | (this->key == 2)) {
			std::cout
				<< "                     hue : " << setw(3) << this->Low1[0] << " ~ 180(0) ~ " << setw(3) << this->High2[0] << endl;

		}

		else {
			std::cout << "\n\n\n     Key Error!!     \n\n\n";
			return;
		}

		std::cout
			<< "              saturation : " << setw(3) << this->Low1[1] << "  ~  " << setw(3) << this->High1[1] << endl
			<< "                   value : " << setw(3) << this->Low1[2] << "  ~  " << setw(3) << this->High1[2] << endl
			<< "*************************************************" << endl << endl << endl;

	}//End print_Area()



	Scalar bgr2hsv(int b, int g, int r) const {

		Mat _color(1, 1, CV_8UC3, Scalar(b, g, r));


		cvtColor(_color, _color, COLOR_BGR2HSV);
		
		int h = _color.at<Vec3b>(0, 0)[0];
		
		int s = _color.at<Vec3b>(0, 0)[1];
		
		int v = _color.at<Vec3b>(0, 0)[2];

		
		return Scalar(h, s, v);
	}//end bgr2hsv



	Scalar hsv2bgr(int h, int s, int v) const {

		Mat _color(1, 1, CV_8UC3, Scalar(h, s, v));

		cvtColor(_color, _color, COLOR_HSV2BGR);



		uint b = _color.at<Vec3b>(0, 0)[0];

		uint g = _color.at<Vec3b>(0, 0)[1];

		uint r = _color.at<Vec3b>(0, 0)[2];


		return Scalar(b, g, r);

	}//end hsv2bgr



	bool get_key() const {
		return key;
	}



	Scalar High_Scalar(int key = 0) const {
		return hsv_Scalar[0 + key * 2];
	}



	Scalar Low_Scalar(int key = 0) const {
		return hsv_Scalar[1 + key * 2];
	}



	Mat binarization(const Mat& _img) const {

		Mat _img_mask1,
			_img_mask2,
			_img_hsv;

		cvtColor(_img, _img_hsv, COLOR_BGR2HSV);
		

		inRange(_img_hsv,
			this->Low_Scalar(),
			this->High_Scalar(),
			_img_mask1
		);		//지정한 hsv 범위를 이용하여 영상을 이진화
		

		if ((this->key == 1) | (this->key == 2)) {

			inRange(_img_hsv,
				this->Low_Scalar(true),
				this->High_Scalar(true),
				_img_mask2);

			_img_mask1 |= _img_mask2;
		}
		
			//morphological closing 모폴로지 열림연산
		dilate(_img_mask1, _img_mask1, getStructuringElement(MORPH_ELLIPSE, this->_dilate));
		
		erode(_img_mask1, _img_mask1, getStructuringElement(MORPH_ELLIPSE, this->_erode));
		cout << _dilate << "D\n";
		cout << _erode << "D\n";
		return _img_mask1;
	}



	~Color_Area() {//소멸자
	}
};



Color_Area* pa = new Color_Area;			//이진화와 색공간을 담당하는 객체



class On_track {

public:
	static void track1(int track_vaule, void* userdata) {
		((Color_Area*)userdata)->set_Gravity(track_vaule, 0);
	}


	static void track2(int track_vaule, void* userdata) {
		((Color_Area*)userdata)->set_Gravity(track_vaule, 1);
	}


	static void track3(int track_vaule, void* userdata) {
		((Color_Area*)userdata)->set_Gravity(track_vaule, 2);
	}


	static void track4(int track_vaule, void* userdata) {
		if (track_vaule == 0) {
			track_vaule = 1;
			return;
		}
		((Color_Area*)userdata)->set_morpholog(1, track_vaule);
	}


	static void track5(int track_vaule, void* userdata) {
		if (track_vaule == 0) {
			track_vaule = 1;
			return;
		}
		else if (userdata == nullptr) {
			cout << "\n ** pa is nullptr \n";
		}
		((Color_Area*)userdata)->set_morpholog(0, track_vaule);
	}
}; // End  Color_Area


				




//1. hsv
//2. 
bool binary_set(Scalar s) {

	pa->set_bgr(s);
		

	return 1;

}//End HSV_set()



bool HSV(Mat& _cpm_img, Mat& _img_mask){

	Mat img_hsv;

	cvtColor(_cpm_img, img_hsv, COLOR_BGR2HSV);	//지정한 hsv 범위를 이용하여 영상을 이진화


	inRange(img_hsv, pa->Low_Scalar(), pa->High_Scalar(), _img_mask);

	if (pa->get_key()) {
		Mat img_mask2;

		inRange(img_hsv, pa->Low_Scalar(true), pa->High_Scalar(true), img_mask2);
		
		_img_mask |= img_mask2;
	}///이진화를 할 색상이 영역이 2개의 부분으로 지정된 경우(빨간색) 2번째 영역에대한 이진화를 해 더함
	//검정부분의 값이 0임으로 비트연산으로 더하면흰 부분의 색상이 겹침



	////morphological opening 작은 점들을 제거(수축)
	erode(_img_mask, _img_mask, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track4, Variable::track4)));

	/*dilate(_img_mask, _img_mask, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track4 + Variable::track5, Variable::track4)));*/

	////morphological closing 영역의 구멍 메우기(팽창)
	dilate(_img_mask, _img_mask, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track5, Variable::track5)));

	/*erode(_img_mask, _img_mask, getStructuringElement(MORPH_ELLIPSE,
		Size(Variable::track5, Variable::track5)));*/



	Mat img_labels, stats, centroids;

	int numoflables = connectedComponentsWithStats(_img_mask, img_labels,
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
			rectangle(_cpm_img, Point(left, top), Point(left + width, top + height),
				Scalar(255, 0, 255), 2);

		}
		else {
			//사용하지 않는 라벨 그리기
			rectangle(_cpm_img, Point(left, top), Point(left + width, top + height),
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
	}

	/*
	Mat img_show = imread("물고기.PNG");

	cvtColor(img_mask1 + 70, img_mask1, COLOR_GRAY2BGR);
	img_show = ~(~img_mask1 + ~img_show);
	*/
	return 1;
}// END_HSV



#endif /// !EMOTION_H