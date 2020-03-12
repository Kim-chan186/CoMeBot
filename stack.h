#pragma once
#ifndef STACK_H
#define STACK_H

#include <xstring>

namespace chan {
	class c_stack {

		unsigned int num = 0;

		char* cstr = nullptr;

	public:

		c_stack() {
			clear();
		}


		~c_stack() {
			delete(this->cstr);
		}


		//10개 이상 입력 무시
		void push(char c) {
			if (num > 50) {
				return;
			}
			this->cstr[num] = c;
			num++;
		}


		char pop() {
			char c = cstr[num];
			cstr[--num] = 0;
			return c;
		}


		void clear() {
			num = 0;
			delete(this->cstr);
			this->cstr = new char[51];
			for (int i = 0; i < 51; i++) {
				cstr[i] = 0;
			}
		}


		string show() {
			if (num == 0) {
				return "";
			}
			string str = cstr;
			return  str;
		}


	};//end c_stack


	class str_stack {										//	<class str_stack>

		unsigned int num = 0;

		string* str = nullptr;

	public:

		str_stack& operator<< (char * str)
		{
			this->push(str);
			return *this;
		}


		str_stack& operator<< (string str)
		{
			this->push(str);
			return *this;
		}


		str_stack() {
			clear();
		}


		str_stack(string* str, int num) : str_stack() {
			for (int i = 0; i < num; i++) {
				*this << str[i];
			}
		}


		~str_stack() {
			delete(this->str);
		}


		//10개 이상 입력 무시
		void push(string _str) {

			if (num > 9) {
				return;
			}
			this->str[num] = _str;
			num++;
		}


		string pop() {
			string c = str[num];
			str[--num] = "";
			return c;
		}


		void clear() {
			num = 0;
			delete [] this->str;
			this->str = new string[10];
			for (int i = 0; i < 10; i++) {
				str[i] = "";
			}
		}


		string* show(int& num) {
			num = this->num;
			return str;
		}


	};//end str_stack
}
#endif /// !EMOTION_H