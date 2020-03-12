#pragma once

#ifndef SINDEX_H
#define SINDEX_H

#include <opencv2/core.hpp>

//string to int Map, const_Data
class SIndex
{
private:
	std::string Str[10];

	unsigned int num;

public:

	SIndex();

	SIndex(std::string* _str, unsigned int _num);

	void set(std::string* _str, unsigned int _num);

	int search(std::string);
	std::string search(int);
	void print_all();
};

SIndex::SIndex() {
}


SIndex::SIndex(std::string* _str, unsigned int _num) {
	this->set(_str, _num);
}

void SIndex::set(std::string* _str, unsigned int _num) {
	for (int i = 0; i < _num; i++) {
		this->Str[i] = _str[i];
	}
	this->num = _num;

	print_all();
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

std:string str[5] = { "하나", "둘", "셋", "넷", "다섯" };
	SIndex index(str, 5);
	my_index.print_all();

	cout << "search of \"하나\": " << my_index.search("하나") << endl;
	cout << "search of \"4\"   : " << my_index.search(4) << endl;

*/