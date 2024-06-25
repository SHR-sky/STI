#include "HC_SR04.h"

// 红外测距


#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_AHB1Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_11
#define HCSR04_ECHO     GPIO_Pin_10


u16 msHcCount = 0; 
 
void HC_Init(void)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(HCSR04_CLK, ENABLE);
 
    GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
 
    GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
    GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    
 
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   
 
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period = (1000-1); 
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);          
 
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);  
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);    
 
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       
    NVIC_Init(&NVIC_InitStructure);
 
    TIM_Cmd(TIM4,DISABLE);     
}

 
static void OpenTimerForHc()  
{
    TIM_SetCounter(TIM4,0);
    msHcCount = 0;
    TIM_Cmd(TIM4, ENABLE); 
}
 
 
static void CloseTimerForHc()    
{
    TIM_Cmd(TIM4, DISABLE); 
}
 
 
void TIM4_IRQHandler(void)  
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  
   {
       TIM_ClearITPendingBit(TIM4, TIM_IT_Update  ); 
       msHcCount++;
   }
}
 
 
 
  u32 GetEchoTimer(void)
{
    u32 t = 0;
    t = msHcCount*1000;
    t += TIM_GetCounter(TIM4);
    TIM4->CNT = 0;  
    delay_ms(50);
    return t;
}
 
  float HC_Get(void )
{
    int t = 0;
    int i = 0;
    float lengthTemp = 0;
    float sum = 0;
    while(i!=5)
   {
       PBout(11) = 1;      
       delay_us(20);
       TRIG_Send = 0;
       while(ECHO_Reci == 0);      
       OpenTimerForHc();        
       i = i + 1;
       while(ECHO_Reci == 1);
       CloseTimerForHc();        
       t = GetEchoTimer();        
       lengthTemp = ((float)t/ (float)58.0);//cm
       sum = lengthTemp + sum ;
 
   }
    lengthTemp = sum / (float)5.0;
    return lengthTemp;
}



