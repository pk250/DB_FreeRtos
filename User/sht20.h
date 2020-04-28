#ifndef __SHT20_H_
#define __SHT20_H_

#include "stm32f10x.h"
#include "main.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"

/*SHT20 �豸������غ궨�壬����ֲ�*/
#define SHT20_ADDRESS  0X40
#define SHT20_Measurement_RH_HM  0XE5
#define SHT20_Measurement_T_HM  0XE3
#define SHT20_Measurement_RH_NHM  0XF5
#define SHT20_Measurement_T_NHM  0XF3
#define SHT20_READ_REG  0XE7
#define SHT20_WRITE_REG  0XE6
#define SHT20_SOFT_RESET  0XFE


typedef struct
{

	float tempreture;
	float humidity;

} SHT20_INFO;

extern SHT20_INFO sht20_info;

void Sht20_GetValue(void);

#endif
