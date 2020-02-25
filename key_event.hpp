#pragma once
#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "service.h"
#include "cam.h"

namespace chan {
	using namespace cv;

	// window�� Console�� �̿��ϴ� ���� general�ϰ� ������ �Ѵ�.
	// ���� �ִ� ķ�� �׷��� ���ϴ� ķ�� �����Ͽ� ������ �Ѵ�.
	bool key_event(cv::VideoCapture& cam) {
		int key = cv::waitKeyEx(1);
		
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
			return 0;
		}

		//����Ű ��   2490368
		
		else if (key == 2490368) {
			cam::cam_set_zoom(1); //++
		}
		//����Ű �Ʒ�   2621440
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