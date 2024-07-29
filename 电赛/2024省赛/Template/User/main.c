#include "sys.h"

extern uint16_t meDA1_Value[DA1_Value_Length];

// CH0 直达信号载波
// CH1 直达信号AM
// CH2 多径信号载波
// CH3 多径信号AM

#define MHz_ 1000000 

int baseFreAdjust = 0;
int baseAmpAdjust = 0;  // 载波可调
int AmAmpAdjust = 0;  // AM波可调
int delayTime = 0; // 延迟时间
int pha = 0; // 延迟相位

int outBaseFre = 35; // 载波频率
int outAmFre = 2; // AM频率
int outBaseAmp = 200; // 载波vpp
int outAmAmp = 50; // AMvpp

int Vpp2Num(int num);

int main()
{
	Serial_Init();
	Serial_Printf("Init Finish!\r\n");
	
	AD9959_Init();
	AD9959_Set_Fre(CH0,outBaseFre*MHz_);
	AD9959_Set_Amp(CH0,(int)(Vpp2Num(outBaseAmp)*1.414));
	AD9959_Set_Phase(CH0,0);
	
	AD9959_Set_Fre(CH1,outAmFre*MHz_);
	AD9959_Set_Amp(CH1,(int)(Vpp2Num(outAmAmp)));
	AD9959_Set_Phase(CH1,0);
	
	AD9959_Set_Fre(CH2,outBaseFre*MHz_);
	AD9959_Set_Amp(CH2,(int)(Vpp2Num(outBaseAmp)*1.414));
	AD9959_Set_Phase(CH2,0);
	
	AD9959_Set_Fre(CH3,outAmFre*MHz_);
	AD9959_Set_Amp(CH3,(int)(Vpp2Num(outAmAmp))); 
	AD9959_Set_Phase(CH3,0);
	
	IO_Update();

	while(1)
	{
		if(baseFreAdjust!=0)
		{
			outBaseFre += baseFreAdjust;
			if(outBaseFre < 30)
			{
				outBaseFre = 30;
			}
			else if(outBaseFre > 40)
			{
				outBaseFre = 40;
			}
			
			AD9959_Set_Fre(CH0,outBaseFre*MHz_);
			AD9959_Set_Amp(CH0,(int)(Vpp2Num(outBaseAmp)*1.414));
			AD9959_Set_Phase(CH0,0);
			
			AD9959_Set_Fre(CH2,outBaseFre*MHz_);
			AD9959_Set_Amp(CH2,(int)(Vpp2Num(outBaseAmp)*1.414));
			AD9959_Set_Phase(CH2,0);
			
			IO_Update();
			
			baseFreAdjust = 0;
		}
		if(baseAmpAdjust!=0)
		{
			baseAmpAdjust = 0;
			
			outBaseAmp += baseFre;
			if(outBaseFre < 30)
			{
				outBaseFre = 30;
			}
			else if(outBaseFre > 40)
			{
				outBaseFre = 40;
			}
			
			AD9959_Set_Fre(CH0,outBaseFre*MHz_);
			AD9959_Set_Amp(CH0,(int)(Vpp2Num(outBaseAmp)*1.414));
			AD9959_Set_Phase(CH0,0);
			
			AD9959_Set_Fre(CH2,outBaseFre*MHz_);
			AD9959_Set_Amp(CH2,(int)(Vpp2Num(outBaseAmp)*1.414));
			AD9959_Set_Phase(CH2,0);
			
			IO_Update();
		}
		if(AmAmpAdjust!=0)
		{
			AmAmpAdjust = 0;
		}
		if(delayTime!=0)
		{
			delayTime = 0;
		}
		if(pha!=0)
		{
			pha = 0;
		}
	}
	for(int i=0; i<100; i++);
	
	IO_Update();
	/*
	for(int i=0;i<10; i++)
	{
		delay_s(1);
		AD9959_Set_Phase(CH1,i*1000);
		IO_Update();
	}*/
	//delay_s(1);
	//AD9959_Set_Phase(CH1,2000);
	
	/*
	for(int i=0; i<DA1_Value_Length; i++)
	{
		meDA1_Value[i] = 2000;
	}
	DA1_Init();
	TIM4_Init();
	TIM_Cmd(TIM4, ENABLE);
	DA2_Init();
	TIM6_Init();
	TIM_Cmd(TIM6, ENABLE);

	while(1)
	{
		delay_s(1);
		Serial_Printf("Circle:1 \r\n");
	}*/
}

/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM_Cmd(TIM2,ENABLE);
	}
}
*/

int Vpp2Num(int vpp)
{
	return (int)(vpp*2.156);
}
