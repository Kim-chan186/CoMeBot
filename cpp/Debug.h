#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include <opencv2/core.hpp>

#define TTT printf(" ** TEST0 **\n");
#define TTT1 printf(" ** TEST1 **\n");
#define TTT2 printf(" ** TEST2 **\n");
#define TTT3 printf(" ** TEST3 **\n");
#define TTT4 printf(" ** TEST4 **\n");

/*
if (CV_VERSION != "4.1.2")
std::cout << "** OpcnCV Version is not 4.1.2\n"
<< "** OpcnCV Version is " << CV_VERSION << std::endl;
*/
namespace debug {


	int roop = 1;

	int calling = 1;



	void empty(cv::Mat& data) {
		cout << calling++ << ". debug empty : " << data.empty() << endl;
	}



	void empty(cv::Mat& data, cv::Mat& data2) {
		cout << calling++ << ". debug empty : " << data.empty() << data2.empty() << endl;
	}



	void empty(cv::Mat& data, cv::Mat& data2, cv::Mat& data3) {
		cout << calling++ << ". debug empty : " << data.empty() << data2.empty() << data3.empty() << endl;
	}



	void empty(cv::Mat& data, cv::Mat& data2, std::vector<cv::DMatch> data3) {
		cout << calling++ << ". debug empty : " << data.empty() << data2.empty() << data3.empty() << endl;
	}



	//������ ���̱�
	void show(cv::Mat& data, cv::Mat& data2) {
		cv::Mat show;
		//cv::moveWindow("debug", 0, 0);
		//cv::hconcat(data, data2, show);
		imshow("debug:show1", data);
		imshow("debug:show2", data2);
	}



	void show_wait(cv::Mat& data, cv::Mat& data2) {
		cv::Mat show;
		//cv::moveWindow("debug", 0, 0);
		//cv::hconcat(data, data2, show);
		imshow("debug:show_wait1", data);
		imshow("debug:show_wait2", data2);
		waitKey(0);
	}



	//�Ʒ��� ���̱�
	//�ݺ������� �Ҳ�...
	void show_compare(cv::Mat& data, cv::Mat& data2) {
		cv::Mat show;
		//cv::moveWindow("debug", 0, 0);
		//cv::hconcat(data, data2, show);
		cv::vconcat(data, data2, show);
		imshow("debug:show_compare", show);
	}



	void show_compare(cv::Mat& data, cv::Mat& data2, cv::Mat& data3) {
		cv::Mat show;
		cv::vconcat(data, data2, show);
		cv::vconcat(show, data3, show);
		imshow("debug:show_compare", show);
	}



	void show_compare(cv::Mat& data, cv::Mat& data2, cv::Mat& data3, cv::Mat& data4) {
		cv::Mat show;
		cv::vconcat(data, data2, show);
		cv::vconcat(show, data3, show);
		cv::vconcat(show, data4, show);
		imshow("debug:show_compare", show);
	}



	void show_compare(cv::Mat& data, cv::Mat& data2, cv::Mat& data3, cv::Mat& data4, cv::Mat& data5) {
		cv::Mat show;
		cv::vconcat(data, data2, show);
		cv::vconcat(show, data3, show);
		cv::vconcat(show, data4, show);
		cv::vconcat(show, data5, show);
		imshow("debug:show_compare", show);
	}



	//data : �̹��� �迭, show : buffer_img, n : �迭 ũ��
	void show_compare(cv::Mat* data, cv::Mat& show, unsigned int n){
		if (show.empty()) {
			show = data[n];
		}
		else {
			cv::vconcat(data[n], show, show);
		}
		if (n) {
			show_compare(data, show, n - 1);
		}
		else {
			imshow("debug:show_compare", show);
		}
	}



	void show_blending(cv::Mat& data, cv::Mat& data2) {
		cv::Mat show;
		cv::addWeighted(data, 0.5, data2, 0.5, 0, show);
		imshow("debug:show_blending", show);
	}



	string Mat_type[8] = {
		"CV_8U"//   0
		,"CV_8S"//   1
		,"CV_16U"//  2
		,"CV_16S"//  3
		,"CV_32S"//  4
		,"CV_32F"//  5
		,"CV_64F"//  6
		,"CV_16F"//  7
	};



	void check_Mat(cv::Mat& data) {
		cout << "\n >> debug check_Mat\n";
		if (data.empty()) {
			cout << "�� �̹��� �Դϴ�.\n";
			cout << " debug <<\n";
			return;
		}
		
		cout << "cols     : " << data.cols << endl 
			 << "rows     : "<< data.rows << endl;
		cout << "channels : " << data.channels() << endl
			 << "type     : " << Mat_type[data.type()] << endl
			 << "depth    : " << data.depth() << endl;

		cout << " debug check_Mat <<\n";
	}
}

#endif /// !EMOTION_H