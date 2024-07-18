#include "timer.h"
#include "ads8688.h"

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  TIM4,TIM_IT_Update ,ENABLE );
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
							 
}

uint16_t value[9]={0,0,0,0,0,0,0,0,0};
uint8_t get_ok=0;//ADC数据读取完成

void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		MAN_CH_Mode(MAN_CH_0);					//选择通道
		value[0]=Get_MAN_CH_Mode_Data();//读取通道数据，
		MAN_CH_Mode(MAN_CH_1);					//选择通道
		value[1]=Get_MAN_CH_Mode_Data();//读取通道数据
		MAN_CH_Mode(MAN_CH_2);
		value[2]=Get_MAN_CH_Mode_Data();//读取通道数据，
		MAN_CH_Mode(MAN_CH_3);
		value[3]=Get_MAN_CH_Mode_Data();//读取通道数据
		MAN_CH_Mode(MAN_CH_4);
		value[4]=Get_MAN_CH_Mode_Data();//读取通道数据，
		MAN_CH_Mode(MAN_CH_5);
		value[5]=Get_MAN_CH_Mode_Data();//读取通道数据
		MAN_CH_Mode(MAN_CH_6);
		value[6]=Get_MAN_CH_Mode_Data();//读取通道数据，
		MAN_CH_Mode(MAN_CH_7);
		value[7]=Get_MAN_CH_Mode_Data();//读取通道数据
		MAN_CH_Mode(MAN_AUX);
		value[8]=Get_MAN_CH_Mode_Data();//采集AUX通道 	AUX电压=value[8]*4096/65535    此通道直接输入至ADC，无内部PGA，输入范围为0-4.096V。
		get_ok=1;
		//
	}
}







