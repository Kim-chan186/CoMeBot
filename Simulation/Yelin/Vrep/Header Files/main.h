#pragma once
/*
	TCP include Files
*/
#include "winsock2.h" // include before <windows.h>
#include <conio.h>
/*
	Vrep Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <string.h>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "Data_Format.h"
/*
	Thread Include Files
*/
#include <thread>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include "typeinfo"
extern "C" {
#include "extApi.h"
#include <v_repConst.h>
}

/* key input */
#define M_PI           (3.141592653589793238462643383279501884197169399375105829974944)
#define TAB_COMMAND    9
#define Q_COMMAND      113
#define S_COMMAND      115
#define W_COMMAND      119
#define R_COMMAND	   114
#define C_COMMAND	   99
#define D_COMMAND	   100
#define A_COMMAND	   97
#define PORT           9200

using namespace std;

/* Action function */
void				 init();
void		  forcesensor();
void	 writedevice(int n);
void				 back();
void				front();
void				 left();
void				right();
/* TCP function */
void recv_socket(SOCKET sock);
void ErrorHandling(const char* message);

/* flag */
bool simulation_run = false;
int				 _Back_flag,
				_Front_flag,
				 _Left_flag,
				_Right_flag,
			  _program_exit,
			  _Reading_flag;

int				   clientID;
int						num;
int						cnt;
int				 force_flag;
/* general variables */
simxInt		 g_objHandle[6];
simxInt	  come_objHandle[6];
simxInt			body_Handle;

simxFloat		   q_cur[6];
simxInt		   image_Handle;
simxInt		   force_Handle;
/* tcp variables */
WSADATA				wsaData;
SOCKET				hSocket;
SOCKADDR_IN		   servAddr;


double		initialPos[6] = { 200 * M_PI / 180, 190 * M_PI / 180, 0, 0, M_PI * 1.05, M_PI };
double	  targetPos[3][6] = { { 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180 },
							{ 90 * M_PI / 180,135 * M_PI / 180,225 * M_PI / 180,180 * M_PI / 180,180 * M_PI / 180,350 * M_PI / 180 },
							{ 160 * M_PI / 180, 150 * M_PI / 180, 30 * M_PI / 180, 30 * M_PI / 180, -M_PI, -M_PI } };
double leftrightPos[2][2] = { { -M_PI * 0.8, -M_PI }, { -M_PI, -M_PI * 0.8 } };
double		   initPos[2] = { 200 * M_PI / 180, 190 * M_PI / 180 };
double         testPos[2] = { 160 * M_PI / 180, 150 * M_PI / 180 };


/*
	Action mode Func List
*/
/*///////////////////////////////////////////////////////////////////////////*/
void init()
{
	/* VREP Transmission_Init */
	_Back_flag = false;
	_Front_flag = false;
	_program_exit = false;
	_Reading_flag = false;


	/* TCP Transmission_Init */
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //소켓 라이브러리를 초기화하고 있다
		ErrorHandling("WSAStartup() error!");
	hSocket = socket(PF_INET, SOCK_STREAM, 0); //소켓을 생성
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	servAddr.sin_port = htons(8585);
}
/*///////////////////////////////////////////////////////////////////////////*/
void back()
{
	for (int i = 0; i < 4; i++)
		simxSetJointTargetPosition(clientID, come_objHandle[i], initialPos[i], simx_opmode_streaming);

	simxSetJointTargetVelocity(clientID, come_objHandle[4], initialPos[4], simx_opmode_streaming);
	simxSetJointTargetVelocity(clientID, come_objHandle[5], initialPos[5], simx_opmode_streaming);
}
void front()
{
	for (int i = 0; i < 4; i++)
		simxSetJointTargetPosition(clientID, come_objHandle[i], targetPos[2][i], simx_opmode_streaming);

	simxSetJointTargetVelocity(clientID, come_objHandle[4], targetPos[2][4], simx_opmode_streaming);
	simxSetJointTargetVelocity(clientID, come_objHandle[5], targetPos[2][5], simx_opmode_streaming);
}
void left()
{
	simxSetJointTargetVelocity(clientID, come_objHandle[4], leftrightPos[0][0], simx_opmode_streaming);
	simxSetJointTargetVelocity(clientID, come_objHandle[5], leftrightPos[0][1], simx_opmode_streaming);
}
void right()
{
	simxSetJointTargetVelocity(clientID, come_objHandle[4], leftrightPos[1][0], simx_opmode_streaming);
	simxSetJointTargetVelocity(clientID, come_objHandle[5], leftrightPos[1][1], simx_opmode_streaming);
}
/*///////////////////////////////////////////////////////////////////////////*/


/*
	TCP Transmission User Func List
*/
void ErrorHandling(const char* message) {
	WSACleanup();
	fputs(message, stderr);
	fputc('\n', stderr);
	_getch();
	exit(1);
}