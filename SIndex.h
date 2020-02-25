#pragma once

#ifndef SINDEX_H
#define SINDEX_H

#include <opencv2/core.hpp>

//string to int Map, const_Data
class SIndex
{
private:
	std::string* Str;
	int num;
public:
	SIndex();
	SIndex(std::string* _str, int _num);
	int search(std::string);
	std::string search(int);
	void print_all();
};

SIndex::SIndex() {
	printf("\n ** String to int Map error, �ʱ�ȭ�Ͽ� ����� �ֽñ� �ٶ��ϴ�.\n");
}

SIndex::SIndex(std::string* _str, int _num) {
	this->Str = _str;
	this->num = _num;
}

int SIndex::search(std::string _str) //find string in index(map)
{
	for (int i = 0; i < this->num; i++) {
		if (this->Str[i] == _str)
			return i;
	}
	return -1;	//is not find
}// End search
std::string SIndex::search(int _num) //find index(map) in strings
{
	return Str[_num];	//is not find
}// End search



void SIndex::print_all() {
	for (int i = 0; i < this->num; i++) {
		printf_s("%5s, %2d\n", Str[i].c_str(), i);
	}
}// End print_all


#endif // !EMOTION_H


/*
//TEST

std:string str[5] = { "�ϳ�", "��", "��", "��", "�ټ�" };
	SIndex index(str, 5);
	my_index.print_all();

	cout << "search of \"�ϳ�\": " << my_index.search("�ϳ�") << endl;
	cout << "search of \"4\"   : " << my_index.search(4) << endl;

*/