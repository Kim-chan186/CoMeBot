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
	const double percent = 0.1; //10%
}

using namespace std;

class Emotion
{
private:
	cv::Point2d mode;
	cv::Point2d emotion;

	int *event, *event_flag, *time_seed,
		line_gain; //coordinate Map line's gain(haed of arrow)

	cv::Mat mode_map, emotion_map, event_map, bg, frame;
	SIndex mode_index; //String to int map, Search of
	std::string txt; //save txt file name

public:
	Emotion();
	Emotion(int* _time_seed);
	Emotion(string* _str, int _str_num, int* _time_seed);
	~Emotion();

	cv::Point emotion2mode();

	void show_emotion();
	void show_mode();

	void process();
};

cv::Point Emotion::emotion2mode() {
	double num;
	cv::Point2d delta;
	/// 1000 / (*time_seed) 1초 동안 루프의 반복횟수
	/// percent 1초당 원하는 이동량(퍼센트)
	///등비수열의 합을 로그로 근사하여 계산한 식
	num = pow(1- emotion::percent, 1/(1000.0 / (*time_seed) -1));
	delta = emotion * num;
	this->mode += emotion - delta;
	this->emotion = delta;
	
	return emotion;
}
void Emotion::process() {
	std::cout << "gg";
}

void  Emotion::show_emotion() {
	printf("emotion : (%lf, %lf) \n", this->emotion.x, this->emotion.y);
}
void  Emotion::show_mode() {
	printf("mode : (%lf, %lf) \n", this->mode.x, this->mode.y);
}

// ** 생성자 ** //
Emotion::Emotion() {
	this->mode = cv::Point2d(0, 0);
	this->emotion = cv::Point2d(100, 100);
}
Emotion::Emotion(int* _time_seed) :Emotion() {
	this->time_seed = _time_seed; ///단위 ms
}
///파일에서 읽어오는 형태로 변경예정, txt에서 초기 설정값을 읽어오는 형태
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
