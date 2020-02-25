#pragma once
#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "service.h"
#include "cam.h"

namespace chan {
	using namespace cv;

	// window랑 Console을 이용하는 것을 general하게 만들어야 한다.
	// 열수 있는 캠과 그렇지 못하는 캠을 구분하여 만들어야 한다.
	bool key_event(cv::VideoCapture& cam) {
		int key = cv::waitKeyEx(1);
		
		//무입력
		if (key == -1) {
			return 1;
		}
		else if (key == 109) {
			string str;
			cout << "<< Commend \n";
			cout << "cam : change cam \n";
			std::getline(cin, str);

			if (str == "cam") {
				cout << "put the Cam Number! \n";
				std::getline(cin, str);
				if (stoi(str) > -1) {
					cam = cv::VideoCapture(stoi(str));
				}
				else {
					cout << str << "는 불가능한 숫자입니다. \n";
				}
			}
		}

		//ESC
		else if (key == 27) {
			cout << "\n  ** ESC로 프로그램을 종료합니다. \n\n";
			return 0;
		}

		//방향키 위   2490368
		
		else if (key == 2490368) {
			cam::cam_set_zoom(1); //++
		}
		//방향키 아래   2621440
		else if (key == 2621440) {
			cam::cam_set_zoom(0); //--
		}

		else {
			cout << key << endl;
		}
		return 1;
	}
}//End_chan

#endif /// !EMOTION_H