#include "usart.h"

uint8_t RX_BLE_BUF[200]={0};
uint8_t RX_BLE_COUNT=0;
uint8_t RNSS_BUF[200]={0};
uint8_t RX_RNSS_BUF[200]={0};
uint8_t RX_RNSS_COUNT=0;
uint8_t RX_JY_BUF[11]={0};
uint8_t RX_JY_COUNT=0;
uint8_t RX_433_BUF[100]={0};
uint8_t RX_433_COUNT=0;
uint8_t RX_JM_BUF[100]={0};
uint8_t RX_JM_COUNT=0;

void usart1_config(void){
	GPIO_InitTypeDef   GPIO_InitStruct;
	USART_InitTypeDef  	 USART_InitStruct;
	NVIC_InitTypeDef    nvic_initstruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);

	//����TX GPIO��
  GPIO_InitStruct.GPIO_Pin    =  Tx_usart1_Pin;
	GPIO_InitStruct.GPIO_Mode   =  GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed  =  GPIO_Speed_50MHz;
	GPIO_Init(Tx_usart1_Port,&GPIO_InitStruct);
	
	//����RX GPIO��
	GPIO_InitStruct.GPIO_Pin       =  Rx_usart1_Pin;
	GPIO_InitStruct.GPIO_Mode      =  GPIO_Mode_IN_FLOATING;
	GPIO_Init(Rx_usart1_Port,&GPIO_InitStruct);
	
	//����USART����
	USART_InitStruct.USART_BaudRate               = 9600 ;
	USART_InitStruct.USART_WordLength             = USART_WordLength_8b ;
	USART_InitStruct.USART_StopBits               = USART_StopBits_1;
	USART_InitStruct.USART_Parity                 = USART_Parity_No;
	USART_InitStruct.USART_Mode                   = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl    = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStruct);
	
	
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	USART_Cmd(USART1,ENABLE);
  USART_ClearFlag(USART1,USART_FLAG_TXE|USART_FLAG_TC);                          //�����־����������޷�����
	

	
	nvic_initstruct.NVIC_IRQChannel = USART1_IRQn;									//usart1�жϺ�
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;										//�ж�ͨ��ʹ��
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 6;							//��ռ�ж����ȼ�
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;									//���ж����ȼ�
	NVIC_Init(&nvic_initstruct);

}

void usart2_config(void){
	GPIO_InitTypeDef   GPIO_InitStruct;
	USART_InitTypeDef  	 USART_InitStruct;
	NVIC_InitTypeDef    nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	//����TX GPIO��
  GPIO_InitStruct.GPIO_Pin    =  Tx_usart2_Pin;
	GPIO_InitStruct.GPIO_Mode   =  GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed  =  GPIO_Speed_50MHz;
	GPIO_Init(Tx_usart2_Port,&GPIO_InitStruct);
	
	//����RX GPIO��
	GPIO_InitStruct.GPIO_Pin       =  Rx_usart2_Pin;
	GPIO_InitStruct.GPIO_Mode      =  GPIO_Mode_IN_FLOATING;
	GPIO_Init(Rx_usart2_Port,&GPIO_InitStruct);
	
	//����USART����
	USART_InitStruct.USART_BaudRate               = 9600 ;
	USART_InitStruct.USART_WordLength             = USART_WordLength_8b ;
	USART_InitStruct.USART_StopBits               = USART_StopBits_1;
	USART_InitStruct.USART_Parity                 = USART_Parity_No;
	USART_InitStruct.USART_Mode                   = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl    = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStruct);
	
	
	
	USART_Cmd(USART2,ENABLE);
  USART_ClearFlag(USART2,USART_FLAG_TXE|USART_FLAG_TC);                          //�����־����������޷�����
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	
	nvic_initstruct.NVIC_IRQChannel = USART2_IRQn;									//usart1�жϺ�
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;										//�ж�ͨ��ʹ��
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 5;							//��ռ�ж����ȼ�
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;									//���ж����ȼ�
	NVIC_Init(&nvic_initstruct);

}

void usart3_config(void){
	GPIO_InitTypeDef   GPIO_InitStruct;
	USART_InitTypeDef  	 USART_InitStruct;
	NVIC_InitTypeDef    nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE); 
	
	//����TX GPIO��
  GPIO_InitStruct.GPIO_Pin    =  Tx_usart3_Pin;
	GPIO_InitStruct.GPIO_Mode   =  GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed  =  GPIO_Speed_50MHz;
	GPIO_Init(Tx_usart3_Port,&GPIO_InitStruct);
	
	//����RX GPIO��
	GPIO_InitStruct.GPIO_Pin       =  Rx_usart3_Pin;
	GPIO_InitStruct.GPIO_Mode      =  GPIO_Mode_IN_FLOATING;
	GPIO_Init(Rx_usart3_Port,&GPIO_InitStruct);
	
	//����USART����
	USART_InitStruct.USART_BaudRate               = 115200 ;
	USART_InitStruct.USART_WordLength             = USART_WordLength_8b ;
	USART_InitStruct.USART_StopBits               = USART_StopBits_1;
	USART_InitStruct.USART_Parity                 = USART_Parity_No;
	USART_InitStruct.USART_Mode                   = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl    = USART_HardwareFlowControl_None;
	USART_Init(USART3,&USART_InitStruct);
	
	
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	USART_Cmd(USART3,ENABLE);
  USART_ClearFlag(USART3,USART_FLAG_TXE|USART_FLAG_TC);                          //�����־����������޷�����

	
	nvic_initstruct.NVIC_IRQChannel = USART3_IRQn;									//usart1�жϺ�
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;										//�ж�ͨ��ʹ��
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 6;							//��ռ�ж����ȼ�
	nvic_initstruct.NVIC_IRQChannelSubPriority = 1;									//���ж����ȼ�
	NVIC_Init(&nvic_initstruct);

}

void usart4_config(void)
{
	GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	
	//Pc10	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin =GPIO_Pin_10;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio_initstruct);
	
	//Pc11	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_11;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio_initstruct);
	  
	
	usart_initstruct.USART_BaudRate = 9600;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//���պͷ���
	usart_initstruct.USART_Parity = USART_Parity_No;									//��У��
	usart_initstruct.USART_StopBits = USART_StopBits_1;								//1λֹͣλ
	usart_initstruct.USART_WordLength = USART_WordLength_8b;							//8λ����λ
	USART_Init(UART4, &usart_initstruct);
	
	USART_Cmd(UART4, ENABLE);														//ʹ�ܴ���
	USART_ClearFlag(UART4,USART_FLAG_TXE| USART_FLAG_RXNE|USART_FLAG_TC);               //�����־����������޷�����
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	
	nvic_initstruct.NVIC_IRQChannel = UART4_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 6;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_initstruct);
} 

void Usart_SendStr(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//��������
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
	}

}

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{

	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsprintf((char *)UsartPrintfBuf, fmt, ap);							//��ʽ��
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);  //�ȴ�֡������ɱ�־λ
	}

}
