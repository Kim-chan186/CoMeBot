#pragma once
//���Ǹ� ���� ��ɵ��� ������ ���
#ifndef EMOTION_H
#define EMOTION_H
#endif // !EMOTION_H

#include <chrono>
#include <iostream>

namespace chan {
	//** error ���
	//* sys ���

	double dTime;
	int CH_count = -1;
	std::chrono::system_clock::time_point tpStart, tpEnd;
	
	/*�ݺ���(while)�ڸ��� ����ϸ� �ڵ����� �ð��� �������ִ� �Լ�
	��� ������ �����ѵ� �ð��� ������ �������� �Է����� ����
	-���� ������ �� ���ڷ� ���� ������ ���� ����
	-�ʱ�ȭ �Լ��� ���� ����
	*/
	bool Check_Time(int Loop = 1000) {
		CH_count++;

		if(CH_count == 0) {
			tpStart = std::chrono::system_clock::now(); //�ð� ���� ���� 
			printf("tlwkr");
			return 1;
		}
		else if (CH_count < Loop){
			return 1;
		}
		else if(CH_count == Loop) {
			tpEnd = std::chrono::system_clock::now(); //�ð� ���� ��
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() /1e6 / Loop;
			std::cout << "Loop Time: " << dTime << "ms" << std::endl;
			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //�ð� ���� ����
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
		cout << "!";//���߱� ���� ����� ���� ����
		for (int j = 0; j < 1000; j++) {
			for (int k = 0; k < 1000; k++) {
				//�ð��Ҹ�
				int a = 100;
				int b = 100;
				int c = a = a * b;
				c = b = a * c - b;
				//�ð��Ҹ�_end
			}
		}
	}

*/