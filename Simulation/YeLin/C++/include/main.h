#pragma once
/*
	Vrep Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <opencv2/opencv.hpp>
#include "Data_Format.h"
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
// add
#define E_COMMAND      101
#define I_COMMAND      105


/* VRep function */
void			   vrepinit();
void	     delay(clock_t n);
double dou_angle(double seta);

/* Action function */
void		    forcesensor();
// modify
void Mode_select(int Eye, int Wing, int Tail, int count);
void						   Eye_Action_comebot(int n);
void						  Tail_Action_comebot(int n);

/* flag */
bool   simulation_run = false;
int	   			   _Back_flag,
	  			  _Front_flag,
	  			   _Left_flag,
	  			  _Right_flag,
	  		    _program_exit,
	  		    _Reading_flag,
	         _visibility_flag;
int				     clientID;
int					  	  num;
int						  cnt;
int				   force_flag;
int				   distribute;
// modify
int				   i_mode = 0;
int			  joint_angle = 0;
int				mode_flag = 0;
int				  pre_EYE = 0;


/* general variables */
simxInt	    come_objHandle[7];
simxInt	   L_eye_objHandle[6];
simxInt    R_eye_objHandle[6];
simxInt		     image_Handle;
simxInt		     force_Handle;

simxFloat		   comebodyPos[3] = { 0, };
simxFloat		L_Wing_Joint_X[3] = { dou_angle(-180),dou_angle(-0.00000000033872),dou_angle(90) };
simxFloat		R_Wing_Joint_X[3] = { dou_angle(-180),dou_angle(0.0000000000065138),dou_angle(-90) };
simxFloat  Joint_Orientation_0[3] = { 0,0,0 };
simxFloat Joint_Orientation_90[3] = { 90 * M_PI / 180,0,0 };






/*///////////////////////////////////////////////////////////////////////////*/
void vrepinit() {
	/* VREP Transmission_Init */
	_Back_flag = false;
	_Front_flag = false;
	_program_exit = false;
	_Reading_flag = false;
	_program_exit = false;
}

void delay(clock_t n) {
	clock_t start = clock();
	while (clock() - start < n);
}

double dou_angle(double seta) {	//Radian을 도로 바꾸는 함수
	return seta * M_PI / 180;
}

simxFloat simxF_angle(simxFloat seta) {	//Radian을 도로 바꾸는 함수

	return seta * M_PI / 180;
}





/*
	Action mode Func List
*/
void Eye_Action_comebot(int n) {
	switch (n) {
	case NORM:
		//////////////////// 보이기
		simxSetObjectIntParameter(clientID, L_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
		simxSetObjectIntParameter(clientID, R_eye_objHandle[NORM], sim_objintparam_visibility_layer, 1, simx_opmode_oneshot_wait);
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

void Tail_Action_comebot(int n) {
	switch (n) {
	case 0:
		//////////// forward Tail
		simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(200.), simx_opmode_streaming);
		//////////// back Tail
		simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
		break;

	case 1:	//////꼬리 천천히 흔들기
		if (mode_flag == 0) {			// A target 으로 가기
			//////////// forward Tail
			simxSetJointTargetPosition(clientID, come_objHandle[0], dou_angle(150.), simx_opmode_streaming);
			//////////// back Tail
			simxSetJointTargetPosition(clientID, come_objHandle[1], dou_angle(140.), simx_opmode_streaming);
		}
		else if (mode_flag == 1) {		// B target 으로 가기
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

void Fin_Action_comebot(int n) {
	switch (n) {
	case 0:	////////////// 날개 앞뒤로 크게 흔들기
		//////////// left wing
		//if(flag == 0)	// A target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
		//////////// right wing
		//else if(flag == 1) // B target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		break;

	case 1:	////////////// 날개 위아래 흔들기
		//////////// left wing
		//if(flag == 0)					// A target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
		//////////// right wing
		//else if(flag == 1)			// B target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		break;

	case 2:	////////////// 날개 아래로 떨구기
		//////////// left wing
		//if(flag == 0)					// A target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
		//////////// right wing
		//else if(flag == 1)			// B target 으로 가기
		simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		break;

	case 3:	///////////slow 지느러미를 천천히 앞뒤로
		if (mode_flag == 0)	{			// A target 으로 가기
			//////////// left wing
			simxSetJointTargetPosition(clientID, come_objHandle[2], dou_angle(-40.), simx_opmode_streaming);
			//////////// right wing
			simxSetJointTargetPosition(clientID, come_objHandle[3], dou_angle(40.), simx_opmode_streaming);
		}
		else if (mode_flag == 1) {		// B target 으로 가기
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