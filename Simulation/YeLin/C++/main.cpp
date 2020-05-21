#define _WINSOCK_DEPRECATED_NO_WARNINGS
/*
	TCP include Files
*/
#include "winsock2.h" // include before <windows.h>
#include <conio.h>
#define BUFSIZE 1024

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

/* 
	OpenCV Include Files
*/
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "VRep_GUI.h"

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

	//#include "extApiPlatform.h" 
}

using namespace std;
using namespace cv;
using namespace chrono;

#define M_PI         (3.141592653589793238462643383279501884197169399375105829974944)
#define TAB_COMMAND      9
#define Q_COMMAND      113
#define E_COMMAND      101
#define R_COMMAND      114
#define D_COMMAND      100
#define A_COMMAND      97
#define S_COMMAND      115
#define I_COMMAND      105
#define W_COMMAND      119
#define PORT         9200

/* Function Define */
void init();
void readdevice();
void writedevice();
void recv_socket(SOCKET sock);
void delay(clock_t n);
double dou_angle(double seta);
simxFloat simxF_angle(simxFloat seta);
void ErrorHandling(char* message);

/*////////////////////////////////[START Variable]//////////////////////////////// */
/* Motion Variables */
bool simulation_run = false;
int _Init_walking_flag,
_Walking_flag,
_Back_flag,
_Front_flag,
_Left_flag,
_Right_flag,
_program_exit,
_Reading_flag;
int clientID;
bool _visibility_flag = false;
int num;
int i_mode = 0;
int cnt;
int joint_angle = 0;
int	force_flag;
int mode_flag = 0;
int pre_EYE = 0;

simxInt   g_objHandle[6];
simxInt come_objHandle[7];
simxInt test_objHandle[2];
simxInt L_eye_objHandle[6];
simxInt R_eye_objHandle[6];
simxFloat		Body_Position[3] = { 0, };
simxFloat      L_Wing_Joint_X[3] = { dou_angle(-180),dou_angle(-0.00000000033872),dou_angle(90) };
simxFloat      L_Wing_Joint_Y[3] = { dou_angle(-90),dou_angle(-0.0000066594),dou_angle(90) };
simxFloat      R_Wing_Joint_X[3] = { dou_angle(-180),dou_angle(0.0000000000065138),dou_angle(-90) };
simxFloat      R_Wing_Joint_Y[3] = { dou_angle(-90),dou_angle(0.0000000085377),dou_angle(-90) };
simxFloat      L_Wing_Angle[3] = { dou_angle(69.186),dou_angle(-28.243),dou_angle(78.224) };
simxFloat      R_Wing_Angle[3] = { dou_angle(69.186),dou_angle(28.243),dou_angle(-78.224) };

simxFloat		Joint_Orientation_0[3] = { 0,0,0 };
simxFloat		Joint_Orientation_90[3] = { 90 * M_PI / 180,0,0 };
simxInt			body_Handle;
simxFloat		   q_cur[6];
simxInt		   image_Handle;
simxInt		   force_Handle;
double   initialPos[6] = { 200 * M_PI / 180, 190 * M_PI / 180, 0, 0, M_PI, M_PI };
double targetPos[3][6] = { { 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180 },
					 { 90 * M_PI / 180,135 * M_PI / 180,225 * M_PI / 180,180 * M_PI / 180,180 * M_PI / 180,350 * M_PI / 180 },
					 { 160 * M_PI / 180, 150 * M_PI / 180, 30 * M_PI / 180, 30 * M_PI / 180, 0, 0 } };
double leftrightPos[2][2] = { { -M_PI * 0.8, -M_PI }, { -M_PI, -M_PI * 0.8 } };
double      initPos[2] = { 200 * M_PI / 180, 190 * M_PI / 180 };
double      testPos[2] = { 160 * M_PI / 180, 150 * M_PI / 180 };

/* General Variables */
//simxInt   g_objHandle[6];
//simxInt come_objHandle[6];
//simxInt test_objHandle[2];

/* TCP Variables */
#include "Data_Format.h"
WSADATA wsaData;
SOCKET hSocket;
SOCKADDR_IN servAddr;

/* Mutex Variables */
mutex mMutex;
condition_variable mCondVar;
struct SafeCondVar
{
	int m_iNC_one;
	int m_iNC_all;
	mutex m_oMtx;
	mutex m_aMtx;
	condition_variable m_oCondVar;

	SafeCondVar();

	void notifyOne();
	void notifyAll();
	void wait();
};
SafeCondVar::SafeCondVar()
	: m_iNC_one(0), m_oMtx(), m_oCondVar()
{
}
void SafeCondVar::notifyAll(){
	lock_guard<mutex> oL(m_aMtx);
	++m_iNC_all;
	m_oCondVar.notify_all();
}
void SafeCondVar::notifyOne() {
	lock_guard<mutex> oL(m_oMtx);
	++m_iNC_one;
	m_oCondVar.notify_one();
}
void SafeCondVar::wait()
{
	unique_lock<mutex> oL(m_oMtx);
	while (true)
	{
		if (0 < m_iNC_one)
		{
			--m_iNC_one;
			return;
		}
		m_oCondVar.wait(oL);
	}
}
SafeCondVar Send_CondVar;
SafeCondVar Recv_CondVar;

/* Fps Variables */
chrono::system_clock::time_point	tpStart, tpEnd;
double	 dTime = 0;
int cont_while = 0;

/* OpenCV Variables */
simxUChar*	   comeimage = 0;
simxInt		   resolution[2];
char _fps[5];	  string fps;

/* Sensor Variables */
simxFloat force_cur = -0.182;
simxFloat getposition[3] = { 0, };

/*////////////////////////////////[END Variable]//////////////////////////////// */


/*////////////////////////////////[START Function]//////////////////////////////// */
/* Motion Function */
void init()
{
	//_Init_walking_flag = false;
	//_Walking_flag = false;
	//_program_exit = false;
	_Back_flag = false;
	_Front_flag = false;
	_program_exit = false;
	_Reading_flag = false;
	_program_exit = false;
}
void readdevice()
{
	simxFloat q_cur[2];
	/*
	simxFlo
 at *qc;   qc = q_cur;
	for (int i = 0; i < 2; i++) {
	   simxGetJointPosition(clientID, g_objHandle[i], qc, simx_opmode_streaming);
	   }
	*/

	// comebot
	for (int i = 0; i < 2; i++)
		simxGetJointPosition(clientID, test_objHandle[0], &q_cur[i], simx_opmode_streaming);
}
void writedevice()
{
	/*
	if (_Init_walking_flag == true) {
	   for (int i = 0; i < 6; i++)
		  simxSetJointTargetPosition(clientID, g_objHandle[i], initialPos[i], simx_opmode_streaming);
	}
	if (_Walking_flag == true) {
	   for (int i = 1; i < 7; i++) {
		  simxSetJointTargetPosition(clientID, g_objHandle[i - 1], targetPos[0][i - 1], simx_opmode_streaming);
		  //simxSetJointTargetPosition(clientID, g_objHandle[j], g_q[j], simx_opmode_streaming);
	   }
	}
	*/

	// comebot
	if (_Init_walking_flag == true) {
		for (int i = 0; i < 4; i++)
			simxSetJointTargetPosition(clientID, come_objHandle[i], initialPos[i], simx_opmode_streaming);
	}
	if (_Walking_flag == true) {
		for (int i = 0; i < 4; i++)
			simxSetJointTargetPosition(clientID, come_objHandle[i], targetPos[2][i], simx_opmode_streaming);
	}
}
void delay(clock_t n)

{
	clock_t start = clock();
	while (clock() - start < n);
}
double dou_angle(double seta)	//Radian을 도로 바꾸는 함수
{
	return seta * M_PI / 180;
}
simxFloat simxF_angle(simxFloat seta)	//Radian을 도로 바꾸는 함수
{
	return seta * M_PI / 180;
}
//Before
//void Fin_Action_comebot(int n)
//{
//	switch (n) {
//
//	case 0:
//		////////////// 날개 앞뒤로 크게 흔들기
//		//////////// left wing
//
//		//if(flag == 0)	// A target 으로 가기
//		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
//		//////////// right wing
//		//else if(flag == 1) // B target 으로 가기
//		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
//
//		break;
//
//	case 1:
//		////////////// 날개 위아래 흔들기
//		//////////// left wing
//
//		//if(flag == 0)	// A target 으로 가기
//		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
//		//////////// right wing
//		//else if(flag == 1) // B target 으로 가기
//		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
//
//		break;
//	case 2:
//		////////////// 날개 아래로 떨구기
//		//////////// left wing
//
//		//if(flag == 0)	// A target 으로 가기
//		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
//		//////////// right wing
//		//else if(flag == 1) // B target 으로 가기
//		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
//
//		break;
//
//	case 3:
//		///////////slow 지느러미를 천천히 앞뒤로
//
//		if (mode_flag == 0)		// A target 으로 가기
//		{
//			//////////// left wing
//			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
//			//////////// right wing
//			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
//		}
//		else if (mode_flag == 1)		// B target 으로 가기
//		{
//			//////////// left wing
//			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(60.), simx_opmode_streaming);
//			//////////// right wing
//			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-60.), simx_opmode_streaming);
//		}
//		break;
//	case 4:		//slow 지느러미를 천천히 앞뒤로
//
//		break;
//
//	default:
//
//		break;
//	}
//}
//void Tail_Action_comebot(int n)
//{
//
//	switch (n) {
//
//	case 0:
//		//////////// forward Tail
//		simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(200.), simx_opmode_streaming);
//		//////////// back Tail
//		simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
//		break;
//
//	case 1:
//		//////꼬리 천천히 흔들기
//		if (mode_flag == 0)		// A target 으로 가기
//		{
//			//////////// forward Tail
//			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(150.), simx_opmode_streaming);
//			//////////// back Tail
//			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
//		}
//		else if (mode_flag == 1)		// B target 으로 가기
//		{
//			//////////// forward Tail
//			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(200.), simx_opmode_streaming);
//			//////////// back Tail
//			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(230.), simx_opmode_streaming);
//		}
//		break;
//
//	case 2:
//		//////////// forward Tail
//		simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(200.), simx_opmode_streaming);
//		//////////// back Tail
//		simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
//		break;
//	case 3:
//		//////////// forward Tail
//		simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(200.), simx_opmode_streaming);
//		//////////// back Tail
//		simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
//		break;
//	case 4:
//
//		break;
//	default:
//
//		break;
//	}
//}
//void Eye_Action_comebot(int n)
//{
//	switch (n) {
//	case NORM:
//		//////////////////// 보이기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// 사라지기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case HAPPY:
//		//////////////////// 보이기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// 사라지기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case SAD:
//		//////////////////// 보이기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// 사라지기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case ANGRY:
//		//////////////////// 보이기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// 사라지기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case BORED:
//		//////////////////// 보이기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// 사라지기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case FUN:
//		//////////////////// 보이기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// 사라지기
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	default:
//		break;
//	}
//}
//void Mode_select(int Eye, int Wing, int Tail, int count)
//{
//	if (pre_EYE != Eye)
//		Eye_Action_comebot(Eye);
//
//	for (int i = 0; i < count; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			Fin_Action_comebot(Wing);
//			Tail_Action_comebot(Tail);
//			simxSynchronousTrigger(clientID);
//		}
//		/*for (int j = 0; j < 1000000; j++)
//		{
//		}*/
//		mode_flag = 1;
//		//delay(1000);
//		//Sleep(1000);
//		for (int j = 0; j < 3; j++)
//		{
//			Fin_Action_comebot(Wing);
//			Tail_Action_comebot(Tail);
//			simxSynchronousTrigger(clientID);
//		}
//		/*	for (int j = 0; j < 1000000; j++)
//			{
//			}*/
//		mode_flag = 0;
//		//delay(1000);
//		//Sleep(1000);
//	}
//}

void init_Joint_Angle_comebot()
{
	simxSetJointTargetPosition(clientID, come_objHandle[2], initPos[0], simx_opmode_streaming);
	simxSetJointTargetPosition(clientID, come_objHandle[3], initPos[1], simx_opmode_streaming);
	simxSetJointTargetPosition(clientID, come_objHandle[0], initPos[2], simx_opmode_streaming);
	simxSetJointTargetPosition(clientID, come_objHandle[1], initPos[3], simx_opmode_streaming);
}
void Fin_Angle_comebot(int n)
{
	switch (n) {

	case 0:
		////////////// 날개 앞뒤로 크게 흔들기
		/////////Left wing Orientation X 축으로 움직이게 끔 
		simxSetObjectOrientation(clientID, come_objHandle[2], -1, L_Wing_Joint_X, simx_opmode_oneshot_wait);

		/////////Right wing Orientation X 축으로 움직이게 끔 
		simxSetObjectOrientation(clientID, come_objHandle[3], -1, R_Wing_Joint_X, simx_opmode_oneshot_wait);
		break;

	case 1:
		////////////// 날개 위아래 흔들기
		/////////Left wing Orientation Y 축으로 움직이게 끔 
		simxSetObjectOrientation(clientID, come_objHandle[2], -1, L_Wing_Joint_Y, simx_opmode_oneshot_wait);
		//simxSetObjectOrientation(clientID, come_objHandle[7], -1, L_Wing_Angle, simx_opmode_oneshot_wait);
		/////////Right wing Orientation Y 축으로 움직이게 끔 
		simxSetObjectOrientation(clientID, come_objHandle[3], -1, R_Wing_Joint_Y, simx_opmode_oneshot_wait);
		//simxSetObjectOrientation(clientID, come_objHandle[8], -1, R_Wing_Angle, simx_opmode_oneshot_wait);
		break;
	case 2:
		////////////// 날개 아래로 떨구기
		/////////Left wing Orientation Y 축으로 움직이게 끔 
		simxSetObjectOrientation(clientID, come_objHandle[2], -1, L_Wing_Joint_Y, simx_opmode_oneshot_wait);
		/////////Right wing Orientation Y 축으로 움직이게 끔 
		simxSetObjectOrientation(clientID, come_objHandle[3], -1, R_Wing_Joint_Y, simx_opmode_oneshot_wait);

		break;

	case 3:
		///////////slow 지느러미를 천천히 앞뒤로
		/////////Left wing Orientation X 축으로 움직이게 끔 
		simxSetObjectOrientation(clientID, come_objHandle[2], -1, L_Wing_Joint_X, simx_opmode_oneshot_wait);

		/////////Right wing Orientation X 축으로 움직이게 끔 
		simxSetObjectOrientation(clientID, come_objHandle[3], -1, R_Wing_Joint_X, simx_opmode_oneshot_wait);

		break;
	case 4:      //slow 지느러미를 천천히 앞뒤로

		break;

	default:

		break;
	}
}
void Fin_Action_comebot(int n)
{
	switch (n) {

	case 0:
		////////////// 날개 앞뒤로 크게 흔들기      
		//////////// left wing
		if (mode_flag == 0)      // A target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(100.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-100.), simx_opmode_streaming);
		}
		break;

	case 1:
		////////////// 날개 위아래 흔들기
		if (mode_flag == 0)      // A target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(60.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-60.), simx_opmode_streaming);
		}
		break;
	case 2:
		////////////// 날개 아래로 떨구기

	 //////////// left wing
		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(90.), simx_opmode_streaming);
		//////////// right wing
		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-90.), simx_opmode_streaming);

		break;

	case 3:
		///////////slow 지느러미를 천천히 앞뒤로

		if (mode_flag == 0)      // A target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(30.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-30.), simx_opmode_streaming);
		}
		break;
	case 4:      //slow 지느러미를 천천히 앞뒤로

		break;

	default:

		break;
	}
}
void Tail_Action_comebot(int n)
{

	switch (n) {

	case 0:
		////꼬리 크게 흔들기
		if (mode_flag == 0)      // A target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(130.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(120.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(220.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(250.), simx_opmode_streaming);
		}

	case 1:
		//////꼬리 작게 흔들기
		if (mode_flag == 0)      // A target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(160.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(150.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(190.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(220.), simx_opmode_streaming);
		}
		break;

	case 2:
		///// 꼬리 내리기
		if (mode_flag == 0)      // A target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(220.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(150.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(230.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
		}
		break;
	case 3:
		////// 꼬리 세우기
		if (mode_flag == 0)      // A target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(130.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(150.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(120.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(160.), simx_opmode_streaming);
		}
		break;
	case 4:

		break;
	default:

		break;
	}
}
void Eye_Action_comebot(int n)
{
	simxFloat prop;
	prop = 1;
	switch (n) {
	case NORM:
		// 보이기
		simxCallScriptFunction(clientID, "normal_left", sim_scripttype_childscript, "normal_left", 1, &L_eye_objHandle[NORM], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "normal_right", sim_scripttype_childscript, "normal_right", 1, &R_eye_objHandle[NORM], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//// 사라지기
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//pre_EYE = n;
		break;
	case HAPPY:
		// 보이기
		simxCallScriptFunction(clientID, "happy_left", sim_scripttype_childscript, "happy_left", 1, &L_eye_objHandle[HAPPY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "happy_right", sim_scripttype_childscript, "happy_right", 1, &R_eye_objHandle[HAPPY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//// 사라지기
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//pre_EYE = n;
		break;
	case SAD:
		// 보이기
		simxCallScriptFunction(clientID, "sad_left", sim_scripttype_childscript, "sad_left", 1, &L_eye_objHandle[SAD], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "sad_right", sim_scripttype_childscript, "sad_right", 1, &R_eye_objHandle[SAD], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//// 사라지기
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//pre_EYE = n;
		break;
	case ANGRY:
		// 보이기
		simxCallScriptFunction(clientID, "angry_left", sim_scripttype_childscript, "angry_left", 1, &L_eye_objHandle[ANGRY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "angry_right", sim_scripttype_childscript, "angry_right", 1, &R_eye_objHandle[ANGRY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//// 사라지기
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//pre_EYE = n;
		break;
	case BORED:
		// 보이기
		simxCallScriptFunction(clientID, "boring_left", sim_scripttype_childscript, "boring_left", 1, &L_eye_objHandle[BORED], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "boring_right", sim_scripttype_childscript, "boring_right", 1, &R_eye_objHandle[BORED], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//// 사라지기
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//pre_EYE = n;
		break;
	case FUN:
		// 보이기
		simxCallScriptFunction(clientID, "fun_left", sim_scripttype_childscript, "fun_left", 1, &L_eye_objHandle[FUN], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "fun_right", sim_scripttype_childscript, "fun_right", 1, &R_eye_objHandle[FUN], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//// 사라지기
		//simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		//pre_EYE = n;
		break;
	default:
		break;
	}

	// 눈 사라지기
	prop = 2;
	switch (pre_EYE) {
	case NORM:
		simxCallScriptFunction(clientID, "normal_left", sim_scripttype_childscript, "normal_left", 1, &L_eye_objHandle[NORM], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "normal_right", sim_scripttype_childscript, "normal_right", 1, &R_eye_objHandle[NORM], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case HAPPY:
		simxCallScriptFunction(clientID, "happy_left", sim_scripttype_childscript, "happy_left", 1, &L_eye_objHandle[HAPPY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "happy_right", sim_scripttype_childscript, "happy_right", 1, &R_eye_objHandle[HAPPY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case SAD:
		simxCallScriptFunction(clientID, "sad_left", sim_scripttype_childscript, "sad_left", 1, &L_eye_objHandle[SAD], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "sad_right", sim_scripttype_childscript, "sad_right", 1, &R_eye_objHandle[SAD], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case ANGRY:
		simxCallScriptFunction(clientID, "angry_left", sim_scripttype_childscript, "angry_left", 1, &L_eye_objHandle[ANGRY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "angry_right", sim_scripttype_childscript, "angry_right", 1, &R_eye_objHandle[ANGRY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case BORED:
		simxCallScriptFunction(clientID, "boring_left", sim_scripttype_childscript, "boring_left", 1, &L_eye_objHandle[BORED], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "boring_right", sim_scripttype_childscript, "boring_right", 1, &R_eye_objHandle[BORED], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case FUN:
		simxCallScriptFunction(clientID, "fun_left", sim_scripttype_childscript, "fun_left", 1, &L_eye_objHandle[FUN], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "fun_right", sim_scripttype_childscript, "fun_right", 1, &R_eye_objHandle[FUN], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	default:
		break;
	}
}
void Mode_select(int Eye, int Wing, int Tail, int count)
{
	if (pre_EYE != Eye)
		Eye_Action_comebot(Eye);

	Fin_Angle_comebot(Wing);
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Fin_Action_comebot(Wing);
			Tail_Action_comebot(Tail);
			simxSynchronousTrigger(clientID);
		}
		/*for (int j = 0; j < 1000000; j++)
		{
		}*/
		mode_flag = 1;
		//delay(1000);
		//Sleep(1000);
		for (int j = 0; j < 5; j++)
		{
			Fin_Action_comebot(Wing);
			Tail_Action_comebot(Tail);
			simxSynchronousTrigger(clientID);
		}
		/*   for (int j = 0; j < 1000000; j++)
		   {
		   }*/
		mode_flag = 0;
		//delay(1000);
		//Sleep(1000);
	}
	for (int j = 0; j < 2; j++)
	{
		init_Joint_Angle_comebot();
		simxSynchronousTrigger(clientID);
	}
}

void forcesensor()
{
	simxReadForceSensor(clientID, force_Handle, NULL, &force_cur, NULL, simx_opmode_streaming);
}
void getpos()
{
	simxGetObjectPosition(clientID, body_Handle, -1, getposition, simx_opmode_blocking);
}
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

/* Img Function */
void getimage()
{
	//cv::namedWindow("vrep", WINDOW_AUTOSIZE);
	//Recv_CondVar.wait();
	while (1) {//for (int time = 0; time < 1000; time++) {
		/*Yelins Only*/
		/*int retval = simxGetVisionSensorImage(clientID, image_Handle, resolution, &comeimage, 0, simx_opmode_streaming);
		if (retval != simx_return_ok) {
			
			continue;
		}
		_itoa_s(dTime + 0.1, _fps, 5, 10);	fps = _fps;
		Mat img(resolution[0], resolution[1], CV_8UC3, comeimage);
		flip(img, img, 0);
		cvtColor(img, img, cv::COLOR_RGB2BGR);
		putText(img, "FPS:" + fps, Point(430, 30), FONT_HERSHEY_PLAIN, 1.2, Scalar(255, 230, 100));
		cv::imshow("vrep", img);
		if (++cont_while == 1000) {
			cont_while = 0;
			tpEnd = chrono::system_clock::now();
			dTime = 100000.0 / chrono::duration_cast<chrono::milliseconds>(tpEnd - tpStart).count();
			tpStart = tpEnd;
		}
		simxSynchronousTrigger(clientID);
		cv::waitKey(27);*/

		Mat img2 = Mat(Size(600, 600), CV_8UC3, Scalar::all(0));
		GUI::img_vrep = &img2;
		GUI::init();
		for (int i = 0; GUI::waitKeySuper(1); i++) {
			int retval = simxGetVisionSensorImage(clientID, image_Handle, resolution, &comeimage, 0, simx_opmode_streaming);
			if (retval != simx_return_ok) {
				printf("\n ** Vrep 연결 문제 !! \n ");
				continue;
			}

			// fps, 언제 끊기는지

			Mat img(resolution[0], resolution[1], CV_8UC3, comeimage);
			if (img.empty()) {
				printf("\n ** Vrep img is empty !! \n ");
				continue;
			}
			cvtColor(img, img2, cv::COLOR_RGB2BGR);

			// 밥, 잠, good, bad 순으로 보내는 부분
			bool* bp = GUI::get_flag();
			//for (int i = 0; i < 4; i++)
			//	cout << bp[i] << " ";
			//printf("\n");

			// 받는 부분
			bool cam_flag = false;
			GUI::cam(0, 0, cam_flag);

			int act[4] = { Oled_State,Fin_State,Tail_State,1 };
			GUI::action(act);
			GUI::show();
		}
		waitKey(27);
	}
}

/*////////////////////////////////[END Function]//////////////////////////////// */


/*////////////////////////////////[START Thread]//////////////////////////////// */
void Parameter_Thread() {
	while (1) {
		if (Hungry_Para > 0)
			Hungry_Para--;
		else
			Hungry_Para = 100;
		if (Tired_Para > 0)
			Tired_Para--;
		else
			Hungry_Para = 100;
		Sleep(5000);
	}
}
void recv_socket(SOCKET sock) {
	char buff[1024];
	int ibuff;
	string mode_buff;
	while (1) {
		
		ZeroMemory(buff, 1024);
		int bytesReceived = recv(sock, buff, 1024, 0);
		if (bytesReceived > 0)
		{
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
	char cMsg[] = "";
	string packet;
	int i = 0;

	/* ID Change */
	scanf_s("%d", &Id); //0 cpp 1 rei 2 stt
	strcpy(cMsg, Data_Packet[0][Id].c_str()); //id : string -> char[]
	send(sock, cMsg, strlen(cMsg), 0); // id setting in Server

	while (1) {
		Send_CondVar.wait();

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
		/*Packet 전송*/
		send(sock, cMsg, strlen(cMsg), 0);
		printf("\nSend_Packet: %s\n", cMsg);

		Sleep(10);
	}
}
void motion_control_thread() {
	int distribute = 0;
	printf("Start Program\n");
	clientID = simxStart((simxChar*)"127.0.0.1", 9200, true, true, 5000, 5);

	/* Tail Init */
	simxGetObjectHandle(clientID, "body_tail1", &come_objHandle[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "tail1_tail2", &come_objHandle[1], simx_opmode_oneshot_wait);
	/* Wing Init */
	simxGetObjectHandle(clientID, "wing_left_Motor", &come_objHandle[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_right_Motor", &come_objHandle[3], simx_opmode_oneshot_wait);
	/* Moter Init */
	simxGetObjectHandle(clientID, "left_wheel_Motor", &come_objHandle[4], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "right_wheel_Motor", &come_objHandle[5], simx_opmode_oneshot_wait);
	/* Body Init */
	simxGetObjectHandle(clientID, "body", &come_objHandle[6], simx_opmode_oneshot_wait);
	/* Sensor Init */
	simxGetObjectHandle(clientID, "camera", &image_Handle, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "shock", &force_Handle, simx_opmode_oneshot_wait);
	/* Eye Setting */
	simxGetObjectHandle(clientID, "normal_left", &L_eye_objHandle[NORM], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "normal_right", &R_eye_objHandle[NORM], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "happy_left", &L_eye_objHandle[HAPPY], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "happy_right", &R_eye_objHandle[HAPPY], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sad_left", &L_eye_objHandle[SAD], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sad_right", &R_eye_objHandle[SAD], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "angry_left", &L_eye_objHandle[ANGRY], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "angry_right", &R_eye_objHandle[ANGRY], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "boring_left", &L_eye_objHandle[BORED], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "boring_right", &R_eye_objHandle[BORED], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "fun_left", &L_eye_objHandle[FUN], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "fun_right", &R_eye_objHandle[FUN], simx_opmode_oneshot_wait);
	/* Test */
	simxGetObjectHandle(clientID, "test_joint", &test_objHandle[0], simx_opmode_oneshot_wait);

	int n = simxSynchronous(clientID, true);
	if (n == simx_return_remote_error_flag) {
		printf("%d\n", n);
	}
	simxInt error = simxStartSimulation(clientID, simx_opmode_oneshot_wait);
	if (error != simx_error_noerror) {
		throw string("Unable to start the simulation");
	}
	/* NORM Eye Visible */
	simxSetObjectIntParameter(clientID, L_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
	/* NORM Eye InVisible */
	simxSetObjectIntParameter(clientID, L_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, L_eye_objHandle[BORED], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[BORED], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, L_eye_objHandle[SAD], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[SAD], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, L_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, L_eye_objHandle[FUN], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[FUN], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	int flag = 0;
	while (1) {
		
		distribute = stoi(Mode[Mode_Select]);
		printf("Motion : %d %d %d \n", Oled_State, Fin_State, Tail_State);

		/*Motion Function*/
		if (clientID != -1) {
			printf("Connection Established\n");
			init();
			while (simxGetConnectionId(clientID) != -1) {
				if (flag == 0) {
					Recv_CondVar.wait();
					flag = 1;
				}
				Recv_CondVar.wait();
				readdevice();
				/*Recv_CondVar.wait();
				distribute = stoi(Mode[Mode_Select]);
				printf("Motion : %d %d %d \n", Oled_State, Fin_State, Tail_State);*/
				
				/*				
				if (kbhit())
				{
					int key = _getch();
					printf("%d\t", key);
					switch (key) {
					case I_COMMAND:
						cout << " Initial Pos\n" << endl;
						//_Init_walking_flag = true;
						//_Walking_flag = false;
						//_visibility_flag = false;
						Mode_select(1, 3, 1, 2);
						Send_CondVar.notifyOne();
						break;

					case R_COMMAND:

						cout << " Put\n" << endl;
						Mode_select(4, 3, 1, 2);
						//_Walking_flag = true;
						//_Init_walking_flag = false;
						//_visibility_flag = true;
						//_WalkingCtrl._initialize();
						//_WalkingCtrl.StartWalking();
						//_WalkingCtrl.setApproachdata(0.0,0.0,50*DEGREE);
						Send_CondVar.notifyOne();
						break;

					case TAB_COMMAND:
						if (simulation_run) {
							simulation_run = false;
							cout << " Stop\n" << endl;
						}
						else {
							simulation_run = true;
							cout << "  Go\n" << endl;
						}
						Send_CondVar.notifyOne();
						break;

					case W_COMMAND:
						//////// GO
						simxSetJointTargetVelocity(clientID, come_objHandle[4], -10, simx_opmode_streaming);
						simxSetJointTargetVelocity(clientID, come_objHandle[5], -10, simx_opmode_streaming);
						cout << "Motor Go\n" << endl;
						Send_CondVar.notifyOne();
						break;

					case S_COMMAND:
						//////// BACK
						simxSetJointTargetVelocity(clientID, come_objHandle[4], 10, simx_opmode_streaming);
						simxSetJointTargetVelocity(clientID, come_objHandle[5], 10, simx_opmode_streaming);
						cout << "Motor Back\n" << endl;
						Send_CondVar.notifyOne();
						break;

					case A_COMMAND:
						//////// LEFT TURN
						simxSetJointTargetVelocity(clientID, come_objHandle[4], -5, simx_opmode_streaming);
						simxSetJointTargetVelocity(clientID, come_objHandle[5], -10, simx_opmode_streaming);
						cout << "Motor Left\n" << endl;
						Send_CondVar.notifyOne();
						break;

					case D_COMMAND:
						//////// RIGHT TURN
						simxSetJointTargetVelocity(clientID, come_objHandle[4], -10, simx_opmode_streaming);
						simxSetJointTargetVelocity(clientID, come_objHandle[5], -5, simx_opmode_streaming);
						cout << "Motor Right\n" << endl;
						Send_CondVar.notifyOne();
						break;

					case E_COMMAND:
						simxSetJointTargetVelocity(clientID, come_objHandle[4], 0, simx_opmode_streaming);
						simxSetJointTargetVelocity(clientID, come_objHandle[5], 0, simx_opmode_streaming);
						cout << "Motor Stop\n" << endl;

						//////// joint rotation
						if (joint_angle == 0)
						{
							joint_angle = 1;
							simxSetObjectOrientation(clientID, test_objHandle[0], -1, Joint_Orientation_90, simx_opmode_streaming);
						}
						else if (joint_angle == 1)
						{
							joint_angle = 0;
							simxSetObjectOrientation(clientID, test_objHandle[0], -1, Joint_Orientation_0, simx_opmode_streaming);
						}
						//////////// Read Body Position
						simxGetObjectPosition(clientID, come_objHandle[6], -1, Body_Position, simx_opmode_streaming);
						printf("Position : %f, %f, %f \n", Body_Position[0], Body_Position[1], Body_Position[2]);
						Send_CondVar.notifyOne();
						break;

					case Q_COMMAND:
						cout << " Program End\n" << endl;
						simulation_run = false;
						_program_exit = true;
						Send_CondVar.notifyOne();
						break;

					default:
						break;
					}
				}
				*/
				

				Mode_select(Oled_State,Fin_State,Tail_State, 2);
				Send_CondVar.notifyOne();
				//Send_CondVar.notifyAll();
				if (simulation_run == true)
				{
					//if (_Init_walking_flag == true) {
					//	/*      initial Pos      */
					//	writedevice();
					//	simxSynchronousTrigger(clientID);
					//}
					//else if (_Walking_flag == true) {
					//	/*      target Pos      */
					//	writedevice();
					//	simxSynchronousTrigger(clientID);
					//}
					simxSynchronousTrigger(clientID);					
				}
				/*Send_CondVar.notifyOne();*/
			}
			simxFinish(clientID);
			
		}

		/*Motion End*/
		
	}
}
/*////////////////////////////////[END Thread]//////////////////////////////// */


int main(int argc, const char* argv[])
{
	Hungry_Para = 50;

	/* TCP Transmission_Init */
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0); //소켓 라이브러리를 초기화하고 있다
		//ErrorHandling("WSAStartup() error!");
	hSocket = socket(PF_INET, SOCK_STREAM, 0); //소켓을 생성
	if (hSocket == INVALID_SOCKET);
		//ErrorHandling("socket() error");
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	servAddr.sin_port = htons(8585);
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR); //생성된 소켓을 바탕으로 서버에 연결요청을 하고 있다
		//ErrorHandling("connect() error!");



	/* Thread_Init */
	
	thread para;	
	thread recvimg;
	thread vrep;
	thread tran;
	thread recv;
	
	recv = thread(recv_socket, hSocket);
	para = thread(Parameter_Thread);
	recvimg = thread(getimage);
	vrep = thread(motion_control_thread);
	tran = thread(tran_socket, hSocket);
	
	

	recv.join();
	para.join();
	recvimg.join();
	vrep.join();
	tran.join();
	

	closesocket(hSocket); //소켓 라이브러리 해제
	WSACleanup();
}


/*
	TCP Transmission User Func List
*/
//////////////////////////////////////////////////////////////////////////
//void ErrorHandling(char* message) {
//	WSACleanup();
//	fputs(message, stderr);
//	fputc('\n', stderr);
//	_getch();
//	exit(1);
//}
//////////////////////////////////////////////////////////////////////////
