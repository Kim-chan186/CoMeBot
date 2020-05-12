#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "main.h"
#include "Data_Transmission.h"
#include "VRep_GUI.h"

using namespace std;
using namespace cv;
using namespace chrono;

/* fps variables */
system_clock::time_point	tpStart, tpEnd;
double	 dTime = 0;
int cont_while = 0;

/* Mutex Variables */
mutex mMutex;
condition_variable mCondVar;


/*///////////////////////////////////////////////////////////////////////////*/
simxFloat force_cur = -0.182;
void forcesensor()
{
	simxReadForceSensor(clientID, force_Handle, NULL, &force_cur, NULL, simx_opmode_streaming);
}
void getpos()
{
	simxGetObjectPosition(clientID, come_objHandle[6], -1, bodyPos, simx_opmode_blocking);
}
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

// ?
simxInt test_objHandle[2];


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
		tpStart = system_clock::now();
		dTime = 1.0 / duration_cast<milliseconds>(tpEnd - tpStart).count();
		tpEnd = tpStart;
		//if (++cont_while == 1000) {
		//	cont_while = 0;
		//	tpEnd = system_clock::now();
		//	dTime = 100000.0 / duration_cast<milliseconds>(tpEnd - tpStart).count();
		//	tpStart = tpEnd;
		//}

		_itoa_s(dTime + 0.1, _fps, 5, 10);	fps = _fps;
		Mat img(resolution[0], resolution[1], CV_8UC3, comeimage);
		flip(img, img, 0);
		cvtColor(img, img, cv::COLOR_RGB2BGR);
		putText(img, "FPS:" + fps, Point(430, 30), FONT_HERSHEY_PLAIN, 1.2, Scalar(255, 230, 100));
		cv::imshow("vrep", img);
		cv::waitKey(27);
	}
}
void Mode_select(int Eye, int Wing, int Tail, int count)
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
		mode_flag = 1;

		for (int j = 0; j < 3; j++)
		{
			Fin_Action_comebot(Wing);
			Tail_Action_comebot(Tail);
			simxSynchronousTrigger(clientID);
		}
		mode_flag = 0;
	}
}





/*///////////////////////////////////////////////////////////////////////////*/
void motion_thread() {
	while (1) {
		Recv_CondVar.wait();
		distribute = stoi(Mode[Mode_Select]);
		printf("Motion : %d %d %d \n", Oled_State, Fin_State, Tail_State);


		if (clientID != -1) {
			printf("Connection Established\n");
			init();
			while (simxGetConnectionId(clientID) != -1) {
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

					case W_COMMAND:	//////// GO
						simxSetJointTargetVelocity(clientID, come_objHandle[4], -10, simx_opmode_streaming);
						simxSetJointTargetVelocity(clientID, come_objHandle[5], -10, simx_opmode_streaming);
						cout << "Motor Go\n" << endl;
						Send_CondVar.notifyOne();
						break;

					case S_COMMAND:	//////// BACK
						simxSetJointTargetVelocity(clientID, come_objHandle[4], 10, simx_opmode_streaming);
						simxSetJointTargetVelocity(clientID, come_objHandle[5], 10, simx_opmode_streaming);
						cout << "Motor Back\n" << endl;
						Send_CondVar.notifyOne();
						break;

					case A_COMMAND:	//////// LEFT TURN
						simxSetJointTargetVelocity(clientID, come_objHandle[4], -5, simx_opmode_streaming);
						simxSetJointTargetVelocity(clientID, come_objHandle[5], -10, simx_opmode_streaming);
						cout << "Motor Left\n" << endl;
						Send_CondVar.notifyOne();
						break;

					case D_COMMAND:	//////// RIGHT TURN
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
						if (joint_angle == 0) {
							joint_angle = 1;
							simxSetObjectOrientation(clientID, test_objHandle[0], -1, Joint_Orientation_90, simx_opmode_streaming);
						}
						else if (joint_angle == 1) {
							joint_angle = 0;
							simxSetObjectOrientation(clientID, test_objHandle[0], -1, Joint_Orientation_0, simx_opmode_streaming);
						}
						//////////// Read Body Position
						simxGetObjectPosition(clientID, come_objHandle[6], -1, bodyPos, simx_opmode_streaming);
						printf("Position : %f, %f, %f \n", bodyPos[0], bodyPos[1], bodyPos[2]);
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

				if (simulation_run == true) {
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
			simxStopSimulation(clientID, simx_opmode_oneshot);
			simxFinish(clientID);
		}
	}
}





int main(int argc, char* argv[])
{
	Hungry_Para = 50;
	r
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



	/*///////////////////////////////////////////////////////////////////////////*/
	clientID = simxStart((simxChar*)"127.0.0.1", 9200, true, true, 5000, 5);

	/* Tail, Wing, Motor, Body Setting */
	simxGetObjectHandle(clientID, "body_tail1", &come_objHandle[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "tail1_tail2", &come_objHandle[1], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_left_Motor", &come_objHandle[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_right_Motor", &come_objHandle[3], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "left_wheel_Motor", &come_objHandle[4], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "right_wheel_Motor", &come_objHandle[5], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "body", &come_objHandle[6], simx_opmode_oneshot_wait);
	/*			Sensor Setting		   */
	simxGetObjectHandle(clientID, "camera", &image_Handle, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "shock", &force_Handle, simx_opmode_oneshot_wait);
	/*			  Eye Setting		   */
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



	/*///////////////////////////////////////////////////////////////////////////*/
	printf("Start Program\n");

	/* Thread_Init */
	thread recv;
	thread tran;
	thread para;
	thread recvimg;
	thread vrep;

	para = thread(Parameter_Thread);
	tran = thread(tran_socket, hSocket);
	recv = thread(recv_socket, hSocket);
	recvimg = thread(getimage);
	vrep = thread(motion_thread);

	recv.join();
	para.join();
	tran.join();
	recvimg.join();
	vrep.join();

	closesocket(hSocket); //소켓 라이브러리 해제
	WSACleanup();

	return 0;
}