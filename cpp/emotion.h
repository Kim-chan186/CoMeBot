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
	//������ �Ҹ���
	Emotion();
	Emotion(cv::Point emotion);
	Emotion(cv::Point emotion, cv::Point mode);
	Emotion(int* _time_seed);
	Emotion(string* _str, int _str_num, int* _time_seed);
	Emotion(cv::Point mode, cv::Point emotion, int* _time_seed);
	~Emotion();


	//��� �Լ�
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


/// 1000 / (*time_seed) 1�� ���� ������ �ݺ�Ƚ��
/// percent 1�ʴ� ���ϴ� �̵���(�ۼ�Ʈ)
///�������� ���� �α׷� �ٻ��Ͽ� ����� ��
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


// ** ������ ** //
Emotion::Emotion(cv::Point emotion, cv::Point mode, int* _time_seed) {
	this->mode = mode;
	this->emotion = emotion;
	this->time_seed = _time_seed; ///���� ms
}


///_time_seed���� �ڵ� ���� �ʿ�
Emotion::Emotion(cv::Point emotion, cv::Point mode){
	this->mode = mode;
	this->emotion = emotion;
}


Emotion::Emotion(cv::Point mode)
	: Emotion(mode, cv::Point(0, 0)) {}


Emotion::Emotion(){}



Emotion::Emotion(int* _time_seed) : Emotion() {
	this->time_seed = _time_seed; ///���� ms
}


///���Ͽ��� �о���� ���·� ���濹��, txt���� �ʱ� �������� �о���� ����
///�̿ϼ�
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