#pragma once
#ifndef DATA_FORMAT_H
#define DATA_FORMAT_H

#define IP_ADDRESS "192.168.0.26"

#include <string>
#include <string.h>


using namespace std;
/*		   ID	     */
#define CPP			0
#define REI			1
#define STT			2
/* TOUCH SENSOR MODE */
#define DEFAULT		0
#define HEAD		1
#define BODY		2
#define FIN			3
#define TAIL		4
/*	  SENSOR MODE  	 */	
#define OFF			0
#define ON			1
/*	   OLED MODE	 */
#define NORM		0
#define HAPPY		1
#define SAD			2
#define ANGRY		3
#define BORED		4
#define FUN			5
/*		FIN MODE	 */
#define SHAKE		0
#define WING		1
#define DOWN		2
#define SLOW		3
#define NOMOVE		4
/*	   TAIL MODE	 */
#define FAST		0
#define SLOW		1
//#define DOWN		2
#define UP			3
//#define NOMOVE	4
/*FACE DETECT & MOTIONFLAG*/
#define NO			0
#define YES			1


string Data_Packet[9][6] = {
	{"cpp","rei","stt"},					// ID
	{"000"},								// ¹è°íÇÄ Parameter
	{"000"},								// ÇÇ°ïÇÔ Parameter
	{"m10","m11","m12","m13","m14"},		// Touch Sensor 0: Default 1: Head 2: Body 3: Fin 4: Tail
	{"m20","m21"},							// Force Sensor 0: OFF 1: ON
	{"m30","m31"},							// Lift Sensor 0: OFF 1: ON
	{"m40","m41","m42","m43","m44","m45"},	// OLED (0~5) 
	{"m50","m51","m52","m53","m54"},		// FIN (0~3)
	{"m60","m61","m62","m63","m64"},		// TAIL (0~3)
};

string Mode[20] = {
	"002",//NORM		SHAKE		DOWN	0
	"020",//NORM		DOWN		FAST	1
	"030",//NORM		SLOW		FAST	2
	"011",//NORM		WING		SLOW	3
	"100",//HAPPY		SHAKE		FAST	4
	"101",//HAPPY		SHAKE		SLOW	5
	"131",//HAPPY		SLOW		SLOW	6
	"212",//SAD			WING		DOWN	7
	"221",//SAD			DOWN		SLOW	8
	"222",//SAD			DOWN		DOWN	9
	"232",//SAD			SLOW		DOWN	10
	"300",//ANGRY		SHAKE		FAST	11
	"303",//ANGRY		SHAKE		UP		12
	"313",//ANGRY		WING		UP		13
	"323",//ANGRY		DOWN		UP		14
	"421",//BORED		DOWN		SLOW	15
	"422",//BORED		WING		DOWN	16
	"431",//BORED		SLOW		SLOW	17
	"510",//FUN			WING		FAST	18
	"513",//FUN			WING		UP		19
};

/* Packet Global Variables */
int				  Id = CPP;
int		 Hungry_Para = 100;
int		  Tired_Para = 100;
int Touch_Sensor = DEFAULT;
int		Force_Sensor = OFF;
int		 Lift_Sensor = OFF;
int		 Oled_State = NORM;
int		Fin_State = NOMOVE;
int	   Tail_State = NOMOVE;
int		  Face_Detect = NO;
int				Reward = 0;
int		   Mode_Select = 0;
int			  Stt_Data = 0;

/* Global Variables */

void Send_Init_Variable() {
	Touch_Sensor = DEFAULT;
	Force_Sensor	 = OFF;
	Lift_Sensor		 = OFF;
	Reward			   = 0;
}
void Recv_Init_Variable() {
	Stt_Data		   = 0;
	Face_Detect		  = NO;
}

#endif