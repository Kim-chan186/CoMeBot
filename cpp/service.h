#pragma once
<<<<<<< HEAD:cpp/service.h
//편의를 위한 기능들을 정의한 헤더
=======
//�렪�쓽瑜� �쐞�븳 湲곕뒫�뱾�쓣 �젙�쓽�븳 �뿤�뜑
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h

#include <chrono>
#include <iostream>

namespace chan {
<<<<<<< HEAD:cpp/service.h
	//** error 출력
	//* sys 출력

	double dTime; // 1loop 당 걸리는 시간
=======
	//** error 異쒕젰
	//* sys 異쒕젰

	double dTime; // 1loop �떦 嫄몃━�뒗 �떆媛�
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
	
	int CH_count = -1;
	std::chrono::system_clock::time_point tpStart, tpEnd;
	
	/*
	
<<<<<<< HEAD:cpp/service.h
	# 시작측정
	for문이나 while문 안에 넣어서 자동으로 시간주기를 측정해주는 함수

	- Check_Time(int Loop = 1000)

	- for (int i = 0; (i < 1000)&chan::Check_Time(); i++){}
	- while(chan::Check_Time()){}

	- 총 시간이 아니라 제어주기 만큼만(1루프)

	몇번 루프를 수행한뒤 시간을 측정할 것인지를 입력으로 받음
	-추후 윈도우 상에 글자로 띄우는 것으로 수정 예정
	-초기화 함수를 만들 예정
=======
	# �떆�옉痢≪젙
	for臾몄씠�굹 while臾� �븞�뿉 �꽔�뼱�꽌 �옄�룞�쑝濡� �떆媛꾩＜湲곕�� 痢≪젙�빐二쇰뒗 �븿�닔
	- Check_Time(int Loop = 1000)
	- for (int i = 0; (i < 1000)&chan::Check_Time(); i++){}
	- while(chan::Check_Time()){}
	- 珥� �떆媛꾩씠 �븘�땲�씪 �젣�뼱二쇨린 留뚰겮留�(1猷⑦봽)
	紐뉖쾲 猷⑦봽瑜� �닔�뻾�븳�뮘 �떆媛꾩쓣 痢≪젙�븷 寃껋씤吏�瑜� �엯�젰�쑝濡� 諛쏆쓬
	-異뷀썑 �쐢�룄�슦 �긽�뿉 湲��옄濡� �쓣�슦�뒗 寃껋쑝濡� �닔�젙 �삁�젙
	-珥덇린�솕 �븿�닔瑜� 留뚮뱾 �삁�젙
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
	*/

	bool Check_Time(int Loop = 1000) {
		CH_count++;

		if(CH_count == 0) {
<<<<<<< HEAD:cpp/service.h
			tpStart = std::chrono::system_clock::now(); //시간 측정 시작 
			printf("시작");
=======
			tpStart = std::chrono::system_clock::now(); //�떆媛� 痢≪젙 �떆�옉 
			printf("�떆�옉");
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
			return 1;
		}
		else if (CH_count < Loop){
			return 1;
		}
		else if(CH_count == Loop) {
<<<<<<< HEAD:cpp/service.h
			tpEnd = std::chrono::system_clock::now(); //시간 측정 끝
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() /1e6 / Loop;
			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //시간 측정 시작
=======
			tpEnd = std::chrono::system_clock::now(); //�떆媛� 痢≪젙 �걹
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() /1e6 / Loop;
			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //�떆媛� 痢≪젙 �떆�옉
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
			cout << "!";//늦추기 위해 디버깅 모드로 빌드
			for (int j = 0; j < 1000; j++) {
				for (int k = 0; k < 1000; k++) {
					//시간소모
=======
			cout << "!";//�뒭異붽린 �쐞�빐 �뵒踰꾧퉭 紐⑤뱶濡� 鍮뚮뱶
			for (int j = 0; j < 1000; j++) {
				for (int k = 0; k < 1000; k++) {
					//�떆媛꾩냼紐�
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
					int a = 100;
					int b = 100;
					int c = a = a * b;
					c = b = a * c - b;
<<<<<<< HEAD:cpp/service.h
					//시간소모_end
=======
					//�떆媛꾩냼紐�_end
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
		}}}*/

	}//end Check_Time
	

	int* int_track_vaule = nullptr;

	double* double_track_vaule = nullptr;

<<<<<<< HEAD:cpp/service.h
	//빠른 트렉바
	//이후 클래스로 변경
=======
	//鍮좊Ⅸ �듃�젆諛�
	//�씠�썑 �겢�옒�뒪濡� 蹂�寃�
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
