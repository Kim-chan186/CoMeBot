#pragma once
/*
	TCP include Files
*/
#include "winsock2.h" // include before <windows.h>
#include <conio.h>
#include <opencv2/opencv.hpp>
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
#include "Data_Format.h"
extern "C" {
#include "extApi.h"
#include <v_repConst.h>
}

/* key input */
#define M_PI		  (3.141592653589793238462643383279501884197169399375105829974944)
#define TAB_COMMAND    9
#define Q_COMMAND      113
#define E_COMMAND      101
#define R_COMMAND      114
#define D_COMMAND      100
#define A_COMMAND      97
#define S_COMMAND      115
#define I_COMMAND      105
#define W_COMMAND      119
#define PORT           9200

using namespace std;

/* Action function */
void				   vreptcp_init();
void				 vrep_parameter();
void		 recv_socket(SOCKET sock);
void				 delay(clock_t n);
double		   dou_angle(double seta);
simxFloat simxF_angle(simxFloat seta);


/*////////////////////////////////[START Variable]//////////////////////////////// */
/* Motion Variables */
bool		   simulation_run = false;
bool		 _visibility_flag = false;
int							 clientID;
int					  joint_angle = 0;
int						   force_flag;
int							lift_flag;
int						mode_flag = 0;
int						  pre_EYE = 0;
simxFloat						 prop;
simxFloat					force_cur;
simxFloat	  getposition[3] = { 0, };


/* General Variables */
simxInt			 bodyHandle;
simxInt	  come_objHandle[7];
simxInt  L_eye_objHandle[6];
simxInt  R_eye_objHandle[6];
simxFloat  Body_Position[3] = { 0, };
simxFloat L_Wing_Joint_X[3] = { dou_angle(-180),dou_angle(-0.00000000033872),dou_angle(90) };
simxFloat L_Wing_Joint_Y[3] = { dou_angle(-90),dou_angle(-0.0000066594),dou_angle(90) };
simxFloat R_Wing_Joint_X[3] = { dou_angle(-180),dou_angle(0.0000000000065138),dou_angle(-90) };
simxFloat R_Wing_Joint_Y[3] = { dou_angle(-90),dou_angle(0.0000000085377),dou_angle(-90) };
simxFloat   L_Wing_Angle[3] = { dou_angle(69.186),dou_angle(-28.243),dou_angle(78.224) };
simxFloat   R_Wing_Angle[3] = { dou_angle(69.186),dou_angle(28.243),dou_angle(-78.224) };

simxInt		   image_Handle;
simxInt		   force_Handle;
double			 initPos[2] = { 200 * M_PI / 180, 190 * M_PI / 180 };
double			 testPos[2] = { 160 * M_PI / 180, 150 * M_PI / 180 };

/*	TCP Variables	*/
WSADATA				wsaData;
SOCKET				hSocket;
SOCKADDR_IN		   servAddr;
/*////////////////////////////////[END Variable]//////////////////////////////// */







/*////////////////////////////////////////////////////////////////////////////// */
void vreptcp_init()
{
	/* VREP Transmission_Init */


	/* TCP Transmission_Init */
	WSAStartup(MAKEWORD(2, 2), &wsaData);					  //소켓 라이브러리를 초기화하고 있다
	hSocket = socket(PF_INET, SOCK_STREAM, 0);				  //소켓을 생성
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	servAddr.sin_port = htons(8585);
	connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)); //생성된 소켓을 바탕으로 서버에 연결요청을 하고 있다
}
/*////////////////////////////////////////////////////////////////////////////// */
void vrep_parameter()
{
	// force sensor
	simxReadForceSensor(clientID, force_Handle, NULL, &force_cur, NULL, simx_opmode_streaming);
	if (force_cur < -0.20)			force_flag = 1;			// -0.182;
	else							force_flag = 0;
	printf("force: %.3f\t\t", force_cur);

	// comebot position
	simxGetObjectPosition(clientID, come_objHandle[6], -1, Body_Position, simx_opmode_streaming);
	printf("Position : %.2f, %.2f, %.2f \n", Body_Position[0]*100, Body_Position[1]*100, Body_Position[2]*100);
	if (getposition[2] > 2.0)		lift_flag = 1;
	else							lift_flag = 0;

}