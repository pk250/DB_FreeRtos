#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define  Tx_usart1_Pin   GPIO_Pin_9
#define  Tx_usart1_Port  GPIOA

#define  Rx_usart1_Pin   GPIO_Pin_10
#define  Rx_usart1_Port  GPIOA

#define  GPIO_usart1_CLK    RCC_APB2Periph_GPIOA
#define  usart1_CLK         RCC_APB2Periph_USART1

#define  Tx_usart2_Pin   GPIO_Pin_2
#define  Tx_usart2_Port  GPIOA

#define  Rx_usart2_Pin   GPIO_Pin_3
#define  Rx_usart2_Port  GPIOA

#define  GPIO_usart2_CLK    RCC_APB2Periph_GPIOA
#define  usart2_CLK         RCC_APB1Periph_USART2

#define  Tx_usart3_Pin   GPIO_Pin_8
#define  Tx_usart3_Port  GPIOD

#define  Rx_usart3_Pin   GPIO_Pin_9
#define  Rx_usart3_Port  GPIOD

#define  GPIO_usart3_CLK    RCC_APB2Periph_GPIOD
#define  usart3_CLK         RCC_APB1Periph_USART3

extern uint8_t RX_BLE_BUF[200];
extern uint8_t RX_BLE_COUNT;
extern uint8_t RNSS_BUF[200];
extern uint8_t RX_RNSS_BUF[200];
extern uint8_t RX_RNSS_COUNT;
extern uint8_t RX_JY_BUF[11];
extern uint8_t RX_JY_COUNT;
extern uint8_t RX_433_BUF[100];
extern uint8_t RX_433_COUNT;
extern uint8_t RX_JM_BUF[100];
extern uint8_t RX_JM_COUNT;

void usart1_config(void);
void usart2_config(void);
void usart3_config(void);
void usart4_config(void);
void Usart_SendStr(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

#endif
