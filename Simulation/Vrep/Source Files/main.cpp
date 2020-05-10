#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "main.h"
#include "Data_Format.h"
using namespace std;
using namespace cv;

/* fps variables */
chrono::system_clock::time_point	tpStart, tpEnd;
double	 dTime = 0;
int cont_while = 0;
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
void SafeCondVar::notifyOne() {
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



/*///////////////////////////////////////////////////////////////////////////*/
void writedevice(int n)
{
	/*
		Motion Thread
		int
	*/
	switch (n) {
	case 1:		front();	break;
	case 2:		left();		break;
	case 3:		right();	break;
	default:	back();		break;
	}
}
/*///////////////////////////////////////////////////////////////////////////*/
simxFloat force_cur = -0.182;
void forcesensor()
{
	simxReadForceSensor(clientID, force_Handle, NULL, &force_cur, NULL, simx_opmode_streaming);
}
/*///////////////////////////////////////////////////////////////////////////*/
simxFloat getposition[3] = { 0, };
void getpos()
{
	simxGetObjectPosition(clientID, body_Handle, -1, getposition, simx_opmode_blocking);
}
/*///////////////////////////////////////////////////////////////////////////*/
simxUChar*	   comeimage = 0;
simxInt		   resolution[2];
char _fps[5];	  string fps;
void getimage()
{
	cv::namedWindow("vrep", CV_WINDOW_AUTOSIZE);
	while (1) {//for (int time = 0; time < 1000; time++) {
		int retval = simxGetVisionSensorImage(clientID, image_Handle, resolution, &comeimage, 0, simx_opmode_streaming);
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
		cv::waitKey(27);
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
void motion_thread() {
	int distribute = 0;
	while (1) {
		Recv_CondVar.wait();
		distribute = stoi(Mode[Mode_Select]);
		//Oled_State = distribute / 100;
		//Fin_State = (distribute % 100) / 10;


			//Tail_State = distribute % 10;
		printf("Motion : %d %d %d \n", Oled_State, Fin_State, Tail_State);
		/*Motion Function*/
		Sleep(2000);
		/*Motion End*/

		Send_CondVar.notifyOne();
	}
}

void vrepandact()
{
	//////////////////////////////////////////////////////////////////////////
	clientID = simxStart((simxChar*)"127.0.0.1", 9200, true, true, 5000, 5);

	/*                get handle                 */
	simxGetObjectHandle(clientID, "body", &body_Handle, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "body_tail1", &come_objHandle[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "tail1_tail2", &come_objHandle[1], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_left_Motor", &come_objHandle[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_right_Motor", &come_objHandle[3], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "left_wheel_Motor", &come_objHandle[4], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "right_wheel_Motor", &come_objHandle[5], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "camera", &image_Handle, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "shock", &force_Handle, simx_opmode_oneshot_wait);


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
			if (kbhit())
			{
				int key = _getch();
				printf("%d\t", key);

				switch (key) {
				case S_COMMAND:
					std::cout << "Back\n";
					_Back_flag = true;		_Front_flag = false;	_Left_flag = false;		_Right_flag = false;
					break;
				case W_COMMAND:
					std::cout << "Front\n";
					_Front_flag = true;		_Back_flag = false;		_Left_flag = false;		_Right_flag = false;
					break;
				case A_COMMAND:
					std::cout << "LEF\n";
					_Left_flag = true;		_Front_flag = false;	_Back_flag = false;		_Right_flag = false;
					break;
				case D_COMMAND:
					std::cout << "RIG\n";
					_Right_flag = true;		_Left_flag = false;		_Front_flag = false;	_Back_flag = false;
					break;
				case R_COMMAND:
					std::cout << "Read\n";
					if (_Reading_flag)		_Reading_flag = false;	else					_Reading_flag = true;
					break;

				case TAB_COMMAND:
					if (simulation_run) {
						simulation_run = false;
						std::cout << "Stop\n";
					}
					else {
						simulation_run = true;
						std::cout << "Go\n";
					}
					break;
				default:
					break;
				}
			}



			// 만약 충격이나 들어올림 플래그가 서면 따로 저장해두고 그 패킷 보내기







			if (simulation_run == true)
			{
				if (_Back_flag == true) {
					/*      initial Pos      */
					num = 0;
					writedevice(num);
					simxSynchronousTrigger(clientID);
				}
				else if (_Front_flag == true) {
					num = 1;
					writedevice(num);
					simxSynchronousTrigger(clientID);
				}
				else if (_Left_flag == true) {
					num = 2;
					writedevice(num);
					simxSynchronousTrigger(clientID);
				}
				else if (_Right_flag == true) {
					num = 3;
					writedevice(num);
					simxSynchronousTrigger(clientID);
				}
				if (_Reading_flag == true) {
					forcesensor();			getpos();
					simxSynchronousTrigger(clientID);
					if ((force_cur < -0.1805) & (force_cur > -0.1835))	force_flag = 0;
					else												force_flag = 1;
					printf("\t%d", force_flag);//printf("%lf\n", force_cur);
					printf("\t%f\t%f\t%f\n", getposition[0], getposition[1], getposition[2]);
				}
			}
			Sleep(0);
		}
		simxStopSimulation(clientID, simx_opmode_oneshot);
		simxFinish(clientID);
	}
}

int main(int argc, char* argv[])
{
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


	printf("Start Program\n");
	/* Thread_Init */
	thread recv;
	thread tran;
	thread para;
	thread vrep;
	thread recvimg;
	thread vrepcon;

	para = thread(Parameter_Thread);
	vrep = thread(motion_thread);
	tran = thread(tran_socket, hSocket);
	recv = thread(recv_socket, hSocket);
	recvimg = thread(getimage);
	vrepcon = thread(vrepandact);

	recv.join();
	para.join();
	tran.join();
	vrep.join();
	recvimg.join();
	vrepcon.join();
	closesocket(hSocket); //소켓 라이브러리 해제
	WSACleanup();

	return 0;
}