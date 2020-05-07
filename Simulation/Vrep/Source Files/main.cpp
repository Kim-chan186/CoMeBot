/*
	TCP include Files
*/
#include "winsock2.h" // include before <windows.h>
#include <conio.h>
#define BUFSIZE 1024
void ErrorHandling(char* message);
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
	Thread Include Files
*/
#include <thread>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include "typeinfo"
using namespace std;


extern "C" {
#include "extApi.h"
#include <v_repConst.h>

	//#include "extApiPlatform.h" 
}

#define M_PI         (3.141592653589793238462643383279501884197169399375105829974944)
#define TAB_COMMAND      9
#define Q_COMMAND      113
#define I_COMMAND      105
#define W_COMMAND      119
#define PORT         9200


void init();
void readdevice();
void writedevice();
void recv_socket(SOCKET sock);

bool simulation_run = false;
int _Init_walking_flag,
_Walking_flag,
_program_exit;
int clientID;


/* General Variables */
simxInt   g_objHandle[6];
simxInt come_objHandle[6];
simxInt test_objHandle[2];
/*/////////////////////////// */

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
	int m_iNC;
	mutex m_oMtx;
	condition_variable m_oCondVar;

	SafeCondVar();

	void notifyOne();
	void wait();
};
SafeCondVar::SafeCondVar()
	: m_iNC(0), m_oMtx(), m_oCondVar()
{
}
void SafeCondVar::notifyOne(){
	lock_guard<mutex> oL(m_oMtx);
	++m_iNC;
	m_oCondVar.notify_one();
}
void SafeCondVar::wait()
{
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
SafeCondVar Send_CondVar;
SafeCondVar Recv_CondVar;
/*/////////////////////////// */

double   initialPos[6] = { 200 * M_PI / 180, 190 * M_PI / 180, 0, 0, M_PI, M_PI };
double targetPos[3][6] = { { 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180, 90 * M_PI / 180 },
					 { 90 * M_PI / 180,135 * M_PI / 180,225 * M_PI / 180,180 * M_PI / 180,180 * M_PI / 180,350 * M_PI / 180 },
					 { 160 * M_PI / 180, 150 * M_PI / 180, 30 * M_PI / 180, 30 * M_PI / 180, 0, 0 } };
double      initPos[2] = { 200 * M_PI / 180, 190 * M_PI / 180 };
double      testPos[2] = { 160 * M_PI / 180, 150 * M_PI / 180 };

/*///////////////////////////////////////////////////////////////////////////*/
void init()
{
	_Init_walking_flag = false;
	_Walking_flag = false;
	_program_exit = false;
}
/*///////////////////////////////////////////////////////////////////////////*/
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
/*///////////////////////////////////////////////////////////////////////////*/
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
/*///////////////////////////////////////////////////////////////////////////*/
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
			//mode_buff = Mode[ibuff].c_str();
			ibuff = atoi(Mode[ibuff].c_str());
			//mMutex.lock();
			Oled_State = ibuff / 100;
			Fin_State = (ibuff % 100) / 10;
			Tail_State = ibuff % 10;
			//mMutex.unlock();
			printf("\nrecv: %s\n", &buff);
			//printf("%d %d %d\n", Oled_State, Fin_State, Tail_State);
			//cout << typeid(ibuff).name() << endl;
			//cout << typeid(Oled_State).name() << endl;
			//cout << "SERVER> " << string(buff, 0, bytesReceived) << endl;
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
	//lock.unlock();
	scanf_s("%d", &Id); //0 cpp 1 rei 2 stt
	//Id = 0;
 	strcpy(cMsg, Data_Packet[0][Id].c_str()); //id : string -> char[]
	send(sock, cMsg, strlen(cMsg), 0); // id setting in Server
	
	while (1) {
		//unique_lock<mutex> lock(mMutex);
		//mCondVar.wait(lock, []()->bool {return MotionEndFlag; });
		Send_CondVar.wait();
		
		packet =
			Data_Packet[0][Id]				+ ","
			+ to_string(Hungry_Para)		+ ","
			+ to_string(Tired_Para)			+ ","
			+ Data_Packet[3][Touch_Sensor]	+ ","
			+ Data_Packet[4][Force_Sensor]	+ ","
			+ Data_Packet[5][Lift_Sensor]	+ ","
			+ Data_Packet[6][Oled_State]	+ ","
			+ Data_Packet[7][Fin_State]		+ ","
			+ Data_Packet[8][Tail_State]	+ ","
			+ to_string(Oled_State)			
			+ to_string(Fin_State)			
			+ to_string(Tail_State)			+ ","
			+ to_string(Face_Detect)		+ ","
			+ to_string(Reward);			
		strcpy(cMsg, packet.c_str()); //packet : string -> char[]
		/*Packet 전송*/
		send(sock, cMsg, strlen(cMsg), 0);
		printf("\nSend_Packet: %s\n", cMsg);
	
		Sleep(10);
	}
}
void motion_thread() {
	int distribute = 0;
	while(1){
		Recv_CondVar.wait();
		//unique_lock<mutex> lock(mMutex);
		distribute = stoi(Mode[Mode_Select]);
		//Oled_State = distribute / 100;
		//Fin_State = (distribute % 100) / 10;
		//Tail_State = distribute % 10;
		printf("Motion : %d %d %d \n", Oled_State, Fin_State, Tail_State);
		/*Motion Function*/
		Sleep(2000);
		/*Motion End*/
		
		Send_CondVar.notifyOne();
		//mCondVar.notify_all();		
	}
}

int main(int argc, char* argv[])
{
	/* Main Variables */
	//char cMsg[] = "";
	//string packet;
	//int i = 0;
	Hungry_Para = 50;

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
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //생성된 소켓을 바탕으로 서버에 연결요청을 하고 있다
		ErrorHandling("connect() error!");

	/* Thread_Init */
	thread recv;
	thread tran;
	thread para;	
	thread vrep;
	
	para = thread(Parameter_Thread);
	vrep = thread(motion_thread);
	tran = thread(tran_socket, hSocket);
	recv = thread(recv_socket, hSocket);
	
	///* ID Change */
	//scanf_s("%d", &Id); //0 cpp 1 rei 2 stt

	//strcpy(cMsg, Data_Packet[0][Id].c_str()); //id : string -> char[]
	//send(hSocket, cMsg, strlen(cMsg), 0); // id setting in Server
	//while (1) {		
	//	packet = 
	//		 Data_Packet[0][Id]				+ "," 
	//		+to_string(Hungry_Para)			+ "," 
	//		+to_string(Tired_Para)			+ ","
	//		+Data_Packet[3][Touch_Sensor]	+ ","
	//		+Data_Packet[4][Force_Sensor]	+ ","
	//		+Data_Packet[5][Lift_Sensor]	+ ","
	//		+Data_Packet[6][Oled_State]		+ ","
	//		+Data_Packet[7][Fin_State]		+ ","
	//		+Data_Packet[8][Tail_State]		+ ","
	//		+to_string(Oled_State)
	//		+to_string(Fin_State)
	//		+to_string(Tail_State)			+ ","
	//		+to_string(Reward);

	//	strcpy(cMsg, packet.c_str()); //packet : string -> char[]
	//	//Packet 전송
	//	send(hSocket, cMsg, strlen(cMsg), 0);

	//	printf("\nSend_Packet: %s\n", cMsg);
	//	Sleep(1000);
	//}
	recv.join();
	para.join();
	tran.join();
	vrep.join();
	closesocket(hSocket); //소켓 라이브러리 해제
	WSACleanup();


	//////////////////////////////////////////////////////////////////////////
	printf("Start Program\n");
	clientID = simxStart((simxChar*)"127.0.0.1", 9200, true, true, 5000, 5);


	/*                get handle                 */
	/*
	simxGetObjectHandle(clientID, "Jaco_joint1", &g_objHandle[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "Jaco_joint2", &g_objHandle[1], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "Jaco_joint3", &g_objHandle[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "Jaco_joint4", &g_objHandle[3], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "Jaco_joint5", &g_objHandle[4], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "Jaco_joint6", &g_objHandle[5], simx_opmode_oneshot_wait);
	*/

	//// comebot
	simxGetObjectHandle(clientID, "body_tail1", &come_objHandle[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "tail1_tail2", &come_objHandle[1], simx_opmode_oneshot_wait);
	//
	simxGetObjectHandle(clientID, "wing_left_Motor", &come_objHandle[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_right_Motor", &come_objHandle[3], simx_opmode_oneshot_wait);
	////
	//simxGetObjectHandle(clientID, "left_wheel_Motor", &come_objHandle[4], simx_opmode_oneshot_wait);
	//simxGetObjectHandle(clientID, "right_wheel_Motor", &come_objHandle[5], simx_opmode_oneshot_wait);
	////


	int n = simxSynchronous(clientID, true);
	if (n == simx_return_remote_error_flag) {
		printf("%d\n", n);
	}
	simxInt error = simxStartSimulation(clientID, simx_opmode_oneshot_wait);
	if (error != simx_error_noerror) {
		throw string("Unable to start the simulation");
	}



	if (clientID != -1) {
		printf("Connection Established\n");
		init();

		while (simxGetConnectionId(clientID) != -1)
		{
			readdevice();
			if (kbhit())
			{
				int key = _getch();
				printf("%d\t", key);

				switch (key) {
				case I_COMMAND:
					cout << " Initial Pos\n" << endl;
					_Init_walking_flag = true;
					_Walking_flag = false;
					break;

				case W_COMMAND:
					cout << " Put\n" << endl;
					_Walking_flag = true;
					_Init_walking_flag = false;
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
				if (_Init_walking_flag == true) {
					/*      initial Pos      */
					writedevice();
					simxSynchronousTrigger(clientID);
				}
				else if (_Walking_flag == true) {
					/*      target Pos      */
					writedevice();
					simxSynchronousTrigger(clientID);
				}
			}
			Sleep(0);
		}
		simxFinish(clientID);
	}
	return 0;	
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
