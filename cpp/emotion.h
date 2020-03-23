#pragma once
#ifndef EMOTION_H
#define EMOTION_H
//감정 엔진과 관련된 헤더파일
//mode가 실제 감정을 2차원으로 표현한 것
//emotion은 관성에 의해서 아직 변하지 않은량

///emotion에서 ms단위로 반복되는 루프안에서
///단위시간당(1s) 일정량(%) mode가 변화할때 필요한 함수 emotion2mode() 까지 구현됨

#include "SIndex.h"
#include <math.h>


namespace emotion {

	const double vanishing_percent = 0.1; //10%

	const double inertial_percent = 0.3; //30%
}

using namespace std;

class Emotion
{
private:
	cv::Point2d mode = cv::Point2d(0, 0);

	cv::Point2d emotion = cv::Point2d(0, 0);

	int *event, *event_flag, *time_seed,
		line_gain; //coordinate Map line's gain(haed of arrow)

	cv::Mat mode_map, emotion_map, event_map, bg, frame;

	SIndex mode_index; //String to int map, Search of

	std::string txt; //save txt file name

public:
	//생성자 소멸자
	Emotion();
	Emotion(cv::Point emotion);
	Emotion(cv::Point emotion, cv::Point mode);
	Emotion(int* _time_seed);
	Emotion(string* _str, int _str_num, int* _time_seed);
	Emotion(cv::Point mode, cv::Point emotion, int* _time_seed);
	~Emotion();


	//기능 함수
	bool delta();
	bool vanishing();
	bool mode2emotion();
	void get(cv::Point& emotion, cv::Point& mode);

	void print_emotion();
	void print_mode();

	void process();
};//End class


void Emotion::get(cv::Point& emotion, cv::Point& mode){
	emotion = this->emotion;
	mode = this->mode;
}


/// 1000 / (*time_seed) 1초 동안 루프의 반복횟수
/// percent 1초당 원하는 이동량(퍼센트)
///등비수열의 합을 로그로 근사하여 계산한 식
bool Emotion::delta() {
	this->mode2emotion();
	return this->vanishing();
}


bool Emotion::mode2emotion() {
	if (!*time_seed) {
		return 0;
	}
	else {
		double num = pow(1 - emotion::inertial_percent, 1 / (1000.0 / (*time_seed) - 1));
		cv::Point2d delta = mode * num;
		this->emotion += mode - delta;
		this->mode = delta;

		return 1;
	}
}


bool Emotion::vanishing() {
	if (!*time_seed) {
		cout << "\n ** emtion.h error time_seed is zero !!\n";
		return 0;
	}
	else {
		double num = pow(1 - emotion::vanishing_percent, 1 / (1000.0 / (*time_seed) - 1));
		cv::Point2d delta = emotion * num;
		this->emotion = delta;

		return 1;
	}
}


void Emotion::process() {
	std::cout << "gg";
}


void  Emotion::print_emotion() {
	printf("emotion : (%lf, %lf) \n", this->emotion.x, this->emotion.y);
}


void  Emotion::print_mode() {
	printf("mode : (%lf, %lf) \n", this->mode.x, this->mode.y);
}


// ** 생성자 ** //
Emotion::Emotion(cv::Point emotion, cv::Point mode, int* _time_seed) {
	this->mode = mode;
	this->emotion = emotion;
	this->time_seed = _time_seed; ///단위 ms
}


///_time_seed관련 코드 수정 필요
Emotion::Emotion(cv::Point emotion, cv::Point mode){
	this->mode = mode;
	this->emotion = emotion;
}


Emotion::Emotion(cv::Point mode)
	: Emotion(mode, cv::Point(0, 0)) {}


Emotion::Emotion(){}



Emotion::Emotion(int* _time_seed) : Emotion() {
	this->time_seed = _time_seed; ///단위 ms
}


///파일에서 읽어오는 형태로 변경예정, txt에서 초기 설정값을 읽어오는 형태
///미완성
Emotion::Emotion(string* _str, int _str_num, int* _time_seed)
	: Emotion(_time_seed)
{
	this->mode_index = SIndex(_str, _str_num);
}


// ** 소멸자 ** //
Emotion::~Emotion()
{
}

#endif /// !EMOTION_H

/*
	TEST emotion2mode()

		int ms = 50;
		Emotion e(&ms);
		for (int i = 0; i < 1000.0 / ms; i++) {
			e.show_emotion();
			e.show_mode();
			Sleep(ms);
			e.emotion2mode();
		}
	*/

/*
	//TEST delta()

	Mat frame = Mat();
	VideoCapture cam = VideoCapture();
	cam::cam_Initialize(cam, frame, 0);

	HSV_set();
	Window_HSV_set();

	int ms = 50;
	
	Point emotion = Point(50, 50), 
		mode = Point(100, 100);
	Emotion e(emotion, mode, &ms);

	for (int i = 0; chan::key_event(cam); cam >> frame, i++) {
		
		HSV(frame);
		cam::ground_projection(frame);//proto

		e.delta();
		e.get(emotion, mode);
		gui_main(emotion, mode, emotion.x, emotion.y);
		imshow_master();
		waitKey(1);
	}
*/