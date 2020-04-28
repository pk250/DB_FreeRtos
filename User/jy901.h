#ifndef __JY901_H_
#define __JY901_H_

#include "main.h"

typedef struct 
{
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
}STime;
typedef struct 
{
	short a[3];
	short T;
}SAcc;
typedef struct
{
	short w[3];
	short T;
}SGyro;
typedef struct 
{
	short Angle[3];
	short T;
}SAngle;
typedef struct 
{
	short h[3];
	short T;
}SMag;

typedef struct 
{
	short sDStatus[4];
}SDStatus;

typedef struct 
{
	long lPressure;
	long lAltitude;
}SPress;

typedef struct 
{
	long lLon;
	long lLat;
}SLonLat;

typedef struct 
{
	short sGPSHeight;
	short sGPSYaw;
	long lGPSVelocity;
}SGPSV;


extern uint8_t haveHead;
extern STime		stcTime;
extern SAcc 		stcAcc;
extern SGyro 		stcGyro;
extern SAngle 	stcAngle;
extern SMag 		stcMag;
extern SDStatus stcDStatus;
extern SPress 	stcPress;
extern SLonLat 	stcLonLat;
extern SGPSV 		stcGPSV;

void JY_Turn_Char(uint8_t *JY_BUF);


#endif
