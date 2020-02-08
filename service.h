#pragma once
//편의를 위한 기능들을 정의한 헤더
#ifndef EMOTION_H
#define EMOTION_H
#endif // !EMOTION_H

#include <chrono>
#include <iostream>

namespace chan {
	//** error 출력
	//* sys 출력

	double dTime;
	int CH_count = -1;
	std::chrono::system_clock::time_point tpStart, tpEnd;
	
	/*반복문(while)자리에 사용하며 자동으로 시간을 측정해주는 함수
	몇번 루프를 수행한뒤 시간을 측정할 것인지를 입력으로 받음
	-추후 윈도우 상에 글자로 띄우는 것으로 수정 예정
	-초기화 함수를 만들 예정
	*/
	bool Check_Time(int Loop = 1000) {
		CH_count++;

		if(CH_count == 0) {
			tpStart = std::chrono::system_clock::now(); //시간 측정 시작 
			printf("tlwkr");
			return 1;
		}
		else if (CH_count < Loop){
			return 1;
		}
		else if(CH_count == Loop) {
			tpEnd = std::chrono::system_clock::now(); //시간 측정 끝
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() /1e6 / Loop;
			std::cout << "Loop Time: " << dTime << "ms" << std::endl;
			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //시간 측정 시작
			CH_count = 0;
			return 1;
		}
		else {
			printf("\n** error Check_Time!!\n");
			return 1;
		}
	}//end Check_Time
	
}

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
