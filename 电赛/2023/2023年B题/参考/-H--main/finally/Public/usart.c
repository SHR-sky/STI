#include "usart.h"		 
#include "tftlcd.h"
#include <stdio.h>

/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

void USART1_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10 ; //GPIOA9   tx   与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
//		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
//	USART_SendData(USART1,0x99);
}

/*******************************************************************************
* 函 数 名         : USART1_IRQHandler
* 函数功能		   : USART1中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
uint32_t usarti;
uint8_t r;
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
//	static char x=0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		r =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		USART_SendData(USART1,r);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	} 
	usarti++;
	USART_ClearFlag(USART1,USART_FLAG_TC);
}
//u8 ut_rx,ut_i;
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//{
////	static char x=0;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		ut_rx=USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
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

//void USART1_IRQHandler(void)                	//串口1中断服务程序
//{
////	static char x=0;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		r =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
//		USART_SendData(USART1,r);
//		
//		if(r==13||r==10){x=0;}
//		else{LCD_ShowChar(20+6*x,20,r,12,0);x++;}
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
//	} 
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//} 

// u8 x,y;
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//{	
//	u8 r;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		r =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
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
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//{	
//	
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		r=USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
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
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//{	
//	
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		r=USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
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
