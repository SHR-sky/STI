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
int outPha = 0;

int Vpp2Num(int vpp, uint8_t Channel);
int Angle2Num(int angle);

int main()
{
	Serial_Init();
	Serial_Printf("Init Finish!\r\n");
	
	AD9959_Init();
	AD9959_Set_Fre(CH0,outBaseFre*MHz_);
	AD9959_Set_Amp(CH0,(int)(Vpp2Num(outBaseAmp,CH0)*1.414));
	AD9959_Set_Phase(CH0,0);
	
	AD9959_Set_Fre(CH1,outAmFre*MHz_);
	AD9959_Set_Amp(CH1,(int)(Vpp2Num(outAmAmp,CH1)));
	AD9959_Set_Phase(CH1,0);
	
	AD9959_Set_Fre(CH2,outBaseFre*MHz_);
	AD9959_Set_Amp(CH2,(int)(Vpp2Num(outBaseAmp,CH2)*1.414));
	AD9959_Set_Phase(CH2,Angle2Num(outPha));
	
	AD9959_Set_Fre(CH3,outAmFre*MHz_);
	AD9959_Set_Amp(CH3,(int)(Vpp2Num(outAmAmp,CH3))); 
	AD9959_Set_Phase(CH3,Angle2Num(outPha));
	
	IO_Update();
	
	/*
	while(1)
	{
		if(baseFreAdjust!=0)
		{
			outBaseAmp += baseFreAdjust*10;
			if(outBaseAmp < 10)
			{
				outBaseAmp = 10;
			}
			else if(outBaseAmp > 400)
			{ 
				outBaseAmp = 400;
			}
			
			AD9959_Set_Fre(CH2,outBaseFre*MHz_);
			AD9959_Set_Amp(CH2,(int)(Vpp2Num(outBaseAmp,CH2)));
			Serial_Printf("%d\r\n",outBaseAmp);
			AD9959_Set_Phase(CH2,0);
			IO_Update();
			baseFreAdjust = 0;
		}
	}
	*/
	
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
			AD9959_Set_Amp(CH0,(int)(Vpp2Num(outBaseAmp,CH0)*1.414));
			AD9959_Set_Phase(CH0,0);
			
			AD9959_Set_Fre(CH2,outBaseFre*MHz_);
			AD9959_Set_Amp(CH2,(int)(Vpp2Num(outBaseAmp,CH2)*1.414));
			AD9959_Set_Phase(CH2,Angle2Num(outPha));
			
			IO_Update();
			
			baseFreAdjust = 0;
		}
		if(baseAmpAdjust!=0)
		{	
			outBaseAmp += baseAmpAdjust*100;  // NEED TO CHANGE
			if(outBaseAmp < 100)
			{
				outBaseAmp = 100;
			}
			else if(outBaseAmp > 300)
			{
				outBaseAmp = 300;
			}
			
			AD9959_Set_Fre(CH0,outBaseFre*MHz_);
			AD9959_Set_Amp(CH0,(int)(Vpp2Num(outBaseAmp,CH0)*1.414));
			AD9959_Set_Phase(CH0,0);
			
			AD9959_Set_Fre(CH2,outBaseFre*MHz_);
			AD9959_Set_Amp(CH2,(int)(Vpp2Num(outBaseAmp,CH2)*1.414));
			AD9959_Set_Phase(CH2,Angle2Num(outPha));
			
			IO_Update();
			baseAmpAdjust = 0;
		}
		if(AmAmpAdjust!=0)
		{	
			outAmAmp += AmAmpAdjust*10;
			if(outAmAmp < 30)
			{
				outAmAmp = 30;
			}
			else if(outAmAmp > 90)
			{
				outAmAmp = 90;
			}
			
			AD9959_Set_Fre(CH1,outAmFre*MHz_);
			AD9959_Set_Amp(CH1,(int)(Vpp2Num(outAmAmp,CH1)));
			AD9959_Set_Phase(CH1,0);
			
			AD9959_Set_Fre(CH3,outAmFre*MHz_);
			AD9959_Set_Amp(CH3,(int)(Vpp2Num(outAmAmp,CH3))); 
			AD9959_Set_Phase(CH3,Angle2Num(outPha));
			AmAmpAdjust = 0;
			IO_Update();
		}
		if(delayTime!=0)
		{
			outPha += delayTime*30.0/(1000.0/(outBaseFre*1.0))*360.0;
			if(outPha < 0)
			{
				outPha = outPha + 360;
			}
			else if(outPha > 360)
			{
				outPha -= 360;
			}
			AD9959_Set_Fre(CH2,outBaseFre*MHz_);
			AD9959_Set_Amp(CH2,(int)(Vpp2Num(outBaseAmp,CH2)*1.414));
			AD9959_Set_Phase(CH2,Angle2Num(outPha));
			
			AD9959_Set_Fre(CH3,outAmFre*MHz_);
			AD9959_Set_Amp(CH3,(int)(Vpp2Num(outAmAmp,CH3))); 
			AD9959_Set_Phase(CH3,Angle2Num(outPha));
			IO_Update();
			delayTime = 0; // 30ns
		}
		if(pha!=0)
		{
			outPha += 10*pha;
			if(outPha < 0)
			{
				outPha = outPha + 360;
			}
			else if(outPha > 360)
			{
				outPha -= 360;
			}
			AD9959_Set_Fre(CH2,outBaseFre*MHz_);
			AD9959_Set_Amp(CH2,(int)(Vpp2Num(outBaseAmp,CH2)*1.414));
			AD9959_Set_Phase(CH2,Angle2Num(outPha));
			
			AD9959_Set_Fre(CH3,outAmFre*MHz_);
			AD9959_Set_Amp(CH3,(int)(Vpp2Num(outAmAmp,CH3))); 
			AD9959_Set_Phase(CH3,Angle2Num(outPha));
			IO_Update();
			pha = 0;
		}
	}
}

/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIMp_Cmd(TIM2,ENABLE);
	} 
}
*/

int Vpp2Num(int vpp, uint8_t Channel)
{
	if(Channel == CH0)
	{
		if(vpp<100)
			return (int)(vpp*2.144);
		else
			return (int)(vpp*2.169);
	}
	else if(Channel == CH1)
	{
		if(vpp<300)
			return (int)(vpp*1.865);
		else
			return (int)(vpp*1.880);
	}
	else if(Channel == CH2)
	{
		if(vpp<100)
			return (int)(vpp*2.144);
		else
			return (int)(vpp*2.169);
	}
	else if(Channel == CH3)
	{
		if(vpp<300)
			return (int)(vpp*1.865);
		else
			return (int)(vpp*1.880);
	}
}

int Angle2Num(int angle)
{
	return (int)(angle*16384.0/360.0);
}
