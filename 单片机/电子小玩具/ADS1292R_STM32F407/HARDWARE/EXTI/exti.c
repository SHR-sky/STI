#include "exti.h"
#include "delay.h" 
#include "key.h"
#include "ads1292r.h"

extern uint8_t ADS1292R_receive_flag;
extern uint8_t ADS1292R_reg[12];
extern u16 counter;

//外部中断8服务程序
void EXTI9_5_IRQHandler(void)
{
    if(EXTI->IMR&EXTI_Line8)//数据接收中断
    {
			EXTI_ClearITPendingBit(EXTI_Line8);
			//counter++;
			ADS1292R_GetValue();  
			//ADS1292_Read_Data((u8 *)ads1292_Cache);//数据存到9字节缓冲区
			ADS1292R_receive_flag=1;
    }
}


//外部中断初始化程序
//初始化PA8为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	EXTI_ClearITPendingBit(EXTI_Line8);//清除中断标志
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);//选择管脚
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;						 //选择中断线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //设置为中断请求，非事件请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						 //外部中断使能
	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;					//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);

	EXTI->IMR &= ~(EXTI_Line8);//屏蔽外部中断


}
