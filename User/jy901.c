#include "jy901.h"

uint8_t haveHead=0;

STime		stcTime;
SAcc 		stcAcc;
SGyro 		stcGyro;
SAngle 	stcAngle;
SMag 		stcMag;
SDStatus stcDStatus;
SPress 	stcPress;
SLonLat 	stcLonLat;
SGPSV 		stcGPSV;

void JY_Turn_Char(unsigned char *JY_BUF){
	if(JY_BUF[0]!=0x55){
		return;
	}else{
		switch(JY_BUF[1]){
			case 0x50://时间输出
				memcpy(&stcTime,&JY_BUF[2],8);
				break;
			case 0x51://加速度输出
				memcpy(&stcAcc,&JY_BUF[2],8);
				break;
			case 0x52://角速度输出
				memcpy(&stcGyro,&JY_BUF[2],8);
				break;
			case 0x53://角度输出
				memcpy(&stcAngle,&JY_BUF[2],8);
				break;
			case 0x54://磁场输出
				memcpy(&stcMag,&JY_BUF[2],8);
				break;
			case 0x55://端口状态输出
				memcpy(&stcDStatus,&JY_BUF[2],8);
				break;
			case 0x56://气压海拔输出
				memcpy(&stcPress,&JY_BUF[2],8);
				break;
			case 0x57://经纬度输出
				memcpy(&stcLonLat,&JY_BUF[2],8);
				break;
			case 0x58://地速输出
				memcpy(&stcGPSV,&JY_BUF[2],8);
				break;
			case 0x59://四元素输出
				break;
			case 0x5A://卫星定位精度输出
				break;
			default:
				break;
		}
	}
}
