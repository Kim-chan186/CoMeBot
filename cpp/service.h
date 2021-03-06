#pragma once
//편의를 위한 기능들을 정의한 헤더
#ifndef SERVICE_H
#define SERVICE_H

#include <chrono>
#include <iostream>

namespace chan {
	//** error 출력
	//* sys 출력

	double dTime; // 1loop 당 걸리는 시간

	int CH_count = - 1;
	std::chrono::system_clock::time_point tpStart, tpEnd;

	/*

	# 시작측정
	for문이나 while문 안에 넣어서 자동으로 시간주기를 측정해주는 함수
	- Check_Time(int Loop = 1000)
	- for (int i = 0; (i < 1000)&chan::Check_Time(); i++){}
	- while(chan::Check_Time()){}
	- 총 시간이 아니라 제어주기 만큼만(1루프)
	몇번 루프를 수행한뒤 시간을 측정할 것인지를 입력으로 받음
	-추후 윈도우 상에 글자로 띄우는 것으로 수정 예정
	-초기화 함수를 만들 예정
	*/
	bool Check_Time(int Loop = 5000) {
		CH_count++;

		if (CH_count == 0) {
			//printf("시간측정 시작\n");
			tpStart = std::chrono::system_clock::now(); //시간 측정 시작 
			return 1;
		}
		else if (CH_count < Loop) {
			return 1;
		}
		else if (CH_count == Loop) {
			tpEnd = std::chrono::system_clock::now(); //시간 측정 끝
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() / 1e6 / Loop;

			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //시간 측정 시작
			CH_count = -1;
			return 1;
		}
		else {
			printf("\n** error Check_Time!!\n");
			return 1;
		}
	}//end Check_Time

}

#endif // !EMOTION_H
/*
//TEST
	for (int i = 0; (i < 1000)&chan::Check_Time(); i++) {
		cout << "!";//늦추기 위해 디버깅 모드로 빌드
		for (int j = 0; j < 1000; j++) {
			for (int k = 0; k < 1000; k++) {
				//시간소모
				int a = 100;
				int b = 100;
				int c = a = a * b;
				c = b = a * c - b;
				//시간소모_end
			}
		}
	}
*/