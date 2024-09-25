#include "exti.h"
#include "delay.h" 
#include "key.h"
#include "ads1292r.h"

extern uint8_t ADS1292R_receive_flag;
extern uint8_t ADS1292R_reg[12];
extern u16 counter;

//�ⲿ�ж�8�������
void EXTI9_5_IRQHandler(void)
{
    if(EXTI->IMR&EXTI_Line8)//���ݽ����ж�
    {
			EXTI_ClearITPendingBit(EXTI_Line8);
			//counter++;
			ADS1292R_GetValue();  
			//ADS1292_Read_Data((u8 *)ads1292_Cache);//���ݴ浽9�ֽڻ�����
			ADS1292R_receive_flag=1;
    }
}


//�ⲿ�жϳ�ʼ������
//��ʼ��PA8Ϊ�ж�����.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	EXTI_ClearITPendingBit(EXTI_Line8);//����жϱ�־
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);//ѡ��ܽ�
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;						 //ѡ���ж���·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //����Ϊ�ж����󣬷��¼�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						 //�ⲿ�ж�ʹ��
	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;					//ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);

	EXTI->IMR &= ~(EXTI_Line8);//�����ⲿ�ж�


}
