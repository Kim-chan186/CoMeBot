#pragma once
#ifndef DATA_TRANSMISSION_H
#define DATA_TRANSMISSION_H

#include "winsock2.h" // include before <windows.h>
#include <windows.h>
#include <conio.h>
#include "VRep_GUI.h"
/*
	Thread Include Files
*/
#include <thread>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include "typeinfo"

#define BUFSIZE 2048

using namespace std;

struct SafeCondVar
{
	int m_iNC;
	mutex m_oMtx;
	condition_variable m_oCondVar;

	SafeCondVar() : m_iNC(0), m_oMtx(), m_oCondVar() {
	}

	void notifyOne() {
		lock_guard<mutex> oL(m_oMtx);
		++m_iNC;
		m_oCondVar.notify_one();
	}
	void wait() {
		unique_lock<mutex> oL(m_oMtx);
		while (true)
		{
			if (0 < m_iNC)
			{
				--m_iNC;
				return;
			}
			m_oCondVar.wait(oL);
		}
	}
};
SafeCondVar    Send_CondVar;
SafeCondVar    Recv_CondVar;

void recv_socket(SOCKET sock);
void tran_socket(SOCKET sock);





/*///////////////////////////////////////////////////////////////////////////*/
void recv_socket(SOCKET sock) {
	char buff[BUFSIZE];
	int ibuff;
	string sbuff;
	string str_data[3];
	int str_cnt = 0;
	//int distribute = 0;

	while (1) {
		ZeroMemory(buff, 1024);
		int bytesReceived = recv(sock, buff, 1024, 0);
		if (bytesReceived > 0) {
			ibuff = atoi(buff); // buff : char[] -> int
			//ibuff = atoi(Mode[ibuff].c_str());
			Mode_Select = ibuff / 10;
			Stt_Data = ibuff % 10;
			//distribute = stoi(Mode[Mode_Select]);
			//Oled_State = distribute / 100;
			//Fin_State = (distribute % 100) / 10;
			//Tail_State = distribute % 10;
			printf("\nrecv: %s\n", &buff);
		}
		
		Recv_CondVar.notifyOne();
	}
}
void tran_socket(SOCKET sock) {
	/* Transmit Variable */
	char cMsg[BUFSIZE] = { 0, };
	string packet;
	int i = 0;
	int* bp;

	/* ID Change */
	scanf_s("%d", &Id); //0 cpp 1 rei 2 stt
	strcpy(cMsg, Data_Packet[0][Id].c_str()); //id : string -> char[]
	send(sock, cMsg, strlen(cMsg), 0); // id setting in Server

	while (1) {
		
		Send_CondVar.wait();
		if ((Lift_Sensor != 0) & (Lift_Sensor != 1))
		{
			Lift_Sensor = OFF;
		}
		bp = GUI::get_flag();
		for (int i = 0; i < 4; i++) {
			if (bp[2] == 1)
				Reward = 30;
			if (bp[3] == 1)
				Reward = -10;
		}
		if (bp[4] == 1)
			Touch_Sensor = 1;
		else if (bp[4] == 2)
			Touch_Sensor = 2;
		else if (bp[4] == 3)
			Touch_Sensor = 3;
		if (flag_FORCE == 1)
			Force_Sensor = 1;
		if (flag_Lift == 1)
			Lift_Sensor = 1;
		//windo_change = OFF;
			
		packet.append(Data_Packet[0][Id]);
		packet.append(",");
		packet.append(to_string(Hungry_Para));
		packet.append(",");
		packet.append(to_string(Tired_Para));
		packet.append(",");
		packet.append(Data_Packet[3][Touch_Sensor]);
		packet.append(",");
		packet.append(Data_Packet[4][Force_Sensor]);
		packet.append(",");
		packet.append(Data_Packet[5][Lift_Sensor]);
		packet.append(",");
		packet.append(Data_Packet[6][Oled_State]);
		packet.append(",");
		packet.append(Data_Packet[7][Fin_State]);
		packet.append(",");
		packet.append(Data_Packet[8][Tail_State]);
		packet.append(",");
		packet.append(to_string(Oled_State));
		packet.append(to_string(Fin_State));
		packet.append(to_string(Tail_State));
		packet.append(",");
		packet.append(to_string(Face_Detect));
		packet.append(",");
		packet.append(to_string(Reward));
		packet.append(",");
		packet.append(to_string(Stt_Data));

		strcpy(cMsg, packet.c_str()); //packet : string -> char[]
		packet.clear();
		flag_FORCE = 0;
		flag_Lift = 0;
		Send_Init_Variable();

		/*Packet Àü¼Û*/
		printf("\nsend :%d %d %d\n", Oled_State, Fin_State, Tail_State);
		send(sock, cMsg, strlen(cMsg), 0);
		//printf("force: %f\n\n", force_cur);

		Sleep(10);
	}
}

#endif /// !DATA_TRANSMISSION_H

