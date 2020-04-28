#include "gpio.h"


void SET_433_Init(void){
	GPIO_InitTypeDef  GPIO_Initstuct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	GPIO_Initstuct.GPIO_Pin   =  SET_433_PIN;
	GPIO_Initstuct.GPIO_Mode  =  GPIO_Mode_Out_PP;
	GPIO_Initstuct.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(SET_433_PORT,&GPIO_Initstuct);
	SET_433_ON;
}

void LED_ERROR_Init(void){
	GPIO_InitTypeDef  GPIO_Initstuct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	GPIO_Initstuct.GPIO_Pin   =  LED_ERROR_PIN;
	GPIO_Initstuct.GPIO_Mode  =  GPIO_Mode_Out_PP;
	GPIO_Initstuct.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(LED_ERROR_PORT,&GPIO_Initstuct);
	LED_ERROR_OFF;
}

void LED_433_Init(void){
	GPIO_InitTypeDef  GPIO_Initstuct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF , ENABLE);
	GPIO_Initstuct.GPIO_Pin   =  LED_433_PIN;
	GPIO_Initstuct.GPIO_Mode  =  GPIO_Mode_Out_PP;
	GPIO_Initstuct.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(LED_433_PORT,&GPIO_Initstuct);
	LED_433_OFF;
}

void LED_BLE_Init(void){
	GPIO_InitTypeDef  GPIO_Initstuct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF , ENABLE);
	GPIO_Initstuct.GPIO_Pin   =  LED_BLE_PIN;
	GPIO_Initstuct.GPIO_Mode  =  GPIO_Mode_Out_PP;
	GPIO_Initstuct.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(LED_BLE_PORT,&GPIO_Initstuct);
	LED_BLE_OFF;
}



void LED_SET_Init(void){
	SET_433_Init();
	LED_ERROR_Init();
	LED_433_Init();
	LED_BLE_Init();
}
