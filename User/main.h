#ifndef __MAIN_H_
#define __MAIN_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f10x.h"
#include "usart.h"
#include "gpio.h"
#include "iic.h"
#include "delay.h"
#include "ff.h"
#include "tf.h"

#define IAP_ADDRESS 0x8000000
#define APP_ADDRESS 0x8001000

typedef enum{
	ICJC_ERROR=0x00,//未接天线或未插北斗用户卡
	GNPS_ERROR=0x01,//未获取到经纬度，可能在无北斗信号区域
	FMOUNT_ERROR=0x02//SD卡挂载失败，未插卡或该卡不支持SPI协议
}Sys_Status;

typedef struct{
	double longitude;
	double latitude;
	uint8_t speed[2];
	short w[3];
}EN_QV;

extern xTaskHandle xStartHandler,xRnssReceiveHandler;
//extern static FATFS fs;
extern FRESULT res;
extern FIL fsrc,fdst;
extern UINT br,bw;
extern volatile char TF_BUF[1024];
extern char	F_NAME[20];
extern const uint8_t BLE_CONNECTED[9];
extern const uint8_t BLE_DISCONNETC[9];
extern EN_QV en_qv[5];
extern uint8_t Switch_Buf[56];
extern uint8_t CMD;
extern uint8_t Server_Addr[3];
extern volatile uint8_t Send_Flag;

void FreeRTOS_Init(void);
void osKernelStart(void);
void Log(char *buf);
void Data_Switch(uint8_t *buf);

#endif
