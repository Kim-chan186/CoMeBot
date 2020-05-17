#pragma once
#ifndef KEY_EVENT_H
#define KEY_EVENT_H


#include "console.h"

namespace chan {
	using namespace cv;

	My_Console Con;

	//����Ű �¿� (<-, -> ȭ��ǥ)
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
			//console  �ڷ� ���� �ŷ� ����!
			cout << "\n  ** ESC�� ���α׷��� �����մϴ�. \n\n";
			quick_exit(EXIT_SUCCESS);
		}


		//����Ű ��     2490368
		else if (key == 2490368) {
			cam::cam_zoom(1); //++
		}
		//����Ű �Ʒ�   2621440
		else if (key == 2621440) {
			cam::cam_zoom(0); //--
		}
		//����Ű ������  2555904
		else if (key == 2555904) {
			arrow_func(1); //++
		}
		//����Ű ����   2424832
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



	// Window�� Console�� �̿��ϴ� ���� general�ϰ� ������ �Ѵ�.
	// ���� �ִ� ķ�� �׷��� ���ϴ� ķ�� �����Ͽ� ������ �Ѵ�.
	bool key_event(cv::VideoCapture& cam) {
		int key = waitKeySuper();


		//���Է�
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
					cout << str << "�� �Ұ����� �����Դϴ�. \n";
				}
			}
		}
		//ESC
		else if (key == 27) {
			cout << "\n  ** ESC�� ���α׷��� �����մϴ�. \n\n";
			quick_exit(EXIT_SUCCESS);
		}

		//����Ű ��   2490368
		else if (key == 2490368) {
			cam::cam_zoom(1); //++
		}
		//����Ű �Ʒ�   2621440
		else if (key == 2621440) {
			cam::cam_zoom(0); //--
		}
		//����Ű ������  2555904
		else if (key == 2555904) {
			cam::cam_brightness(1); //++
		}
		//����Ű ����   2424832
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