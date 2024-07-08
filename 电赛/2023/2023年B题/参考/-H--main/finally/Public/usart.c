#include "usart.h"		 
#include "tftlcd.h"
#include <stdio.h>

/*******************************************************************************
* �� �� ��         : USART1_Init
* ��������		   : USART1��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

void USART1_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10 ; //GPIOA9   tx   ��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
	

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
//		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
//	USART_SendData(USART1,0x99);
}

/*******************************************************************************
* �� �� ��         : USART1_IRQHandler
* ��������		   : USART1�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
uint32_t usarti;
uint8_t r;
void USART1_IRQHandler(void)                	//����1�жϷ������
{
//	static char x=0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		r =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		USART_SendData(USART1,r);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	} 
	usarti++;
	USART_ClearFlag(USART1,USART_FLAG_TC);
}
//u8 ut_rx,ut_i;
//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
////	static char x=0;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
//	{
//		ut_rx=USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		USART_SendData(USART1,ut_rx);ut_i++;
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
//	} 
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//} 	

uint8_t USART_FLAG=0;
extern uint8_t FREA,FREB;
void Usart_Mes(uint8_t kinda,uint8_t frea,uint8_t kindb,uint8_t freb,uint8_t phase)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	USART_SendData(USART1,0x88);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	USART_SendData(USART1,kinda);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	if(frea==0) frea=5;
	USART_SendData(USART1,frea);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	USART_SendData(USART1,kindb);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	if(freb==0) freb=10;
	USART_SendData(USART1,freb);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
//	if(freb%frea!=0) phase=0x01;
//	if(kinda==0x17&&kindb==0x17) phase=0x01;
	if((freb%frea==0)&&(kinda==0x13&&kindb==0x13))
	{
		if(phase==1) USART_SendData(USART1,0x02);
		else if(phase==0xff) USART_SendData(USART1,0xfe);
		else USART_SendData(USART1,phase);
	}
	else USART_SendData(USART1,0x01);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	USART_SendData(USART1,0x99);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
}

//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
////	static char x=0;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
//	{
//		r =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		USART_SendData(USART1,r);
//		
//		if(r==13||r==10){x=0;}
//		else{LCD_ShowChar(20+6*x,20,r,12,0);x++;}
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
//	} 
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//} 

// u8 x,y;
//void USART1_IRQHandler(void)                	//����1�жϷ������
//{	
//	u8 r;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
//	{
//		r =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		USART_SendData(USART1,r);
//		
//		if(r==65)
//		{
//			x=0;
//			if(y==8)
//			{
//				y=0;
//				LCD_ShowChar(20+6*x,20+y*12,r,12,0);
//			}
//			else
//			{
//				y++;
//				LCD_ShowChar(20+6*x,20+y*12,r,12,0);
//			}
//		}
//		else if(r>=128) {
//		LCD_ShowChar(300,400,r,24,0);
//		LCD_ShowNum(300,600,r,3,24);}
//		else
//		{
//			x++;LCD_ShowChar(20+6*x,20+y*12,r,12,0);
//		}
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
//	} 
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//} 	

//u8 x,y,s=0;
//u8 rx[13];
//u8 i=0,j=0;
//u8 r;
//void USART1_IRQHandler(void)                	//����1�жϷ������
//{	
//	
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
//	{
//		r=USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		
//		if(r==65&&s==0) {s=1;i=0;}
//		if(s==1) 
//		{
//			rx[i]=r;i++;
//			if(r==32)
//			{
//				s=0;
////				LCD_ShowString(20,20+(rx[2]-49)*20,100,12,12,rx);
//			}
//		}
////		if(i==12||r==32)
////		{
////			i=0;
////			LCD_ShowString(20,20+(rx[2]-48)*20,100,12,12,rx);
////		}
//		USART_SendData(USART1,r);
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
//	} 
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//} 	

//u8 x,y,s=0;
//u8 rx[8][13];
//u8 i=0,j=0;
//u8 r;
//void USART1_IRQHandler(void)                	//����1�жϷ������
//{	
//	
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
//	{
//		r=USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		
//		if(r==65&&s==0) {s=1;i=0;j++;if(j==8)j=0;}
//		if(s==1) 
//		{
//			rx[j][i]=r;i++;
//			if(r==32)
//			{
//				s=0;
////				LCD_ShowString(20,20+(rx[2]-49)*20,100,12,12,rx);
//			}
//		}
////		if(i==12||r==32)
////		{
////			i=0;
////			LCD_ShowString(20,20+(rx[2]-48)*20,100,12,12,rx);
////		}
//		USART_SendData(USART1,r);
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
//	} 
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//} 	
