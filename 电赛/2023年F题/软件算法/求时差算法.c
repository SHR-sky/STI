#include "Timer.h"
#include "LED.h"
#include "usart.h"

volatile uint32_t capture1 = 0;
volatile uint32_t capture2 = 0;
volatile uint32_t capture3 = 0;
volatile uint8_t capture_flag [3] = {0,0,0};
volatile uint32_t ovf_count = 0;


//定时器5通道1输入捕获配置
//初始化定时器5的结构体
TIM_ICInitTypeDef  TIM5_ICInitStructure;

void TIM5_Init(void)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  			//PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			//PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//PA0 下拉
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  			//PA1 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			//PA1 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;  			//PA2 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			//PA2 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = 36; 							    //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =2-1; 							//预分频器,由于系统时钟为72M，最终分辨率为1微秒  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		//TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 					//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; 					//CC1S=01，CC1通道被配置为输入，IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//映射到TI1上,若映射到TI2上为TIM_ICSelection_IndirectTI
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			//配置输入分频,不分频，每个边沿都触发一次捕获 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x0F;							//IC1F配置输入滤波器，最大滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2;          			
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			
  	TIM5_ICInitStructure.TIM_ICFilter = 0x0F;							
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_3; 
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x0F;
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  			//TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  	//先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  							//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3 ,ENABLE);//允许更新中（计数器溢出计数中断）,允许CC1IE捕获中断	
	
   	TIM_Cmd(TIM5,ENABLE ); 										//使能定时器5
   

}



void TIM2_IRQHandler(void) 
{
	
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
        if (capture_flag[0] == 0)
		{
            capture1 = TIM_GetCapture1(TIM2) + ovf_count * 0x10000;
            capture_flag[0]++;	//防止干扰，防止再次使用
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{
        if (capture_flag[1] == 0) 
		{
            capture2 = TIM_GetCapture2(TIM2) + ovf_count * 0x10000;
            capture_flag[1]++;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
	{
        if (capture_flag[2] == 0) 
		{
            capture3 = TIM_GetCapture3(TIM2) + ovf_count * 0x10000;
            capture_flag[2]++;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //计数器溢出
	{
        ovf_count++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
	
}
