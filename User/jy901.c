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
			case 0x50://ʱ�����
				memcpy(&stcTime,&JY_BUF[2],8);
				break;
			case 0x51://���ٶ����
				memcpy(&stcAcc,&JY_BUF[2],8);
				break;
			case 0x52://���ٶ����
				memcpy(&stcGyro,&JY_BUF[2],8);
				break;
			case 0x53://�Ƕ����
				memcpy(&stcAngle,&JY_BUF[2],8);
				break;
			case 0x54://�ų����
				memcpy(&stcMag,&JY_BUF[2],8);
				break;
			case 0x55://�˿�״̬���
				memcpy(&stcDStatus,&JY_BUF[2],8);
				break;
			case 0x56://��ѹ�������
				memcpy(&stcPress,&JY_BUF[2],8);
				break;
			case 0x57://��γ�����
				memcpy(&stcLonLat,&JY_BUF[2],8);
				break;
			case 0x58://�������
				memcpy(&stcGPSV,&JY_BUF[2],8);
				break;
			case 0x59://��Ԫ�����
				break;
			case 0x5A://���Ƕ�λ�������
				break;
			default:
				break;
		}
	}
}
