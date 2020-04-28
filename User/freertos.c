#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

#include "queue.h"
#include "list.h"
#include "gpio.h"
#include "sht20.h"
#include "jy901.h"
#include "bd_rnss.h"

void vRnssReceiveTask(void *pvParameters);
void vRnssSendTask(void *pvParameters);
void vStart_Task(void *pvParameters);
void vDataSaveTask(void *pvParameters);
void vTXSQTask(void *pvParameters);

uint8_t ICXX_BUF[22]={0};
uint8_t ZJXX_BUF[21]={0};
uint8_t SJXX_BUF[18]={0};
uint8_t GNPX_BUF[32]={0};
uint8_t FKXX_BUF[16]={0};
uint8_t i=0,TXSQ_COUNT=0;

uint8_t TXSQ_BUF[97]={0};
uint8_t TXSQ_Short_Buf[112]={0};

static unsigned char uStartToPass,uRnssToPass,uRnssSendToPass,uDataSaveToPass,uTXSQToPass;
xTaskHandle xStartHandler,xRnssReceiveHandler,xRnssSendHandler,xDataSaveHandler,xTXSQHandler;

void FreeRTOS_Init(void){
	
	xTaskCreate(vStart_Task,"xStartHandler",256,&uStartToPass,0,&xStartHandler);
}

void vStart_Task(void *pvParameters){
	while(1){	
		if(ulTaskNotifyTake(pdTRUE,0)==1&&CRC_Check(RNSS_BUF,21,RNSS_BUF[21])==0x01&&RNSS_Cmp(RNSS_BUF)==ICXX&&(RNSS_BUF[7]|RNSS_BUF[8]|RNSS_BUF[9])!=0x00){
			taskENTER_CRITICAL();
			memcpy(ICXX_BUF,RNSS_BUF,22);
			memset(RNSS_BUF,0,sizeof(RNSS_BUF));
			RNSS_ICJC_GET(ICXX_BUF);
			sprintf(F_NAME,"0:/%d.txt",(((int)icxx_type.user_address[0]<<16)|((int)icxx_type.user_address[1]<<8)|(int)icxx_type.user_address[2]));
			
			xTaskCreate(vRnssReceiveTask,"vRnssTask",500,&uRnssToPass,3,&xRnssReceiveHandler);
			xTaskCreate(vRnssSendTask,"vRnssSendTask",500,&uRnssSendToPass,1,&xRnssSendHandler);
			xTaskCreate(vDataSaveTask,"vDataSaveTask",500,&uDataSaveToPass,2,&xDataSaveHandler);
			xTaskCreate(vTXSQTask,"vTXSQTask",128,&uTXSQToPass,4,&xTXSQHandler);
			vTaskDelete(xStartHandler);
			LED_ERROR_OFF;
			taskEXIT_CRITICAL();
		}else{
			UsartPrintf(USART3, "{\'Error Code\':%d}\r\n",ICJC_ERROR);
			LED_ERROR_ON;
		}
		vTaskDelay(1000);
		RNSS_CMD(icjc,sizeof(icjc));
		vTaskDelay(500);
	}
}

void osKernelStart(void){
	vTaskStartScheduler();
}

void TXSQ_Handle(){
	TXSQ_BUF[0]='$';
	TXSQ_BUF[1]='T';
	TXSQ_BUF[2]='X';
	TXSQ_BUF[3]='S';
	TXSQ_BUF[4]='Q';
	TXSQ_BUF[5]=0x00;
	TXSQ_BUF[6]=0x61;
	memcpy(TXSQ_BUF+7,&icxx_type.user_address,3);
	TXSQ_BUF[10]=0x46;
	memcpy(TXSQ_BUF+11,&Server_Addr,3);
	TXSQ_BUF[14]=0x02;
	TXSQ_BUF[15]=0x74;
	TXSQ_BUF[16]=0x00;
	TXSQ_BUF[94]=CMD;
	TXSQ_BUF[96]=Get_CRC(TXSQ_BUF,96);
	taskENTER_CRITICAL();
	RNSS_CMD(TXSQ_BUF,97);
	#ifdef DEBUG
	UsartPrintf(USART3,"Send TXSQ\r\n");
	#endif
	taskEXIT_CRITICAL();
}

void GNPX_Handle(){
	uint8_t j=0,n=0;
	int wTmp1=0,wTmp2=0;
	uint8_t Pressure;
	short Altitude;
	eTaskState e;
	if(gnpx_type.bd_sum!=0x00){
			en_qv[i].latitude=(gnpx_type.latitude[3]+gnpx_type.latitude[2]*100+gnpx_type.latitude[1]*10000+gnpx_type.latitude[0]*1000000);
			en_qv[i].longitude=(gnpx_type.longitude[3]+gnpx_type.longitude[2]*100+gnpx_type.longitude[1]*10000+gnpx_type.longitude[0]*1000000);
			memcpy(en_qv[i].speed,gnpx_type.speed,2);
			memcpy(en_qv[i++].w,stcGyro.w,6);
				if(i>=5){
					for(j=0;j<5;j++){
						wTmp2=abs((int)en_qv[j].w[0])+abs((int)en_qv[j].w[1])+abs((int)en_qv[j].w[2]);
						if(wTmp1<wTmp2){
							wTmp1=wTmp2;
							n=j;
						}
					}
					sprintf((char *)TXSQ_Short_Buf+(TXSQ_COUNT*17),"%08.0f%09.0f",en_qv[n].latitude,en_qv[n].longitude);
					TXSQ_COUNT++;
					if(TXSQ_COUNT>=6){
						sprintf((char *)TXSQ_Short_Buf+102,"%04.0f%02.0f%02.0f",(double)((gnpx_type.speed[0]*256+gnpx_type.speed[1])/10),(sht20_info.tempreture+30),sht20_info.humidity);
						Data_Switch(TXSQ_Short_Buf);
						memcpy(TXSQ_BUF+17,Switch_Buf,55);
						memcpy(TXSQ_BUF+72,&stcAcc,6);
						memcpy(TXSQ_BUF+78,&stcGyro,6);
						memcpy(TXSQ_BUF+84,&stcAngle,6);
						Pressure=stcPress.lPressure/1000;
						TXSQ_BUF[90]=Pressure;
						Altitude=stcPress.lAltitude/100;
						memcpy(TXSQ_BUF+91,&Altitude,2);
						if(stcPress.lAltitude>0){
							TXSQ_BUF[95]=0x10;
						}else{
							TXSQ_BUF[95]=0x00;
						}
						e=eTaskGetState(xTXSQHandler);
						if(e==eReady||e==eBlocked){
							if(xTaskNotifyGive(xTXSQHandler)==pdPASS){
								taskYIELD();
							}else{
								Log("xTaskNotifyGive\r\n");
							}
						}
						TXSQ_COUNT=0;
					}
					i=0;
				}
	}
}

void vRnssReceiveTask(void *pvParameters){
	RNSSStatus_TypeDef rnssStatus,fkxxStatus;
	eTaskState e;
	uint8_t Ble_Cmd[4]={'#','#','#',0x00};
	for(;;){
			if(ulTaskNotifyTake(pdTRUE,0xFFFF)==1){
				rnssStatus=RNSS_Cmp(RNSS_BUF);
				switch(rnssStatus){
				case ZJXX:
						if(CRC_Check(RNSS_BUF,20,RNSS_BUF[20])){
							memcpy(ZJXX_BUF,RNSS_BUF,21);
							RNSS_XTZJ_GET(ZJXX_BUF);
						}
						break;
				case SJXX:
						if(CRC_Check(RNSS_BUF,17,RNSS_BUF[17])){
							memcpy(SJXX_BUF,RNSS_BUF,18);
							RNSS_SJSC_GET(SJXX_BUF);
						}
						break;
				case GNPX:
						if(CRC_Check(RNSS_BUF,31,RNSS_BUF[31])){
							memcpy(GNPX_BUF,RNSS_BUF,32);
							RNSS_GNPS_GET(GNPX_BUF);
							GNPX_Handle();
							#ifdef DEBUG
							UsartPrintf(USART3, "Send gnpx queue\r\n");
							#endif
						}
						break;
				case FKXX:
						if(CRC_Check(RNSS_BUF,31,RNSS_BUF[31])){
							memcpy(FKXX_BUF,RNSS_BUF,16);
							fkxxStatus=RNSS_FKXX_GET(FKXX_BUF);
							if(fkxxStatus==ICXX){
								CMD=0x00;
								#ifdef DEBUG
								UsartPrintf(USART3,"Send TXSQ ok\r\n");
								#endif
							}else if(fkxxStatus==ICXX_F){
								e=eTaskGetState(xTXSQHandler);
								if(e==eReady||e==eBlocked){
									if(xTaskNotifyGive(xTXSQHandler)==pdPASS){
										taskYIELD();
									}else{
									Log("xTaskNotifyGive\r\n");
									}
								}
								#ifdef DEBUG
								UsartPrintf(USART3,"Send TXSQ fail\r\n");
								#endif
							}
						}
						break;
				case TXXX:
						if(CRC_Check(RNSS_BUF,20,RNSS_BUF[20])){
							Ble_Cmd[3]=RNSS_BUF[18];
							UsartPrintf(USART3,(char *)Ble_Cmd);
						}
						break;
				default:break;
			}
			memset(RNSS_BUF,0,sizeof(RNSS_BUF));
		}
	}
}

void vRnssSendTask(void *pvParameters){
	eTaskState e;
	for(;;){
		vTaskDelay(450);
		RNSS_CMD(gnps,sizeof(gnps));
		LED_433_ON;
		vTaskDelay(400);
		RNSS_CMD(sjsc,sizeof(sjsc));
		LED_433_OFF;
		vTaskDelay(400);
		Sht20_GetValue();
		vTaskDelay(50);
		RNSS_CMD(xtzj,sizeof(xtzj));
		LED_433_ON;
		UsartPrintf(USART2,"USER_ID:%d\r\n",(((int)icxx_type.user_address[0]<<16)|((int)icxx_type.user_address[1]<<8)|(int)icxx_type.user_address[2]));
		IWDG_ReloadCounter();
		//UsartPrintf(USART3,"APP1 Flash:%08X\r\n",*(__IO uint32_t*)0x08040000);
		/**发起任务通知，进行数据TF卡及蓝牙发送
		*/
		e=eTaskGetState(xDataSaveHandler);
		if(e==eReady||e==eBlocked){
			if(xTaskNotifyGive(xDataSaveHandler)==pdPASS){
				taskYIELD();
			}else{
				Log("xTaskNotifyGive\r\n");
			}
		}
	}
}

void vDataSaveTask(void *pvParameters){
	for(;;){
		if(ulTaskNotifyTake(pdTRUE,0xFFFF)==1){
			taskENTER_CRITICAL();
			if(sjxx_type.year>2000&&gnpx_type.bd_sum!=0x00){
				//gnpx_type.bd_sum=0x00;
				LED_ERROR_OFF;
				memset((void *)TF_BUF,'\0',sizeof(TF_BUF));
				sprintf((char *)TF_BUF+strlen((char *)TF_BUF),"{\"Time\":\"%d:%d:%d\",",sjxx_type.hour,sjxx_type.minute,sjxx_type.second);
				sprintf((char *)TF_BUF+strlen((char *)TF_BUF),"\"T\":%.2f,\"H\":%.2f,",sht20_info.tempreture,sht20_info.humidity);
//				sprintf((char *)TF_BUF+strlen((char *)TF_BUF),"\'ACC\':[%f,%f,%f],",(float)stcAcc.a[0]/32768*16,(float)stcAcc.a[1]/32768*16,(float)stcAcc.a[2]/32768*16);
//				sprintf((char *)TF_BUF+strlen((char *)TF_BUF),"\'Gyro\':[%f,%f,%f],",(float)stcGyro.w[0]/32768*2000,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);
//				sprintf((char *)TF_BUF+strlen((char *)TF_BUF),"\'Angle\':[%f,%f,%f],",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
				sprintf((char *)TF_BUF+strlen((char *)TF_BUF),"\"Pres\":%ld,\"Alti\":%.2f,",stcPress.lPressure,(float)stcPress.lAltitude/100);
				sprintf((char *)TF_BUF+strlen((char *)TF_BUF),"\"dB\":[%d,%d,%d,%d,%d,%d],",zjxx_type.one_level,zjxx_type.two_level,zjxx_type.three_level,zjxx_type.four_level,zjxx_type.five_level,zjxx_type.six_level);
				sprintf((char *)TF_BUF+strlen((char *)TF_BUF),"\"E\":%.6f,\"N\":%.6f,\"V\":%.2f}\r\n",gnpx_type.lon,gnpx_type.lat,(double)((gnpx_type.speed[0]*256+gnpx_type.speed[1])/10));
				//Log((char *)TF_BUF);
				UsartPrintf(USART3,(char*)TF_BUF);
			}else{
				LED_ERROR_ON;
			}
			taskEXIT_CRITICAL();
		}
	}
}

void vTXSQTask(void *pvParameters){
	uint8_t LED_Flash=0,Flag_Count=0;
	for(;;){
		if(ulTaskNotifyTake(pdTRUE,0xFFFF)==1){
			while(1){
				if(Send_Flag==0x01||Flag_Count>40){
					#ifdef DEBUG
					UsartPrintf(USART3,"Send_Flag:%d Flag_Count:%d\r\n",Send_Flag,Flag_Count);
					#endif
					Flag_Count=0;
					break;
				}
				vTaskDelay(50);
				Flag_Count++;
			}
			TXSQ_Handle();
			for(LED_Flash=0;LED_Flash<4;LED_Flash++){
				LED_ERROR_ON;
				vTaskDelay(100);
				LED_ERROR_OFF;
				vTaskDelay(100);
			}
		}
	}
}

