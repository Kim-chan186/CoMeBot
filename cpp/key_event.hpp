#pragma once
#ifndef KEY_EVENT_H
#define KEY_EVENT_H


#include "console.h"

namespace chan {
	using namespace cv;

	My_Console Con;

	//방향키 좌우 (<-, -> 화살표)
	bool (*arrow_func)(bool) = cam::cam_brightness;


	int waitKeySuper(int num = 1) {
		
		int key = waitKeyEx(num);
		if (key == -1) {
			return -1;
		}
	

		// space bar
		if (key == 32) {
		}
		//Enter
		else if (key == 13) {
			Con.find_event();
		}
		//back space bar
		else if (key == 8) {
			Con.pop();
		}
		//ESC
		else if (key == 27) {
			//console  뒤로 가는 거로 변경!
			cout << "\n  ** ESC로 프로그램을 종료합니다. \n\n";
			quick_exit(EXIT_SUCCESS);
		}


		//방향키 위     2490368
		else if (key == 2490368) {
			cam::cam_zoom(1); //++
		}
		//방향키 아래   2621440
		else if (key == 2621440) {
			cam::cam_zoom(0); //--
		}
		//방향키 오른쪽  2555904
		else if (key == 2555904) {
			arrow_func(1); //++
		}
		//방향키 왼쪽   2424832
		else if (key == 2424832) {
			arrow_func(0); //--
		}


		//F1
		else if (key == 7340032){
			std::cout << "cam_Exposure   : " << cam::_cam.get(CAP_PROP_EXPOSURE) << endl;
			arrow_func = cam::cam_exposure;
		}
		//F2
		else if (key == 7405568) {
			std::cout << "cam_Brightness : " << cam::_cam.get(CAP_PROP_BRIGHTNESS) << endl;
			arrow_func = cam::cam_brightness;
		}	
		//echo
		else if (key > 255) {
			cout << key << endl;
		}
		else {
			Con << key;
		}
		Con.show();

		return key;
	}//end waitKeySuper



	// Window랑 Console을 이용하는 것을 general하게 만들어야 한다.
	// 열수 있는 캠과 그렇지 못하는 캠을 구분하여 만들어야 한다.
	bool key_event(cv::VideoCapture& cam) {
		int key = waitKeySuper();


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
			quick_exit(EXIT_SUCCESS);
		}

		//방향키 위   2490368
		else if (key == 2490368) {
			cam::cam_zoom(1); //++
		}
		//방향키 아래   2621440
		else if (key == 2621440) {
			cam::cam_zoom(0); //--
		}
		//방향키 오른쪽  2555904
		else if (key == 2555904) {
			cam::cam_brightness(1); //++
		}
		//방향키 왼쪽   2424832
		else if (key == 2424832) {
			cam::cam_brightness(0); //--
		}

		else {
			cout << key << endl;
		}
		return 1;
	}
}//End_chan

#endif /// !EMOTION_H