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

#define BUFSIZE 1024

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
	string mode_buff;
	while (1) {
		ZeroMemory(buff, 1024);
		int bytesReceived = recv(sock, buff, 1024, 0);
		if (bytesReceived > 0) {
			ibuff = atoi(buff); // buff : char[] -> int
			ibuff = atoi(Mode[ibuff].c_str());

			Oled_State = ibuff / 100;
			Fin_State = (ibuff % 100) / 10;
			Tail_State = ibuff % 10;
			printf("\nrecv: %s\n", &buff);
		}

		Recv_CondVar.notifyOne();
	}
}
void tran_socket(SOCKET sock) {
	/* Transmit Variable */
	char cMsg[BUFSIZE] = "";
	string packet;
	int i = 0;

	/* ID Change */
	scanf_s("%d", &Id); //0 cpp 1 rei 2 stt
	strcpy(cMsg, Data_Packet[0][Id].c_str()); //id : string -> char[]
	send(sock, cMsg, strlen(cMsg), 0); // id setting in Server

	while (1) {
		Send_CondVar.wait();
		GUI::getPara();
		packet =
			Data_Packet[0][Id] + ","
			+ to_string(Hungry_Para) + ","
			+ to_string(Tired_Para) + ","
			+ Data_Packet[3][Touch_Sensor] + ","
			+ Data_Packet[4][Force_Sensor] + ","
			+ Data_Packet[5][Lift_Sensor] + ","
			+ Data_Packet[6][Oled_State] + ","
			+ Data_Packet[7][Fin_State] + ","
			+ Data_Packet[8][Tail_State] + ","
			+ to_string(Oled_State)
			+ to_string(Fin_State)
			+ to_string(Tail_State) + ","
			+ to_string(Face_Detect) + ","
			+ to_string(Reward);
		strcpy(cMsg, packet.c_str()); //packet : string -> char[]
		packet.clear();
		/*Packet Àü¼Û*/
		send(sock, cMsg, strlen(cMsg), 0);
		printf("\nSend_Packet: %s\n", cMsg);

		Sleep(10);
	}
}

#endif /// !DATA_TRANSMISSION_H

