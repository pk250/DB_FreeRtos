#ifndef _DELAY_H_
#define _DELAY_H_

#include "stm32f10x.h"
#include "main.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"

void Delay_Init(void);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);

#endif
