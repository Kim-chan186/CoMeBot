#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include "service.h"
#include "Debug.h"
#include "cam.h"
#include "stack.h"
#include "SIndex.h"
#include "node.h"


namespace chan {

	class My_Console {

		str_stack con_msg;

		c_stack con_buf;

		SIndex indexs[10];

	public:

		My_Console() {
			string str_com[10][10] = { 
				{"cam", "key"},	//0
				{},//1
				{},//2
				{},//3
				{},//4
				{},//5
				{},//6
				{},//7
				{},//8
				{} //9
			};
			

			for (int i = 0; i < 10; i++) {
				indexs[i].set(str_com[i], 10);
			}

			this->set_commend();
		}



		void set_commend() {
			con_msg << "<< Commend";
			con_msg << "       cam : change cam";
			con_msg << "       key : show key commend";
		}



		void set_key() {
			con_msg << "<< key : show ";
			con_msg << "      F1 : set <-, -> exposure";
			con_msg << "      F2 : set <-, -> brightness (default)";
			con_msg << "      up : zoom ++";
			con_msg << "    down : zoom --";
		}



		void set_cam() {

			//존재하지 않을 예외
			/*if (cam::_cam == nullptr) {
				con_msg << "  ** sys : open cam 0 !!";
				cam::_cam = &cv::VideoCapture(0);
			}*/
			
			if (cam::_cam.isOpened()) {
				con_msg << "<< cam : change cam";
			}
			else {
				con_msg << "<< cam : change cam (is not Opened !!)";
			}

			con_msg << "<<change  : change cam";
		}



		void find_event() {
			//수정중
			int ievent = indexs[0].search(con_buf.show());

			con_buf.clear();
			if (ievent == -1) {
				return;
			}
			//"cam"
			else if (ievent == 0) {
				this->con_msg.clear();
				this->set_cam();
			}
			//"key"
			else if (ievent == 1) {
				this->con_msg.clear();
				this->set_key();
			}

		}//end find_event()



		void pop()
		{
			this->con_buf.pop();
		}



		void show(){
			int num = 0;
			string* pstr = con_msg.show(num);

			cv::Mat img_console1 = Mat(cv::Size(300, 200), CV_8UC3, Scalar::all(1));
			cv::Mat img_console2 = Mat(cv::Size(300, 30), CV_8UC3, Scalar::all(1));

			for (int i = 0; i < num; i++) {
				putText(img_console1, pstr[i],
					Point(10, img_console1.rows / 9 * i + 15),
					FONT_HERSHEY_PLAIN,
					1,							//fontScale
					cv::Scalar(255, 0, 255),	//color
					1.5);
			}

			putText(img_console2,con_buf.show(),
				Point(10, img_console2.rows - 10),
				FONT_HERSHEY_PLAIN,
				1,							//fontScale
				cv::Scalar(255, 255, 0),	//color
				1.5);

			cv::vconcat(img_console1, img_console2, img_console2);
			cv::imshow("Con", img_console2);
		}



		//1개만 입력 받음
		void operator<< (char str)
		{
			this->con_buf.push(str);
		}
	};
	
}
#endif /// !EMOTION_H