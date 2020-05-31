//#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
#include <cstring>

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
double dou_angle(double seta)	//Radian�� ���� �ٲٴ� �Լ�
{
	return seta * M_PI / 180;
}
simxFloat simxF_angle(simxFloat seta)	//Radian�� ���� �ٲٴ� �Լ�
{
	return seta * M_PI / 180;
}
//Before
//void Fin_Action_comebot(int n)
//{
//	switch (n) {
//
//	case 0:
//		////////////// ���� �յڷ� ũ�� ����
//		//////////// left wing
//
//		//if(flag == 0)	// A target ���� ����
//		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
//		//////////// right wing
//		//else if(flag == 1) // B target ���� ����
//		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
//
//		break;
//
//	case 1:
//		////////////// ���� ���Ʒ� ����
//		//////////// left wing
//
//		//if(flag == 0)	// A target ���� ����
//		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
//		//////////// right wing
//		//else if(flag == 1) // B target ���� ����
//		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
//
//		break;
//	case 2:
//		////////////// ���� �Ʒ��� ������
//		//////////// left wing
//
//		//if(flag == 0)	// A target ���� ����
//		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
//		//////////// right wing
//		//else if(flag == 1) // B target ���� ����
//		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
//
//		break;
//
//	case 3:
//		///////////slow �������̸� õõ�� �յڷ�
//
//		if (mode_flag == 0)		// A target ���� ����
//		{
//			//////////// left wing
//			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
//			//////////// right wing
//			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
//		}
//		else if (mode_flag == 1)		// B target ���� ����
//		{
//			//////////// left wing
//			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(60.), simx_opmode_streaming);
//			//////////// right wing
//			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-60.), simx_opmode_streaming);
//		}
//		break;
//	case 4:		//slow �������̸� õõ�� �յڷ�
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
//		//////���� õõ�� ����
//		if (mode_flag == 0)		// A target ���� ����
//		{
//			//////////// forward Tail
//			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(150.), simx_opmode_streaming);
//			//////////// back Tail
//			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
//		}
//		else if (mode_flag == 1)		// B target ���� ����
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
//		//////////////////// ���̱�
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// �������
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case HAPPY:
//		//////////////////// ���̱�
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// �������
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case SAD:
//		//////////////////// ���̱�
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// �������
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case ANGRY:
//		//////////////////// ���̱�
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// �������
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case BORED:
//		//////////////////// ���̱�
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// �������
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
//		pre_EYE = n;
//		break;
//	case FUN:
//		//////////////////// ���̱�
//		simxSetObjectIntParameter(clientID, L_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		simxSetObjectIntParameter(clientID, R_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
//		//////////////////// �������
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
		////////////// ���� �յڷ� ũ�� ����
		/////////Left wing Orientation X ������ �����̰� �� 
		simxSetObjectOrientation(clientID, come_objHandle[2], -1, L_Wing_Joint_X, simx_opmode_oneshot_wait);

		/////////Right wing Orientation X ������ �����̰� �� 
		simxSetObjectOrientation(clientID, come_objHandle[3], -1, R_Wing_Joint_X, simx_opmode_oneshot_wait);
		break;

	case 1:
		////////////// ���� ���Ʒ� ����
		/////////Left wing Orientation Y ������ �����̰� �� 
		simxSetObjectOrientation(clientID, come_objHandle[2], -1, L_Wing_Joint_Y, simx_opmode_oneshot_wait);
		//simxSetObjectOrientation(clientID, come_objHandle[7], -1, L_Wing_Angle, simx_opmode_oneshot_wait);
		/////////Right wing Orientation Y ������ �����̰� �� 
		simxSetObjectOrientation(clientID, come_objHandle[3], -1, R_Wing_Joint_Y, simx_opmode_oneshot_wait);
		//simxSetObjectOrientation(clientID, come_objHandle[8], -1, R_Wing_Angle, simx_opmode_oneshot_wait);
		break;
	case 2:
		////////////// ���� �Ʒ��� ������
		/////////Left wing Orientation Y ������ �����̰� �� 
		simxSetObjectOrientation(clientID, come_objHandle[2], -1, L_Wing_Joint_Y, simx_opmode_oneshot_wait);
		/////////Right wing Orientation Y ������ �����̰� �� 
		simxSetObjectOrientation(clientID, come_objHandle[3], -1, R_Wing_Joint_Y, simx_opmode_oneshot_wait);

		break;

	case 3:
		///////////slow �������̸� õõ�� �յڷ�
		/////////Left wing Orientation X ������ �����̰� �� 
		simxSetObjectOrientation(clientID, come_objHandle[2], -1, L_Wing_Joint_X, simx_opmode_oneshot_wait);

		/////////Right wing Orientation X ������ �����̰� �� 
		simxSetObjectOrientation(clientID, come_objHandle[3], -1, R_Wing_Joint_X, simx_opmode_oneshot_wait);

		break;
	case 4:      //slow �������̸� õõ�� �յڷ�

		break;

	default:

		break;
	}
}
void Fin_Action_comebot(int n)
{
	switch (n) {

	case 0:
		////////////// ���� �յڷ� ũ�� ����      
		//////////// left wing
		if (mode_flag == 0)      // A target ���� ����
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target ���� ����
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(100.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-100.), simx_opmode_streaming);
		}
		break;

	case 1:
		////////////// ���� ���Ʒ� ����
		if (mode_flag == 0)      // A target ���� ����
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target ���� ����
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(60.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-60.), simx_opmode_streaming);
		}
		break;
	case 2:
		////////////// ���� �Ʒ��� ������

	 //////////// left wing
		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(90.), simx_opmode_streaming);
		//////////// right wing
		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-90.), simx_opmode_streaming);

		break;

	case 3:
		///////////slow �������̸� õõ�� �յڷ�

		if (mode_flag == 0)      // A target ���� ����
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target ���� ����
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(30.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-30.), simx_opmode_streaming);
		}
		break;
	case 4:      //slow �������̸� õõ�� �յڷ�

		break;

	default:

		break;
	}
}
void Tail_Action_comebot(int n)
{

	switch (n) {

	case 0:
		////���� ũ�� ����
		if (mode_flag == 0)      // A target ���� ����
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(130.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(120.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target ���� ����
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(220.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(250.), simx_opmode_streaming);
		}

	case 1:
		//////���� �۰� ����
		if (mode_flag == 0)      // A target ���� ����
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(160.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(150.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target ���� ����
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(190.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(220.), simx_opmode_streaming);
		}
		break;

	case 2:
		///// ���� ������
		if (mode_flag == 0)      // A target ���� ����
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(220.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(150.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target ���� ����
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(230.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
		}
		break;
	case 3:
		////// ���� �����
		if (mode_flag == 0)      // A target ���� ����
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(130.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(150.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)      // B target ���� ����
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
	switch (n) {
	case NORM:
		//////////////////// ���̱�
		simxSetObjectIntParameter(clientID, L_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// �������
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case HAPPY:
		//////////////////// ���̱�
		simxSetObjectIntParameter(clientID, L_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// �������
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case SAD:
		//////////////////// ���̱�
		simxSetObjectIntParameter(clientID, L_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// �������
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case ANGRY:
		//////////////////// ���̱�
		simxSetObjectIntParameter(clientID, L_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// �������
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case BORED:
		//////////////////// ���̱�
		simxSetObjectIntParameter(clientID, L_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// �������
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case FUN:
		//////////////////// ���̱�
		simxSetObjectIntParameter(clientID, L_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// �������
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
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
		/*for (int j = 0; j < 1000000; j++)
		{
		}*/
		{
			Fin_Action_comebot(Wing);
			Tail_Action_comebot(Tail);
			simxSynchronousTrigger(clientID);
		}
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
	while (1) {
		Mat img2 = Mat(Size(600, 600), CV_8UC3, Scalar::all(0));
		GUI::img_vrep = &img2;
		GUI::init();
		for (int i = 0; GUI::waitKeySuper(1); i++) {
			int retval = simxGetVisionSensorImage(clientID, image_Handle, resolution, &comeimage, 0, simx_opmode_streaming);
			if (retval != simx_return_ok) {
				printf("\n ** ! ���! \n");
				continue;
			}
			Mat img(resolution[0], resolution[1], CV_8UC3, comeimage);
			//flip(img, img, 0);
			if (img.empty()) {
				printf("\n ** Vrep img is empty! *********************************************\n");
				continue;
			}
			else {
				cvtColor(img, img2, cv::COLOR_RGB2BGR);
			}
			GUI::GazeSync(Hungry_Para, Tired_Para);
			// ��, ��, good, bad ������ ������ �κ�
			/*bool* bp = GUI::get_flag();
			for (int i = 0; i < 4; i++) {
				printf("@%d@", bp[i]);
			}
			printf("\n")z;*/

			// �޴� �κ�
			bool cam_flag = false;
			GUI::cam(0, 0, cam_flag);

			int act[4] = { Oled_State,Fin_State,Tail_State,1 };
			GUI::action(act);
			GUI::show();
		}
		waitKey(27);
	}
}
void getPara()
{	//GUI���� Ŭ���̺�Ʈ �޾ƿ��� �Լ�, �� step���� �Էµ� Ŭ���̺�Ʈ�� �޾ƿ�
	bool data[4] = {};
	bool flag[4] = {};
	for (int i = 0; i < 4; i++)
	{
		flag[i] = GUI::get_click()[i];
	}
	//printf(flag[0] ? "feed:true\t" : "feed:false\t");
	//Ŭ���̺�Ʈ�� ���� parameter������Ʈ
	if (flag[0])
	{
		if (Hungry_Para <= 70)
			Hungry_Para += 30;
		else
			Hungry_Para = 100;
	}
	//printf(flag[1] ? "sleep:true\t" : "sleep:false\t");
	if (flag[1])
	{
		if (Tired_Para <= 70)
			Tired_Para += 30;
		else
			Tired_Para = 100;
	}
	//printf(flag[2] ? "good:true\t" : "good:false\t");
	//printf(flag[3] ? "bad:true\t" : "bad:false\n");
	if (flag[2] | flag[3])
	{
		if (flag[2])
			Reward = 100;
		else
			Reward = -30;
	}
	else
		Reward = 0;

	GUI::reset_click();	//click event �ʱ�ȭ
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
			Tired_Para = 100;
		Sleep(5000);
	}
}
void recv_socket(SOCKET sock) {
	char buff[1024];
	int ibuff;
	string sbuff;
	string str_data[3];
	int str_cnt=0;
	char* tok;
	while (1) {
		ZeroMemory(buff, 1024);
		int bytesReceived = recv(sock, buff, 1024, 0);
		if (bytesReceived > 0)
		{
			ibuff = atoi(buff); // buff : char[] -> int
			//ibuff = atoi(Mode[ibuff].c_str());
			Mode_Select = ibuff / 10;
			Stt_Data = ibuff % 10;
			printf("%d", Mode_Select);
			printf("\nrecv: %s\n", &buff);
		}
		Recv_CondVar.notifyOne();
	}
}
void tran_socket(SOCKET sock) {
	/* Transmit Variable */
	char cMsg[1024] = "";
	string packet;
	int i = 0;
	bool* bp;

	/* ID Change */
	scanf_s("%d", &Id); //0 cpp 1 rei 2 stt
	strcpy(cMsg, Data_Packet[0][Id].c_str()); //id : string -> char[]
	send(sock, cMsg, strlen(cMsg), 0); // id setting in Server

	while (1) {
		Send_CondVar.wait();
		bp = GUI::get_flag();
		for (int i = 0; i < 4; i++) {
			printf("@%d@", bp[i]);
			if (bp[2] == 1)
				Reward = 10;
			if (bp[3] == 1)
				Reward = -10;
		}
		printf("\n");

		//getPara();	//Ŭ���̺�Ʈ�� ���� parameter������Ʈ
		packet = //"cpp, m10, m20, m30, m41, m53, m61, 131, 0, 0, 0";
		Data_Packet[0][Id] + ","
		+ to_string(Hungry_Para) + "," //bp[0]
		+ to_string(Tired_Para) + "," //bp[1]
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
		/*Packet ����*/
		packet.clear();
		//Reward = 0;
		Send_Init_Variable();
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
				
				distribute = stoi(Mode[Mode_Select]);
				Oled_State = distribute / 100;
				Fin_State = (distribute % 100) / 10;
				Tail_State = distribute % 10;
				printf("Motion : %d %d %d \n", Oled_State, Fin_State, Tail_State);
				Mode_select(Oled_State,Fin_State,Tail_State, 2);
				Send_CondVar.notifyOne();

				if (simulation_run == true)
				{
					simxSynchronousTrigger(clientID);					
				}
			}
			simxFinish(clientID);
			
		}

		/*Motion End*/
		
	}
}
/*////////////////////////////////[END Thread]//////////////////////////////// */


int main(int argc, char* argv[])
{
	Hungry_Para = 50;
	Tired_Para = 50;
	/* TCP Transmission_Init */
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //���� ���̺귯���� �ʱ�ȭ�ϰ� �ִ�
		ErrorHandling("WSAStartup() error!");
	hSocket = socket(PF_INET, SOCK_STREAM, 0); //������ ����
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	servAddr.sin_port = htons(8585);
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //������ ������ �������� ������ �����û�� �ϰ� �ִ�
		ErrorHandling("connect() error!");



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
	

	closesocket(hSocket); //���� ���̺귯�� ����
	WSACleanup();
}


/*
	TCP Transmission User Func List
*/
//////////////////////////////////////////////////////////////////////////
void ErrorHandling(char* message) {
	WSACleanup();
	fputs(message, stderr);
	fputc('\n', stderr);
	_getch();
	exit(1);
}
//////////////////////////////////////////////////////////////////////////
