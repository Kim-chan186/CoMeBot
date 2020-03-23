#pragma once
<<<<<<< HEAD:cpp/service.h
//���Ǹ� ���� ��ɵ��� ������ ���
=======
//편의를 위한 기능들을 정의한 헤더
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h

#include <chrono>
#include <iostream>

namespace chan {
<<<<<<< HEAD:cpp/service.h
	//** error ���
	//* sys ���

	double dTime; // 1loop �� �ɸ��� �ð�
=======
	//** error 출력
	//* sys 출력

	double dTime; // 1loop 당 걸리는 시간
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
	
	int CH_count = -1;
	std::chrono::system_clock::time_point tpStart, tpEnd;
	
	/*
	
<<<<<<< HEAD:cpp/service.h
	# ��������
	for���̳� while�� �ȿ� �־ �ڵ����� �ð��ֱ⸦ �������ִ� �Լ�

	- Check_Time(int Loop = 1000)

	- for (int i = 0; (i < 1000)&chan::Check_Time(); i++){}
	- while(chan::Check_Time()){}

	- �� �ð��� �ƴ϶� �����ֱ� ��ŭ��(1����)

	��� ������ �����ѵ� �ð��� ������ �������� �Է����� ����
	-���� ������ �� ���ڷ� ���� ������ ���� ����
	-�ʱ�ȭ �Լ��� ���� ����
=======
	# 시작측정
	for문이나 while문 안에 넣어서 자동으로 시간주기를 측정해주는 함수
	- Check_Time(int Loop = 1000)
	- for (int i = 0; (i < 1000)&chan::Check_Time(); i++){}
	- while(chan::Check_Time()){}
	- 총 시간이 아니라 제어주기 만큼만(1루프)
	몇번 루프를 수행한뒤 시간을 측정할 것인지를 입력으로 받음
	-추후 윈도우 상에 글자로 띄우는 것으로 수정 예정
	-초기화 함수를 만들 예정
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
	*/

	bool Check_Time(int Loop = 1000) {
		CH_count++;

		if(CH_count == 0) {
<<<<<<< HEAD:cpp/service.h
			tpStart = std::chrono::system_clock::now(); //�ð� ���� ���� 
			printf("����");
=======
			tpStart = std::chrono::system_clock::now(); //시간 측정 시작 
			printf("시작");
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
			return 1;
		}
		else if (CH_count < Loop){
			return 1;
		}
		else if(CH_count == Loop) {
<<<<<<< HEAD:cpp/service.h
			tpEnd = std::chrono::system_clock::now(); //�ð� ���� ��
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() /1e6 / Loop;
			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //�ð� ���� ����
=======
			tpEnd = std::chrono::system_clock::now(); //시간 측정 끝
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() /1e6 / Loop;
			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //시간 측정 시작
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
			CH_count = 0;
			return 1;
		}
		else {
			printf("\n ** error Check_Time!! \n");
			return 1;
		}
		/*//TEST Check_Time
		for (int i = 0; (i < 1000)&chan::Check_Time(); i++) {
<<<<<<< HEAD:cpp/service.h
			cout << "!";//���߱� ���� ����� ���� ����
			for (int j = 0; j < 1000; j++) {
				for (int k = 0; k < 1000; k++) {
					//�ð��Ҹ�
=======
			cout << "!";//늦추기 위해 디버깅 모드로 빌드
			for (int j = 0; j < 1000; j++) {
				for (int k = 0; k < 1000; k++) {
					//시간소모
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
					int a = 100;
					int b = 100;
					int c = a = a * b;
					c = b = a * c - b;
<<<<<<< HEAD:cpp/service.h
					//�ð��Ҹ�_end
=======
					//시간소모_end
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
		}}}*/

	}//end Check_Time
	

	int* int_track_vaule = nullptr;

	double* double_track_vaule = nullptr;

<<<<<<< HEAD:cpp/service.h
	//���� Ʈ����
	//���� Ŭ������ ����
=======
	//빠른 트렉바
	//이후 클래스로 변경
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
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
<<<<<<< HEAD:cpp/service.h


=======
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
