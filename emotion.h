#pragma once
#ifndef EMOTION_H
#define EMOTION_H
//���� ������ ���õ� �������
//mode�� ���� ������ 2�������� ǥ���� ��
//emotion�� ������ ���ؼ� ���� ������ ������

///emotion���� ms������ �ݺ��Ǵ� �����ȿ���
///�����ð���(1s) ������(%) mode�� ��ȭ�Ҷ� �ʿ��� �Լ� emotion2mode() ���� ������

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
	/// 1000 / (*time_seed) 1�� ���� ������ �ݺ�Ƚ��
	/// percent 1�ʴ� ���ϴ� �̵���(�ۼ�Ʈ)
	///�������� ���� �α׷� �ٻ��Ͽ� ����� ��
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

// ** ������ ** //
Emotion::Emotion() {
	this->mode = cv::Point2d(0, 0);
	this->emotion = cv::Point2d(100, 100);
}
Emotion::Emotion(int* _time_seed) :Emotion() {
	this->time_seed = _time_seed; ///���� ms
}
///���Ͽ��� �о���� ���·� ���濹��, txt���� �ʱ� �������� �о���� ����
Emotion::Emotion(string* _str, int _str_num, int* _time_seed)
	: Emotion(_time_seed)
{
	this->mode_index = SIndex(_str, _str_num);
}
// ** �Ҹ��� ** //
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
