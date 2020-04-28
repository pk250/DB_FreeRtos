/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   用3.5.0版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "main.h"
void Delay_Init(void);
void Write_Version(void);
void Uart4_433_Init(void);
void IWDG_Init(uint8_t pre,uint16_t rlr);

static FATFS fs;
FRESULT res;
FIL fsrc,fdst;
UINT br,bw;

volatile char TF_BUF[1024]={0};
char F_NAME[20]={0};
uint8_t Server_Addr[3]={0x05,0x95,0x40};

volatile uint8_t Send_Flag=0;
uint8_t CMD=0x00,Switch_Buf[56]={0};
const uint8_t BLE_CONNECTED[9]={0x41,0x64,0x76,0x65,0x72,0x74,0x69,0x73,0x69};
const uint8_t BLE_DISCONNETC[9]={0x43,0x6F,0x6E,0x6E,0x65,0x63,0x74,0x65,0x64};

EN_QV en_qv[5]={0};

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,APP_ADDRESS-IAP_ADDRESS);
	Delay_Init();
	usart1_config();
	usart2_config();
	usart3_config();
	usart4_config();
	LED_SET_Init();
	MSD0_SPI_Configuration();
	IWDG_Init(6,10937);
	IIC_Init();
	Write_Version();
	
	res=f_mount(0,&fs);
	sprintf(F_NAME,"0:/tmp.txt");
	FreeRTOS_Init();
	
	osKernelStart();
	
	while(1){
			
	}
	  
}

void IWDG_Init(uint8_t pre,uint16_t rlr){
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //取消寄存器写保护
	IWDG_SetPrescaler(pre);//设置预分频系数 0-6
	IWDG_SetReload(rlr);//设置重装载值
	IWDG_ReloadCounter();  //重装载初值
	IWDG_Enable(); //打开独立看门狗
}

void Uart4_433_Init(void){
	SET_433_OFF;
	UsartPrintf(UART4,"AT+B9600");
	UsartPrintf(UART4,"AT+FU3");
	UsartPrintf(UART4,"AT+P8");
	SET_433_ON;
}

void Write_Version(void){
	FLASH_Unlock();
	FLASH_ErasePage(0x08040000);
	FLASH_ProgramWord(0x08040000,0x08001000);
	FLASH_Lock();
}

void Data_Switch(uint8_t *buf){
	int count_i=0,count_j=0;
	uint8_t dat,Value_H,Value_L;
	for(count_i=0,count_j=0;count_i<112;count_i++){
		switch(buf[count_i]){
			case '0':
			dat = 0x00;
			break;
		 case '1':
			dat = 0x01;
			break;
		 case '2':
			dat = 0x02;
			break;		
		 case '3':
			dat = 0x03;
			break;
		 case '4':
			dat = 0x4;
			break;
		 case '5':
			dat = 0x5;
			break;
		 case '6':
			dat = 0x06;
			break;
		 case '7':
			dat = 0x07;
			break;
		 case '8':
			dat = 0x08;
			break;
		 case '9':
			dat = 0x09;
			break;
		 case '.':
			dat = 0x0a;
			break;
		}
		if(count_i%2==0){
			Value_H=dat<<4;
		}else{
			Value_L=dat;
			Switch_Buf[count_j]=Value_H|Value_L;
			count_j++;
		}
	}
}

void Log(char *buf){
	if(res==FR_OK){
		res = f_open(&fdst, F_NAME, FA_OPEN_ALWAYS | FA_WRITE|FA_READ);
		if(res==FR_OK){
			f_lseek(&fdst,fdst.fsize);
			res=f_write(&fdst,buf,strlen(buf),&bw);
			f_close(&fdst);
		}
	}else{
		#ifdef DEBUG
		UsartPrintf(USART3,"write log error : %d\n\r",res);
		#endif
		res=f_mount(0,&fs);
	}
}

/*********************************************END OF FILE**********************/

