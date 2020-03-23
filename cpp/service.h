#pragma once
//���Ǹ� ���� ��ɵ��� ������ ���

#include <chrono>
#include <iostream>

namespace chan {
	//** error ���
	//* sys ���

	double dTime; // 1loop �� �ɸ��� �ð�
	
	int CH_count = -1;
	std::chrono::system_clock::time_point tpStart, tpEnd;
	
	/*
	
	# ��������
	for���̳� while�� �ȿ� �־ �ڵ����� �ð��ֱ⸦ �������ִ� �Լ�

	- Check_Time(int Loop = 1000)

	- for (int i = 0; (i < 1000)&chan::Check_Time(); i++){}
	- while(chan::Check_Time()){}

	- �� �ð��� �ƴ϶� �����ֱ� ��ŭ��(1����)

	��� ������ �����ѵ� �ð��� ������ �������� �Է����� ����
	-���� ������ �� ���ڷ� ���� ������ ���� ����
	-�ʱ�ȭ �Լ��� ���� ����
	*/

	bool Check_Time(int Loop = 1000) {
		CH_count++;

		if(CH_count == 0) {
			tpStart = std::chrono::system_clock::now(); //�ð� ���� ���� 
			printf("����");
			return 1;
		}
		else if (CH_count < Loop){
			return 1;
		}
		else if(CH_count == Loop) {
			tpEnd = std::chrono::system_clock::now(); //�ð� ���� ��
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() /1e6 / Loop;
			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //�ð� ���� ����
			CH_count = 0;
			return 1;
		}
		else {
			printf("\n ** error Check_Time!! \n");
			return 1;
		}
		/*//TEST Check_Time
		for (int i = 0; (i < 1000)&chan::Check_Time(); i++) {
			cout << "!";//���߱� ���� ����� ���� ����
			for (int j = 0; j < 1000; j++) {
				for (int k = 0; k < 1000; k++) {
					//�ð��Ҹ�
					int a = 100;
					int b = 100;
					int c = a = a * b;
					c = b = a * c - b;
					//�ð��Ҹ�_end
		}}}*/

	}//end Check_Time
	

	int* int_track_vaule = nullptr;

	double* double_track_vaule = nullptr;

	//���� Ʈ����
	//���� Ŭ������ ����
	static void on_trackbar(int _value , void* _userdata) {
		_userdata = (double*)(int*)(int)((*(int*)_value) / 10.0);
	}


	void fast_trackbar(double* _value) {
		cv::namedWindow("track Bar");
		double_track_vaule = _value;
		cv::createTrackbar("value / 10.0", "track Bar", int_track_vaule, 1000, on_trackbar, _value);
		//setTrackbarPos("low threshold", "Canny", 50);
		//00 = getTrackbarPos('low threshold', 'Canny')
	}


}


