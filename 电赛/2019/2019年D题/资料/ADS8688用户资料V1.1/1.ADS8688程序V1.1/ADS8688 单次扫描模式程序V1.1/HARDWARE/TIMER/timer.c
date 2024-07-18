#include "timer.h"
#include "ads8688.h"

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  TIM4,TIM_IT_Update ,ENABLE );
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
							 
}

uint16_t value[9]={0,0,0,0,0,0,0,0,0};
uint8_t get_ok=0;//ADC���ݶ�ȡ���

void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		MAN_CH_Mode(MAN_CH_0);					//ѡ��ͨ��
		value[0]=Get_MAN_CH_Mode_Data();//��ȡͨ�����ݣ�
		MAN_CH_Mode(MAN_CH_1);					//ѡ��ͨ��
		value[1]=Get_MAN_CH_Mode_Data();//��ȡͨ������
		MAN_CH_Mode(MAN_CH_2);
		value[2]=Get_MAN_CH_Mode_Data();//��ȡͨ�����ݣ�
		MAN_CH_Mode(MAN_CH_3);
		value[3]=Get_MAN_CH_Mode_Data();//��ȡͨ������
		MAN_CH_Mode(MAN_CH_4);
		value[4]=Get_MAN_CH_Mode_Data();//��ȡͨ�����ݣ�
		MAN_CH_Mode(MAN_CH_5);
		value[5]=Get_MAN_CH_Mode_Data();//��ȡͨ������
		MAN_CH_Mode(MAN_CH_6);
		value[6]=Get_MAN_CH_Mode_Data();//��ȡͨ�����ݣ�
		MAN_CH_Mode(MAN_CH_7);
		value[7]=Get_MAN_CH_Mode_Data();//��ȡͨ������
		MAN_CH_Mode(MAN_AUX);
		value[8]=Get_MAN_CH_Mode_Data();//�ɼ�AUXͨ�� 	AUX��ѹ=value[8]*4096/65535    ��ͨ��ֱ��������ADC�����ڲ�PGA�����뷶ΧΪ0-4.096V��
		get_ok=1;
		//
	}
}







