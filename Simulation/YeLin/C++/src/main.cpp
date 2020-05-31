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
double dou_angle(double seta)	//Radian�� ���� �ٲٴ� �Լ�
{
	return seta * M_PI / 180;
}
simxFloat simxF_angle(simxFloat seta)	//Radian�� ���� �ٲٴ� �Լ�
{
	return seta * M_PI / 180;
}

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
	prop = 2;	// �� �������
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

	prop = 1;	// �� ���̱�
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

simxFloat wantpos[3] = { 0, 0, 1.5907 };
void Mode_select(int Eye, int Wing, int Tail, int count)
{
	//simxCallScriptFunction(clientID, "camera", sim_scripttype_childscript, "gotoposition", 1, &come_objHandle[6], 3, wantpos, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
	//simxSetObjectPosition(clientID, come_objHandle[6], -1, wantpos, simx_opmode_oneshot_wait);
	if (pre_EYE != Eye)
		Eye_Action_comebot(Eye);

	Fin_Angle_comebot(Wing);
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 5; j++) {
			Fin_Action_comebot(Wing);
			Tail_Action_comebot(Tail);
			simxSynchronousTrigger(clientID);
		}
		mode_flag = 1;

		for (int j = 0; j < 5; j++) {
			Fin_Action_comebot(Wing);
			Tail_Action_comebot(Tail);
			simxSynchronousTrigger(clientID);
		}
		mode_flag = 0;
	}

	for (int j = 0; j < 2; j++) {
		init_Joint_Angle_comebot();
		simxSynchronousTrigger(clientID);
	}
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
				printf("\n ** Vrep ���� ���� !! \n ");
				continue;
			}

			// fps, ���� ������� !!

			Mat img(resolution[0], resolution[1], CV_8UC3, comeimage);
			if (img.empty()) {
				printf("\n ** Vrep img is empty !! \n ");
				continue;
			}
			cvtColor(img, img2, cv::COLOR_RGB2BGR);

			// ��, ��, good, bad ������ ������ �κ�
			bool* bp = GUI::get_flag();
			//for (int i = 0; i < 4; i++)
			//	cout << bp[i] << " ";
			//printf("\n");

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
void motion_control_thread() {
	int distribute = 0;
	printf("Start Program\n");
	clientID = simxStart((simxChar*)"127.0.0.1", 9200, true, true, 5000, 5);

	/* Tail Init */
	simxGetObjectHandle(clientID, "body_tail1",		   &come_objHandle[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "tail1_tail2",	   &come_objHandle[1], simx_opmode_oneshot_wait);
	/* Wing Init */
	simxGetObjectHandle(clientID, "wing_left_Motor",   &come_objHandle[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "wing_right_Motor",  &come_objHandle[3], simx_opmode_oneshot_wait);
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

	int flag = 0;
	while (1) {

		distribute = stoi(Mode[Mode_Select]);
		printf("Motion : %d %d %d \n", Oled_State, Fin_State, Tail_State);

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
				//vrep_parameter();

				Mode_select(Oled_State, Fin_State, Tail_State, 2);
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


	closesocket(hSocket); //���� ���̺귯�� ����
	WSACleanup();
}