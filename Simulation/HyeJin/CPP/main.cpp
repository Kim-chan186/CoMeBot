#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "main.h"
#include "Data_Format.h"
#include "VRep_GUI.h"
#include "Data_Transmission.h"

using namespace std;
using namespace cv;
using namespace chrono;


/* OpenCV Variables */
simxUChar*	   comeimage = 0;
simxInt		   resolution[2];


/*////////////////////////////////[START Function]//////////////////////////////// */
/* Motion Function */
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

void init_Joint_Angle_comebot()
{
	simxSetJointTargetPosition(clientID, come_objHandle[0], initialPos[0], simx_opmode_streaming);
	simxSetJointTargetPosition(clientID, come_objHandle[1], initialPos[1], simx_opmode_streaming);
	//////wing joint inside
	simxSetJointTargetPosition(clientID, come_wing_jointHandle[1], initialPos[2], simx_opmode_streaming);
	simxSetJointTargetPosition(clientID, come_wing_jointHandle[3], initialPos[3], simx_opmode_streaming);
	/////wing joint outside
	simxSetJointTargetPosition(clientID, come_wing_jointHandle[0], initialPos[4], simx_opmode_streaming);
	simxSetJointTargetPosition(clientID, come_wing_jointHandle[2], initialPos[5], simx_opmode_streaming);
}
void Fin_Action_comebot(int n)
{
	switch (n) {

	case 0:
		////////////// 날개 앞뒤로 크게 흔들기		
		//////////// left wing
		if (mode_flag == 0)		// A target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[1], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)		// B target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[1], dou_angle(100.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[3], dou_angle(-100.), simx_opmode_streaming);
		}
		break;

	case 1:
		////////////// 날개 위아래 흔들기
		if (mode_flag == 0)		// A target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[0], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[2], dou_angle(120.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)		// B target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[0], dou_angle(60.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[2], dou_angle(20.), simx_opmode_streaming);
		}
		break;
	case 2:
		////////////// 날개 아래로 떨구기

	//////////// left wing
		simxSetJointTargetPosition(clientID, come_wing_jointHandle[0], dou_angle(90.), simx_opmode_streaming);
		//////////// right wing
		simxSetJointTargetPosition(clientID, come_wing_jointHandle[2], dou_angle(-10.), simx_opmode_streaming);

		break;

	case 3:
		///////////slow 지느러미를 천천히 앞뒤로

		if (mode_flag == 0)		// A target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[1], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1)		// B target 으로 가기
		{
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[1], dou_angle(30.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_wing_jointHandle[3], dou_angle(-30.), simx_opmode_streaming);
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
	prop = 2;	// 눈 사라지기
	switch (pre_EYE) {
	case NORM:
		simxCallScriptFunction(clientID, "normal_left", sim_scripttype_childscript, "normal_left", 1, &L_eye_objHandle[NORM], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "normal_right", sim_scripttype_childscript, "normal_right", 1, &R_eye_objHandle[NORM], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		break;
	case HAPPY:
		simxCallScriptFunction(clientID, "happy_left", sim_scripttype_childscript, "happy_left", 1, &L_eye_objHandle[HAPPY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "happy_right", sim_scripttype_childscript, "happy_right", 1, &R_eye_objHandle[HAPPY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		break;
	case SAD:
		simxCallScriptFunction(clientID, "sad_left", sim_scripttype_childscript, "sad_left", 1, &L_eye_objHandle[SAD], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "sad_right", sim_scripttype_childscript, "sad_right", 1, &R_eye_objHandle[SAD], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		break;
	case ANGRY:
		simxCallScriptFunction(clientID, "angry_left", sim_scripttype_childscript, "angry_left", 1, &L_eye_objHandle[ANGRY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "angry_right", sim_scripttype_childscript, "angry_right", 1, &R_eye_objHandle[ANGRY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		break;
	case BORED:
		simxCallScriptFunction(clientID, "boring_left", sim_scripttype_childscript, "boring_left", 1, &L_eye_objHandle[BORED], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "boring_right", sim_scripttype_childscript, "boring_right", 1, &R_eye_objHandle[BORED], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		break;
	case FUN:
		simxCallScriptFunction(clientID, "fun_left", sim_scripttype_childscript, "fun_left", 1, &L_eye_objHandle[FUN], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "fun_right", sim_scripttype_childscript, "fun_right", 1, &R_eye_objHandle[FUN], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[pre_EYE], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
		break;
	default:
		break;
	}

	prop = 1;	// 눈 보이기
	switch (n) {
	case NORM:
		simxCallScriptFunction(clientID, "normal_left", sim_scripttype_childscript, "normal_left", 1, &L_eye_objHandle[NORM], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "normal_right", sim_scripttype_childscript, "normal_right", 1, &R_eye_objHandle[NORM], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		break;
	case HAPPY:
		simxCallScriptFunction(clientID, "happy_left", sim_scripttype_childscript, "happy_left", 1, &L_eye_objHandle[HAPPY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "happy_right", sim_scripttype_childscript, "happy_right", 1, &R_eye_objHandle[HAPPY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		break;
	case SAD:
		simxCallScriptFunction(clientID, "sad_left", sim_scripttype_childscript, "sad_left", 1, &L_eye_objHandle[SAD], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "sad_right", sim_scripttype_childscript, "sad_right", 1, &R_eye_objHandle[SAD], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[SAD], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		break;
	case ANGRY:
		simxCallScriptFunction(clientID, "angry_left", sim_scripttype_childscript, "angry_left", 1, &L_eye_objHandle[ANGRY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "angry_right", sim_scripttype_childscript, "angry_right", 1, &R_eye_objHandle[ANGRY], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		break;
	case BORED:
		simxCallScriptFunction(clientID, "boring_left", sim_scripttype_childscript, "boring_left", 1, &L_eye_objHandle[BORED], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "boring_right", sim_scripttype_childscript, "boring_right", 1, &R_eye_objHandle[BORED], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[BORED], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		break;
	case FUN:
		simxCallScriptFunction(clientID, "fun_left", sim_scripttype_childscript, "fun_left", 1, &L_eye_objHandle[FUN], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxCallScriptFunction(clientID, "fun_right", sim_scripttype_childscript, "fun_right", 1, &R_eye_objHandle[FUN], 1, &prop, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, L_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[FUN], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		break;
	default:
		break;
	}
	pre_EYE = n;
}

void Mode_select(int Eye, int Wing, int Tail, int count)
{
	if (pre_EYE != Eye)
		Eye_Action_comebot(Eye);

	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 3; j++) {
			Fin_Action_comebot(Wing);
			Tail_Action_comebot(Tail);
			simxSynchronousTrigger(clientID);
		}
		mode_flag = 1;

		for (int j = 0; j < 3; j++) {
			Fin_Action_comebot(Wing);
			Tail_Action_comebot(Tail);
			simxSynchronousTrigger(clientID);
		}
		mode_flag = 0;
	}

	for (int j = 0; j < 3; j++) {
		init_Joint_Angle_comebot();
		simxSynchronousTrigger(clientID);
	}
}
/*////////////////////////////////[END Function]//////////////////////////////// */


/*////////////////////////////////[START Thread]//////////////////////////////// */
void getimage()
{
	Mat img2 = Mat(Size(600, 600), CV_8UC3, Scalar::all(0));
	while (1) {
		
		GUI::img_vrep = &img2;
		GUI::init();
		for (int i = 0; GUI::waitKeySuper(1); i++) {
			int retval = simxGetVisionSensorImage(clientID, image_Handle, resolution, &comeimage, 0, simx_opmode_streaming);
			if (retval != simx_return_ok) {
				//printf("\n ** Vrep 연결 문제 !! \n ");
				continue;
			}
			// fps, 언제 끊기는지 !!

			Mat img(resolution[0], resolution[1], CV_8UC3, comeimage);
			if (img.empty() | (resolution[0]<0) | (resolution[1]<0) | resolution[0] > 10000 | resolution[1] > 10000) {
				printf("\n ** Vrep img is empty !! \n ");
				continue;
			}
			//img2 = img;
			cvtColor(img, img2, cv::COLOR_RGB2BGR);

			GUI::GazeSync(Hungry_Para, Tired_Para);

			// 밥, 잠, good, bad 순으로 보내는 부분
			//for (int i = 0; i < 4; i++)
			//	cout << bp[i] << " ";
			//printf("\n");


			//vrep_parameter();

			// 받는 부분
			bool cam_flag = false;
			GUI::cam(0, 0, cam_flag);
			//printf("%s, %s, %s. %s\n",Oled_State, Fin_State, Tail_State, Stt_Data);
			int act[4] = { Oled_State,Fin_State,Tail_State,Stt_Data };
			GUI::action(act);
			GUI::show();
		}
	}
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
			Tired_Para = 100;
	
		Sleep(5000);
	}
}
void motion_control_thread() {
	
	printf("Start Program\n");
	clientID = simxStart((simxChar*)"127.0.0.1", 9200, true, true, 5000, 5);

	/* Tail Init */
	simxGetObjectHandle(clientID, "body_tail1",		   &come_objHandle[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "tail1_tail2",	   &come_objHandle[1], simx_opmode_oneshot_wait);
	/* Wing Init */
	simxGetObjectHandle(clientID, "wing_left_Motor1", &come_wing_jointHandle[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_left_Motor2", &come_wing_jointHandle[1], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_right_Motor1", &come_wing_jointHandle[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_right_Motor2", &come_wing_jointHandle[3], simx_opmode_oneshot_wait);
	/* Moter Init */
	simxGetObjectHandle(clientID, "left_wheel_Motor",  &come_objHandle[4], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "right_wheel_Motor", &come_objHandle[5], simx_opmode_oneshot_wait);
	/* Body Init */
	simxGetObjectHandle(clientID, "body",			   &come_objHandle[6], simx_opmode_oneshot_wait);
	/* Sensor Init */
	simxGetObjectHandle(clientID, "camera",			   &image_Handle, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "shock",			   &force_Handle, simx_opmode_oneshot_wait);
	/* Eye Setting */
	simxGetObjectHandle(clientID, "normal_left",	   &L_eye_objHandle[NORM], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "normal_right",	   &R_eye_objHandle[NORM], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "happy_left",		   &L_eye_objHandle[HAPPY], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "happy_right",	   &R_eye_objHandle[HAPPY], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sad_left",		   &L_eye_objHandle[SAD], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sad_right",		   &R_eye_objHandle[SAD], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "angry_left",		   &L_eye_objHandle[ANGRY], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "angry_right",	   &R_eye_objHandle[ANGRY], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "boring_left",	   &L_eye_objHandle[BORED], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "boring_right",	   &R_eye_objHandle[BORED], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "fun_left",		   &L_eye_objHandle[FUN], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "fun_right",		   &R_eye_objHandle[FUN], simx_opmode_oneshot_wait);


	int n = simxSynchronous(clientID, true);
	if (n == simx_return_remote_error_flag) {
		printf("%d\n", n);
	}
	simxInt error = simxStartSimulation(clientID, simx_opmode_oneshot_wait);
	if (error != simx_error_noerror) {
		throw string("Unable to start the simulation");
	}
	
	/* NORM Eye Visible */
	simxSetObjectIntParameter(clientID, L_eye_objHandle[NORM],  sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[NORM],  sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
	/* NORM Eye InVisible */
	simxSetObjectIntParameter(clientID, L_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[HAPPY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, L_eye_objHandle[BORED], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[BORED], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, L_eye_objHandle[SAD],   sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[SAD],   sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, L_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[ANGRY], sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, L_eye_objHandle[FUN],   sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	simxSetObjectIntParameter(clientID, R_eye_objHandle[FUN],   sim_objintparam_visibility_layer, 256, simx_opmode_oneshot_wait);
	
	int distribute = 0;
	int flag = 0;
	while (1) {

		/*Motion Function*/
		if (clientID != -1) {
			printf("Connection Established\n");
			vreptcp_init();
			while (simxGetConnectionId(clientID) != -1) {
				if (flag == 0) {
					Recv_CondVar.wait();
					flag = 1;
				}
				Recv_CondVar.wait();


				distribute = stoi(Mode[Mode_Select]);
				Oled_State = distribute / 100;
				Fin_State = (distribute % 100)/10;
				Tail_State = distribute % 10;
				printf("Motion : %d %d %d \n", Oled_State, Fin_State, Tail_State);

				Mode_select(Oled_State, Fin_State, Tail_State, 2);
				vrep_parameter();
				

				Send_CondVar.notifyOne();
				if (simulation_run == true) {	
					
					simxSynchronousTrigger(clientID);
				}
			}
			simxFinish(clientID);
		}
		/*Motion End*/
	}
}
/*////////////////////////////////[END Thread]//////////////////////////////// */


int main(int argc, const char* argv[])
{

	/* VRep & TCP Init */
	vreptcp_init();

	/* Thread Init */
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