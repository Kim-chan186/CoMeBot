#include "winsock2.h" // include before <windows.h>
#define BUFSIZE 1024
void ErrorHandling(char* message);
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <thread>
#include <Data_Format.h>
#include <time.h>
extern "C" {
#include "extApi.h"
#include <v_repConst.h>
}

using namespace std;
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


void init();
void readdevice();
void writedevice();

bool simulation_run = false;
int _Init_walking_flag,
_Walking_flag,
_program_exit;
int clientID;
bool _visibility_flag = false;
int num;
int i_mode = 0;
int joint_angle = 0;
int mode_flag = 0;
int pre_EYE = 0;

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

/* general variables */
simxInt   g_objHandle[6];
simxInt come_objHandle[7];
simxInt test_objHandle[2];
simxInt L_eye_objHandle[6];
simxInt R_eye_objHandle[6];
simxFloat		Body_Position[3] = { 0, };
simxFloat		L_Wing_Joint_X[3] = { dou_angle(-180),dou_angle(-0.00000000033872),dou_angle(90) };
simxFloat		R_Wing_Joint_X[3] = { dou_angle(-180),dou_angle(0.0000000000065138),dou_angle(-90) };

simxFloat		Joint_Orientation_0[3] = { 0,0,0 };
simxFloat		Joint_Orientation_90[3] = { 90 * M_PI / 180,0,0 };

/*/////////////////////////// */
/* tcp variables */
WSADATA wsaData;
SOCKET hSocket;
SOCKADDR_IN servAddr;
/*/////////////////////////// */


double   initialPos[6] = { 200 * M_PI / 180, 190 * M_PI / 180, 0, 0, M_PI, M_PI };
double   minitialPos[6] = { 10, 10, 10, 10, 0, 0 };
double   mtargetPos[6] = { 10, 10, 10, 10, 50, 100 };
double targetPos[3][6] = { { 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180 },
					 { 90 * M_PI / 180,135 * M_PI / 180,225 * M_PI / 180,180 * M_PI / 180,180 * M_PI / 180,350 * M_PI / 180 },
					 { 160 * M_PI / 180, 150 * M_PI / 180, 30 * M_PI / 180, 30 * M_PI / 180, M_PI, M_PI } };

double      initPos[2] = { 200 * M_PI / 180, 190 * M_PI / 180 };
double      testPos[2] = { 160 * M_PI / 180, 150 * M_PI / 180 };


/*///////////////////////////////////////////////////////////////////////////*/
void init()
{
	_Init_walking_flag = false;
	_Walking_flag = false;
	_program_exit = false;
	_visibility_flag = false;
}

void Fin_Action_comebot(int n)		
{
	switch ( n) {
	
	case 0:
		////////////// 날개 앞뒤로 크게 흔들기
		//////////// left wing

		//if(flag == 0)	// A target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
		//////////// right wing
		//else if(flag == 1) // B target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);

		break;
	
	case 1:
		////////////// 날개 위아래 흔들기
		//////////// left wing

		//if(flag == 0)	// A target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
		//////////// right wing
		//else if(flag == 1) // B target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);

		break;
	case 2:
		////////////// 날개 아래로 떨구기
		//////////// left wing

		//if(flag == 0)	// A target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
		//////////// right wing
		//else if(flag == 1) // B target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);

		break;

	case 3:		
		///////////slow 지느러미를 천천히 앞뒤로

		if (mode_flag == 0)		// A target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)		// B target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(60.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(-60.), simx_opmode_streaming);
		}
		break;
	case 4:		//slow 지느러미를 천천히 앞뒤로

		break;

	default:
	
		break;
	}
}

void Tail_Action_comebot(int n)
{

	switch (n) {
	
	case 0:
		//////////// forward Tail
		simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(200.), simx_opmode_streaming);
		//////////// back Tail
		simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
		break;

	case 1:
		//////꼬리 천천히 흔들기
		if (mode_flag == 0)		// A target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(150.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)		// B target 으로 가기
		{
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(200.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(230.), simx_opmode_streaming);
		}
		break;

	case 2:
		//////////// forward Tail
		simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(200.), simx_opmode_streaming);
		//////////// back Tail
		simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
		break;
	case 3:
		//////////// forward Tail
		simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(200.), simx_opmode_streaming);
		//////////// back Tail
		simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
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
		//////////////////// 보이기
		simxSetObjectIntParameter(clientID,L_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID,R_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// 사라지기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case HAPPY:
		//////////////////// 보이기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// 사라지기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case SAD:
		//////////////////// 보이기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// 사라지기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case ANGRY:
		//////////////////// 보이기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// 사라지기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case BORED:
		//////////////////// 보이기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// 사라지기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	case FUN:
		//////////////////// 보이기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		//////////////////// 사라지기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		pre_EYE = n;
		break;
	default:
		break;
	}
}

void Mode_select(int Eye,int Wing, int Tail, int count)
{
	if (pre_EYE != Eye)
		Eye_Action_comebot(Eye);

	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 3; j++)
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
		for (int j = 0; j < 3; j++)
		{
			Fin_Action_comebot(Wing);
			Tail_Action_comebot(Tail);
			simxSynchronousTrigger(clientID);
		}
	/*	for (int j = 0; j < 1000000; j++)
		{
		}*/
		mode_flag = 0;
		//delay(1000);
		//Sleep(1000);
	}
}

int main(int argc, char* argv[])
{
	printf("Start Program\n");
	clientID = simxStart((simxChar*)"127.0.0.1", 9200, true, true, 5000, 5);

	//// comebot
	simxGetObjectHandle(clientID, "body_tail1", &come_objHandle[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "tail1_tail2", &come_objHandle[1], simx_opmode_oneshot_wait);
	//
	simxGetObjectHandle(clientID, "wing_left_Motor", &come_objHandle[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_right_Motor", &come_objHandle[3], simx_opmode_oneshot_wait);
	////
	simxGetObjectHandle(clientID, "left_wheel_Motor", &come_objHandle[4], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "right_wheel_Motor", &come_objHandle[5], simx_opmode_oneshot_wait);
	////
	simxGetObjectHandle(clientID, "body", &come_objHandle[6], simx_opmode_oneshot_wait);
	//// eye settinh
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
	////Test
	simxGetObjectHandle(clientID, "test_joint", &test_objHandle[0], simx_opmode_oneshot_wait);

	int n = simxSynchronous(clientID, true);
	if (n == simx_return_remote_error_flag) {
		printf("%d\n", n);
	}
	simxInt error = simxStartSimulation(clientID, simx_opmode_oneshot_wait);
	if (error != simx_error_noerror) {
		throw string("Unable to start the simulation");
	}

	//////////////////// 보이기
	simxSetObjectIntParameter(clientID, L_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
	//////////////////// 사라지기
	simxSetObjectIntParameter(clientID, L_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, L_eye_objHandle[BORED], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[BORED], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);

	if (clientID != -1) {
		printf("Connection Established\n");
		init();

		while (simxGetConnectionId(clientID) != -1)
		{
			//readdevice();
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
					break;
				
				case W_COMMAND:
					////////// GO
					simxSetJointTargetVelocity(clientID, come_objHandle[4], -30, simx_opmode_streaming);
					simxSetJointTargetVelocity(clientID, come_objHandle[5], -30, simx_opmode_streaming);
					cout << "Motor Go\n" << endl;
					break;

				case S_COMMAND:
					////////// BACK
					simxSetJointTargetVelocity(clientID, come_objHandle[4], 30, simx_opmode_streaming);
					simxSetJointTargetVelocity(clientID, come_objHandle[5], 30, simx_opmode_streaming);
					cout << "Motor Back\n" << endl;

					break;

				case A_COMMAND:
					////////// LEFT TURN
					simxSetJointTargetVelocity(clientID, come_objHandle[4], -15, simx_opmode_streaming);
					simxSetJointTargetVelocity(clientID, come_objHandle[5], -30, simx_opmode_streaming);
					cout << "Motor Left\n" << endl;
				
				case D_COMMAND:
					////////// RIGHT TURN
					simxSetJointTargetVelocity(clientID, come_objHandle[4], -30, simx_opmode_streaming);
					simxSetJointTargetVelocity(clientID, come_objHandle[5], -15, simx_opmode_streaming);
					cout << "Motor Right\n" << endl;

					break;

				case E_COMMAND:

					////////// joint rotation
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
					////////////// Read Body Position
					//simxGetObjectPosition(clientID, come_objHandle[6], -1, Body_Position, simx_opmode_streaming);
					//printf("Position : %f, %f, %f \n", Body_Position[0], Body_Position[1], Body_Position[2]);
					break;

				case Q_COMMAND:
					cout << " Program End\n" << endl;
					simulation_run = false;
					_program_exit = true;
					break;

				default:
					break;
				}
			}
			
			
			if (simulation_run == true)
			{
				//Wing_Action_comebot(i_mode);
				//Tail_Action_comebot(i_mode);
				simxSynchronousTrigger(clientID);
				/*if (_Init_walking_flag == true) {
					//      initial Pos      
					//num = 0;
					//writedevice(num);
					//////////////////// 보이기
					//simxSetObjectIntParameter(clientID, eye_objHandle[0], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
					/////////////////// joint velocity stop
					//simxSetJointTargetVelocity(clientID, eye_objHandle[1], 0, simx_opmode_oneshot_wait);
					simxSynchronousTrigger(clientID);
				}
				else if (_Walking_flag == true) {
					//      target Pos      
					//num = 1;
					//writedevice(num);
					//////////////////// 사라지기
					simxSetObjectIntParameter(clientID, eye_objHandle[0],sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
					/////////////////// joint velocity go
					//simxSetJointTargetVelocity(clientID, eye_objHandle[1], 100, simx_opmode_oneshot_wait);
					simxSynchronousTrigger(clientID);
				}

				//if (_visibility_flag == true) {
				//	     initial Pos      
				//	simxSetObjectIntParameter(clientID,eye_objHandle[0],10,1, simx_opmode_streaming);
				//	for (int i = 0; i < 6; i++)
				//		simxSetJointTargetPosition(clientID, come_objHandle[i], targetPos[2][i], simx_opmode_streaming);
				//	simxSynchronousTrigger(clientID);
				//}
				//else if (_visibility_flag == false) {
				//	      target Pos      
				//	simxSetObjectIntParameter(clientID, eye_objHandle[0], 10, 10, simx_opmode_streaming);
				//	for (int i = 0; i < 6; i++)
				//		simxSetJointTargetPosition(clientID, come_objHandle[i], targetPos[2][i], simx_opmode_streaming);
				//	simxSynchronousTrigger(clientID);
				//}*/
			}
			
			Sleep(0);
		}
		simxFinish(clientID);
	}
	return 0;
}