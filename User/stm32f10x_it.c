/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"
#include "usart.h"
#include "bd_rnss.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "jy901.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None

void SVC_Handler(void)
{
}
  */
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None

void PendSV_Handler(void)
{
}
  */
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None

void SysTick_Handler(void)
{
}
  */
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void USART1_IRQHandler(void){
	uint8_t Clear=Clear;

	BaseType_t xHigherPriorityTaskWoken;
	
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
			RX_RNSS_BUF[RX_RNSS_COUNT++]=USART_ReceiveData(USART1);
	}else if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET){
			Clear=USART1->SR;
			Clear=USART1->DR;
			/*
			实现消息队列，将接收的数据回传至任务
			*/
			if(xRnssReceiveHandler!=NULL){
				vTaskNotifyGiveFromISR(xRnssReceiveHandler,&xHigherPriorityTaskWoken);
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			}else{
				vTaskNotifyGiveFromISR(xStartHandler,&xHigherPriorityTaskWoken);
			}
			memcpy(RNSS_BUF,RX_RNSS_BUF,RX_RNSS_COUNT);
			RX_RNSS_COUNT=0;
			
			memset(RX_RNSS_BUF,0,sizeof(RX_RNSS_BUF));
	}
		if(USART_GetITStatus(USART1,USART_FLAG_ORE)==SET){
			RX_RNSS_BUF[RX_RNSS_COUNT++]=USART_ReceiveData(USART1);
			USART_ClearFlag(USART1,USART_FLAG_ORE);
	}
		
}
void USART2_IRQHandler(void){
	uint16_t rx_data = 0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
		USART_ITConfig(USART2,USART_IT_RXNE,DISABLE); 
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		rx_data=USART_ReceiveData(USART2);
		USART_SendData(USART1,rx_data);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); 
	}
}

void USART3_IRQHandler(void){
	uint8_t Clear=Clear;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET){
		RX_BLE_BUF[RX_BLE_COUNT++]=USART_ReceiveData(USART3);
	}else if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET){
			Clear=USART3->SR;
			Clear=USART3->DR;
		if(RX_BLE_COUNT<9){
			/*
			实现消息队列，将接收的数据回传至任务
			*/
			if(RX_BLE_BUF[0]=='#'&&RX_BLE_BUF[1]=='#'&&RX_BLE_BUF[2]=='#'){
				CMD=RX_BLE_BUF[3];
			}else if(RX_BLE_BUF[0]=='C'&&RX_BLE_BUF[1]=='M'&&RX_BLE_BUF[2]=='D'){
				/*
				*实现其它指令功能
				*/
			}
		}else{
			if(RX_BLE_BUF[0]==0x41&&RX_BLE_BUF[1]==0x64&&RX_BLE_BUF[2]==0x76&&
				RX_BLE_BUF[3]==0x65&&RX_BLE_BUF[4]==0x72&&RX_BLE_BUF[5]==0x74&&
			RX_BLE_BUF[6]==0x69&&RX_BLE_BUF[7]==0x73&&RX_BLE_BUF[8]==0x69){
				LED_BLE_ON;
			}else if(RX_BLE_BUF[0]==0x43&&RX_BLE_BUF[1]==0x6F&&RX_BLE_BUF[2]==0x6E&&
				RX_BLE_BUF[3]==0x6E&&RX_BLE_BUF[4]==0x65&&RX_BLE_BUF[5]==0x63&&
			RX_BLE_BUF[6]==0x74&&RX_BLE_BUF[7]==0x65&&RX_BLE_BUF[8]==0x64){
				LED_BLE_OFF;
			}else if(RX_BLE_BUF[1]==0x41&&RX_BLE_BUF[2]==0x64&&RX_BLE_BUF[3]==0x76&&
				RX_BLE_BUF[4]==0x65&&RX_BLE_BUF[5]==0x72&&RX_BLE_BUF[6]==0x74&&
			RX_BLE_BUF[7]==0x69&&RX_BLE_BUF[8]==0x73&&RX_BLE_BUF[9]==0x69){
				LED_BLE_ON;
			}else if(RX_BLE_BUF[1]==0x43&&RX_BLE_BUF[2]==0x6F&&RX_BLE_BUF[3]==0x6E&&
				RX_BLE_BUF[4]==0x6E&&RX_BLE_BUF[5]==0x65&&RX_BLE_BUF[6]==0x63&&
			RX_BLE_BUF[7]==0x74&&RX_BLE_BUF[8]==0x65&&RX_BLE_BUF[9]==0x64){
				LED_BLE_OFF;
			}
			
		}
		Usart_SendStr(USART3,RX_BLE_BUF,RX_BLE_COUNT);
		RX_BLE_COUNT=0;
	}
}

void UART4_IRQHandler(void){
	uint8_t tmp=0;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
  {
		tmp=USART_ReceiveData(UART4);
		if(RX_JY_COUNT>0&&RX_JY_COUNT<11){
			RX_JY_BUF[RX_JY_COUNT++]=tmp;
		}
		if(tmp==0x55&RX_JY_COUNT==0){
			RX_JY_BUF[RX_JY_COUNT++]=tmp;
		}else if(RX_JY_COUNT>=11){
			RX_JY_COUNT=0;
			JY_Turn_Char(RX_JY_BUF);
			memset(RX_JY_BUF,0,sizeof(RX_JY_BUF));
		}
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
  }
	
	USART_ClearITPendingBit(UART4,USART_IT_ORE);
}
/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
