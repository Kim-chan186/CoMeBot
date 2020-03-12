

#pragma once
#ifndef MYNODE_H
#define MYNODE_H

#include "SIndex.h"
#include "Debug.h"

namespace chan {


	struct node {

		node* previous = nullptr;

		node* next = nullptr;

		int data = 0;

	};


	class master_node
	{
	private:

		node* root;

		node* head;

		int branchs = 0;


	public:

		void push(int data) {

			this->head = new node{
				this->head,
				0,
				data
			};

			this->head->previous->next = this->head;

			branchs++;
		}


		int pop(){
			int data = this->head->data;
	

			this->head = this->head->previous;

			delete this->head->next;

			this->branchs--;


			return data;
		}


		void operator << (int data)
		{
			this->push(data);
		}


		void operator >> (int& data)
		{
			data = this->pop();
		}


		
		master_node();
		~master_node();
	};


	master_node::master_node()
	{
		this->head = this->root = new node;
		this->branchs = 0;
	}


	master_node::~master_node() {
		for (int i = 0; i < branchs; i++) {
			this->pop();
		}
	}

}
#endif /// !EMOTION_H