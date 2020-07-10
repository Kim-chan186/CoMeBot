#pragma once
#ifndef VREP_GUI_H
#define VREP_GUI_H

#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include "Data_Format.h"

using namespace cv;
using namespace std;

namespace GUI {
	Mat *frame = nullptr;
	Mat *img_vrep = nullptr;
	Mat *img_event = nullptr;
	Mat Micon = imread("icon.png", IMREAD_COLOR);
	Mat *icon = &Micon;

	/* Str Event Control Variables */
	//					"기본", "행복", "슬픔", "화남", "지루" , "신남"
	string O_LED[6] = { "Basic", "Happy", "Sad", "Angry", "Bore", "Excite" }; //
	string bodyPos[5] = { "Shake", "Wing", "Down", "Slow", "None" }; //
	//				  "앞뒤", "날개짓", "내림", "뒤로"
	string T_AIL[5] = { "Fast", "Slow", "Down", "Up", "None" }; //
	string txt[8] = { "  ", "Thank", "praise", "dissa", "sad", "bored", "Sorry", "Bad" };

	string HUNGER[5] = { "배부름", "포만감", "보통", "배고픔", "굶주림" };
	string FATIGUE[5] = { "활발", "생기", "보통", "힘듬", "아픔" };
	string TOUCH[4] = { "없음", "머리", "지느러미","꼬리"};
	string MOVENT[2] = { "없음", "충돌" };
	string FACE[2] = { "없음", "보임" };

	string str_frame[8] = {
		"Tuch", "cam", //touch
		"food", "zZZ",
		"good", "bad", //칭찬 혼내기
		"hung", "Heal"
	};

	// "표정", "Wing", "Tail", "Text"
	string* str_event[4] = { O_LED, bodyPos, T_AIL, txt };
	//string* str_state[9] = { O_LED, bodyPos, T_AIL, HUNGER, FATIGUE, TOUCH, MOVENT };

	int* state[9] = {};

	// ping, human,			bab, zzZ,		good, bad
	Point fshift[6] = {
		Point(-28, 6), Point(-24, 7),
		Point(-28, 6), Point(-20, 6),
		Point(-27, 6), Point(-20, 7)
	};


	Point fpoint[6] = {
		Point(85, 100), Point(215, 100),
		Point(85, 490), Point(215, 490),
		Point(85, 620), Point(215, 620)
	};


	Scalar fsclar[6] = {
		Scalar::all(0), Scalar(255, 0, 0),
		Scalar(0, 152, 243), Scalar(240, 32, 160),
		Scalar(0, 255, 0), Scalar(0, 0, 255)
	};


	int npts = { 4 };

	vector<Point> vertex1 = {
		Point(50, 180), Point(50, 420),
		Point(120, 420), Point(120, 180)
	};

	vector<Point> vertex2 = {
		Point(180, 180), Point(180, 420),
		Point(250, 420), Point(250, 180)
	};

	vector<Point> vertex3 = {
		Point(50, 250), Point(50, 420),
		Point(120, 420), Point(120, 250)
	};

	vector<Point> vertex4 = {
		Point(180, 250), Point(180, 420),
		Point(250, 420), Point(250, 250)
	};



	Point epoint[4] = {
		Point(100, 75), Point(225, 75),
		Point(350, 75), Point(500, 75)
	};

	Point eshift[4] = {
		Point(-35, 8), Point(-27, 8),
		Point(-33, 8), Point(-30, 8)
	};


	Rect ping(522 - 40, 359 - 40, 80, 80);
	Rect bab(521 - 40, 481 - 40, 80, 80);
	Rect zzZ(198 - 40, 240 - 40, 80, 80);
	Rect human(520 - 40, 241 - 40, 80, 80);
	Rect good(418 - 40, 480 - 40, 80, 80);
	Rect bad(523 - 40, 115 - 40, 80, 80);

	Rect ricon[6] = { ping, human, bab, zzZ, good, bad };

	// 머리 날개 꼬리
	Rect rtouch[3] = { Rect(325, 325, 65, 65), Rect(108, 383, 100, 50), Rect(100, 250, 80, 50) };


	/*///////////////////////////////////////////////////////////////////////////*/
	int act[4] = {-1,-1,-1,-1};

	// 표정, Wing, Tail, Txt 상태 Rectangle 표시
	void action(int a[4]) {
		for (int i = 0; i < 4; i++) {
			if (act[i] != a[i]) {
					act[i] = a[i];
					//string text = str_event[i][act[i];
					// Rectangle 그리기
					rectangle(*img_event, epoint[i]-Point(50, 40), epoint[i] + Point(50, 40), Scalar(255, 255, 255), -1);
					//putText(*img_event, str_event[i][i], Point(50, 50), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0));
					
					putText(*img_event, str_event[i][act[i]], epoint[i] + eshift[i],
						FONT_HERSHEY_COMPLEX,
						0.7,
						Scalar(0, 0, 0),
						2, //굵기
						8
					);
					
			}
		}
	}


	// 배고픔, 체력 게이지
	unsigned int bar1 = 75, bar2 = 75;

	// 이거 현제 퍼센트 계산 하도록 수정
	// 강제 값 변경이 없을때 flag = 0
	void rectbar(double per1, double per2, bool flag = 0) {
		if (flag) {
			if (bar1 != 0)
				bar1--;

			if (bar2 != 0)
				bar2--;

			per1 = 1 - bar1 / 100.0;
			per2 = 1 - bar2 / 100.0;
		}
		else {
			bar1 += per1;
			bar2 += per2;

			if (bar1 > 100)
				bar1 = 100;
			if (bar2 > 100)
				bar2 = 100;

			per1 = 1 - per1 / 100;
			per2 = 1 - per2 / 100;
		}
		// 게이지 표현
		rectangle(*frame, vertex1[0], vertex1[2], Scalar(0, 0, 0), -1);
		rectangle(*frame, vertex2[0], vertex2[2], Scalar(0, 0, 0), -1);

		rectangle(*frame, vertex1[0] + Point(0, 240 * per1), vertex1[2], Scalar(0, 152, 243), -1);
		rectangle(*frame, vertex2[0] + Point(0, 240 * per2), vertex2[2], Scalar(240, 32, 160), -1);

	} // end rectbar

	void box(int i, bool flag = 1) {
		Rect r(fpoint[i] - Point(40, 40), Size(80, 80));
		(*frame)(r) = (*icon)(ricon[i]) - Scalar::all(50 * (!flag));

		//rectangle(*frame, r, fsclar[i], 3);
		//circle(*frame, fpoint[i], 45, fsclar[i], 7);

		putText(*frame, str_frame[i], fpoint[i] + fshift[i],
			FONT_HERSHEY_COMPLEX,
			0.7,
			fsclar[i],
			2, //굵기
			8
		);
	}

	void GazeSync(double per1, double per2) {
		if (per1 > 100)
			per1 = 100;
		if (per2 > 100)
			per2 = 100;
		per1 = 1 - per1 / 100;
		per2 = 1 - per2 / 100;
		// 게이지 표현
		rectangle(*frame, vertex1[0], vertex1[2], Scalar(0, 0, 0), -1);
		rectangle(*frame, vertex2[0], vertex2[2], Scalar(0, 0, 0), -1);

		rectangle(*frame, vertex1[0] + Point(0, 240 * per1), vertex1[2], Scalar(0, 152, 243), -1);
		rectangle(*frame, vertex2[0] + Point(0, 240 * per2), vertex2[2], Scalar(240, 32, 160), -1);
	}




	/*///////////////////////////////////////////////////////////////////////////*/
	bool mflag[6] = { 0, 0, 0, 0, 0, 0 };

	//				  X, X, O, O, O, O
	int flag_buf[5] = { 0, 0, 0, 0, 0 }; // 통신용

	// 밥, 잠, good, bad 순
	int* get_flag() {
		int _flag_buf[5] = {};
		for (int i = 0; i < 5; i++) {
			_flag_buf[i] = flag_buf[i];
			flag_buf[i] = 0;
		}
		//rectbar(0, 0, true);

		return _flag_buf;
	}


	// NULL  CAM
	void cam(bool b0, bool b1, bool flag = 1) {
		if (flag) {
			mflag[0] = b0;

			mflag[1] = b1;

			box(0, mflag[0]);
			box(1, mflag[1]);
		}
	}
	
	// Mouse Event가 들어오면 Callback 일어남
	void GUI_Event(int event, int x, int y, int flags, void* userdata) {
		//push
		if (event == EVENT_LBUTTONDOWN)
		{
			for (int i = 2; i < 6; i++) {

				if (Rect(fpoint[i] - Point(40, 40), Size(80, 80)).contains(Point(x - 600, y))) {
					box(i, 0);
					mflag[i] = true;
					return;
				}
			}

			//togle
			for (int i = 0; i < 2; i++) {

				if (Rect(fpoint[i] - Point(40, 40), Size(80, 80)).contains(Point(x - 600, y))) {
					mflag[i] = !mflag[i];
					box(i, mflag[i]);
					return;
				}
			}
			//cout << "좌표 = (" << x << ", " << y << ")" << endl;
			// touch
			for (int i = 0; i < 3; i++) {

				if (rtouch[i].contains(Point(x, y))) {

					flag_buf[4] = i + 1;
					cout << TOUCH[i + 1] << endl;

					return;
				}
			}
		}

		else if (event == EVENT_LBUTTONUP) {
			for (int i = 2; i < 6; i++) {
				if (mflag[i]) {
					box(i, 1);
					mflag[i] = false;
					flag_buf[i - 2] = true;

					//표시용
					if (i == 2) {
						rectbar(50, 0);
						Hungry_Para += 50;
						if (Hungry_Para > 100)
							Hungry_Para = 100;
					}
					else if (i == 3) {
						rectbar(0, 50);
						Tired_Para += 50;
						if (Tired_Para > 100)
							Tired_Para = 100;
					}
					return;
				}			
			}
		}

		else if (event == EVENT_RBUTTONDOWN) {
			//std::cout << "Point = (" << x << ", " << y << ")," << endl;
			std::cout << "Rect (" << x << " - 40, " << y << " - 40, 80, 80);" << endl;
		}
	}

	

	void init() {
		namedWindow("GUI");
		setMouseCallback("GUI", GUI_Event, nullptr);


		if (img_vrep == nullptr) {
			img_vrep = new Mat(Size(600, 600), CV_8UC3, Scalar::all(10));
		}else
			resize(*img_vrep, *img_vrep, Size(600, 600));

		frame = new Mat(Size(300, 750), CV_8UC3, Scalar::all(250)); 
		img_event = new Mat(Size(600, 150), CV_8UC3, Scalar::all(100));
		resize(*icon, *icon, Size(600, 600));
		rectbar(0, 0);


		// 테스트용 격자무늬
		/*Point zx(8, 0), zy(0, 10), sx(65, 0), sy(0, 85), x(108, 0), y(0, 123), ex(600, 0), ey(0, 600);

		// 600, 600    
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 7; j++) {
				line(*img_vrep, sx + x * i + zx * j, sx + x * i + ey + zx * j, Scalar::all(70));
				line(*img_vrep, sy + y * i + zy * j, sy + y * i + ex+ zy * j, Scalar::all(70));
			}
		}*/

		// 300, 750
		for (int i = 0; i < 6; i++) {
			Rect r(fpoint[i] - Point(40, 40), Size(80, 80));
			rectangle(*frame, r, fsclar[i], 3);

			box(i);
		}

		// 600, 150
		action(act);


	} // init() end

	simxFloat wantpos[3] = { 0, 0, 1.5907 };
	void goingposition() {
		simxCallScriptFunction(clientID, "camera", sim_scripttype_childscript, "gotoposition", 1, &come_objHandle[6], 3, wantpos, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
	}

	//void show() {
	//	Mat show;
	//	resize(*img_vrep, *img_vrep, Size(600, 600));
	//	vconcat(*img_vrep, *img_event, show);
	//	hconcat(show, *frame, show);		
	//	imshow("GUI", show); 
	//}


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
		}
		//back space bar
		else if (key == 8) {
		}
		//ESC
		else if (key == 27) {
			//console  뒤로 가는 거로 변경!
			cout << "\n  ** ESC로 프로그램을 종료합니다. \n\n";
			quick_exit(EXIT_SUCCESS);
		}
		//방향키 위     2490368
		else if (key == 2490368) {
		}
		//방향키 아래   2621440
		else if (key == 2621440) {
		}
		//방향키 오른쪽  2555904
		else if (key == 2555904) {
		}
		//방향키 왼쪽   2424832
		else if (key == 2424832) {
		}
		else if (key == 49) {//1
			Stt_Data = 1;
		}
		else if (key == 50) {//2
			Stt_Data = 2;
		}
		else if (key == 51) {//3
			Stt_Data = 3;
		}
		else if (key == 52) {//4
			Stt_Data = 4;
		}
		else if (key == 53) {//5
			Stt_Data = 5;
		}
		else if (key == 54) {//6
			Stt_Data = 6;
			
		}
		else if (key == 55) {//7
			Stt_Data = 7;
			
		}
		else if (key == 56) {//8
							 //goingposition();
			windo_change = ON;
		}
		else if (key == 57) {//9
							 //goingposition();
			windo_change = OFF;
		}
		//F1
		else if (key == 7340032) {
			Force_Sensor = ON;
			flag_FORCE = 1;
			printf("%d\n", Force_Sensor);
		}
		//F2
		else if (key == 7405568) {
			goingposition();
			Lift_Sensor = ON;
			flag_Lift = 1;
			printf("%d\n", Lift_Sensor);
		}
		//echo
		else if (key > 255) {
			cout << key << endl;
		}
		else {
		}

		return key;
	}//end waitKeySuper

	void show(int ms = 1) {
		Mat show;

		resize(*img_vrep, *img_vrep, Size(600, 600));

		/*rectangle(*img_vrep, Rect(110, 300, 70, 45), Scalar::all(0), 2);
		rectangle(*img_vrep, Rect(325, 325, 50, 50), Scalar::all(0), 2);
		rectangle(*img_vrep, Rect(150, 410, 100, 30), Scalar::all(0), 2);*/

		vconcat(*img_vrep, *img_event, show);

		hconcat(show, *frame, show);


		imshow("GUI", show);

		GUI::waitKeySuper(ms);
	}
}

#endif /// !VREP_GUI_H