// TIM4
#include "TIM.h"

#define KEY1_INT_GPIO_PORT GPIOA
#define KEY1_INT_GPIO_CLK RCC_AHB1Periph_GPIOA
#define KEY1_INT_GPIO_PIN GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE EXTI_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE EXTI_PinSource0
#define KEY1_INT_EXTI_LINE EXTI_Line0
#define KEY1_INT_EXTI_IRQ EXTI0_IRQn

#define KEY1_IRQHandler EXTI0_IRQHandler

#define ARR_NUM 10000
#define PSC_NUM 84

uint32_t IC1Value,IC2Value;
float DutyCycle,Frequency;
int cnt = 0;

void exti_init(void)  //外部中断初始化函数
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//设置中断组为 0
	//设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/*
void TIM_PWMINPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

//--------------------GPIO结构体初始化-------------------------
	//开始 GPIO端口时钟
	RCC_AHB1PeriphClockCmd (ADVANCE_ICPWM_GPIO_CLK, ENABLE);

	//定时器复用引脚

	GPIO_PinAFConfig(ADVANCE_ICPWM_GPIO_PORT,ADVANCE_ICPWM_PINSOURCE,ADVANCE_ICPWM_AF);

	//高级控制定时器 PWM输入捕获引脚
	GPIO_InitStructure.GPIO_Pin = ADVANCE_ICPWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(ADVANCE_ICPWM_GPIO_PORT, &GPIO_InitStructure);

	//开启 TIMx_CLK
	//高级定时器时钟源 TIMxCLK = 2*PCLK2=168MHz
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE);

	//--------------------时基结构体初始化----------------------
	//累计 TIM_Period个后产生一个更新或者中断 
	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;
	//驱动 CNT计数器的时钟 = TIMxCLK/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
	//时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	//计数方式
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	//重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	//初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
	//-------------------输入捕获结构体初始化-------------------
	//使用 PWM输入模式时，需要占用两个捕获寄存器，一个测周期，另外一个测占空比
	//捕获通道 IC1配置
	//选择捕获通道
	TIM_ICInitStructure.TIM_Channel = ADVANCE_IC1PWM_CHANNEL;
	//设置捕获的边沿
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	//设置捕获通道的信号来自于哪个输入通道，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 1分频，即捕获信号的每个有效边沿都捕获
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	//不滤波
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	//初始化 PWM输入模式
	TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
	//当工作做 只需要设置触发信号的那一路即可（用于测量周期）
	//另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
	//选择输入捕获的触发信号
	TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);
	
	//选择从模式:复位模式
	// PWM输入模式时,从模式必须工作在复位模式，当捕获开始时,会被复位
	TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable);
	
	//使能捕获中断,这个中断针对的是主捕获通道（测量周期那个）
	TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);

	//使能高级控制定时器，计数器开始计数
	TIM_Cmd(ADVANCE_TIM, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//设置中断组为 0
	//设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQn;
	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
*/

void Timer_Init(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_TimeBaseStructure.TIM_Period = (10000-1); 
    TIM_TimeBaseStructure.TIM_Prescaler =(8400-1); 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);          
 
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);  
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       
    NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM2,ENABLE); 	
	// Input Capture
	// PB6 ~PB9 -> TIM4CH1~CH4
	/*
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启时钟

	TIM_InternalClockConfig(TIM2);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; // TIM结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = ARR_NUM - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC_NUM - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除更新时的中断标志位，防止更新时程序直接进入中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitTypeDef NVIC_InitStructure; // NVIC结构体
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  // 相应优先级
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2, ENABLE);
	*/
}

/*
void ADVANCE_TIM_IRQHandler (void)
{
	//清除定时器捕获/比较 1中断 
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);

	/'获取输入捕获值 
	IC1Value = TIM_GetCapture1(ADVANCE_TIM);
	IC2Value = TIM_GetCapture2(ADVANCE_TIM);
	//注意：捕获寄存器 CCR1和 CCR2的值在计算占空比和频率的时候必须加 1
	if (IC1Value != 0) 
	{
		cnt += 1;
		//占空比计算 
		DutyCycle = (double)((IC2Value+1) * 100) / (double)(IC1Value+1);

		//频率计算 
		Frequency += 16800/(double)(ADVANCE_TIM_PSC+1)/(double)(IC1Value+1);
		if(cnt >= 10000)
		{
			Serial_Printf("Duty:%lf , Fre:%lf\r\n",DutyCycle,Frequency);
			cnt = 0;
			Frequency = 0;
		}
	} 
	else 
	{
		DutyCycle = 0;
		Frequency = 0;
	}
}
*/

void KEY1_IRQHandler(void)
{
	if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) {
		cnt += 1;
		//Serial_Printf("1\r\n");
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
	}
}
