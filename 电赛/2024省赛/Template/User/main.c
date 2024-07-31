#include "sys.h"

extern uint16_t meDA1_Value[DA1_Value_Length];

// CH0 直达信号载波
// CH1 直达信号AM
// CH3 多径信号载波
// CH2 多径信号AM

#define MHz_ 1000000 

#define VCA_NUM 1100

#define REF_VCC_100mv 125

#define REF_VCC_1000mv 1257

int baseFreAdjust = 0;
int baseAmpAdjust = 0;  // 载波可调
int AmAmpAdjust = 0;  // AM波可调
int delayTime = 0; // 延迟时间
int pha = 0; // 延迟相位

int CWorAM = 1;

int outBaseFre = 35; // 载波频率m
int outAmFre = 2; // AM频率
int outBaseAmp = 50; // 载波vpp
int outAmAmp = 240; // AMvpp
int outPha = 0;
int Db2Num(int DB);
int Vpp2Num(int vpp, uint8_t Channel);
int Angle2Num(int angle);

#define RELAY_CTR PCout(13)

// PC13 继电器
// PC12 DIS
// __nop() 5.95ns

int main()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	                   //PC port clock enable
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_12;                             //Initialize PD0~12
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	for(int i=0; i<DA1_Value_Length; i++)
	{
		meDA1_Value[i] = REF_VCC_1000mv;
	}
	DA1_Init();
	TIM4_Init();
	TIM_Cmd(TIM4, ENABLE);
	
	PE_GPIO_Init();
	
	Serial_Init();
	Serial_Printf("Init Finish!\r\n");
	
	PE4302_0_Set(0);
	
	AD9959_Init();
	AD9959_Set_Fre(CH0,outBaseFre*MHz_);
	AD9959_Set_Amp(CH0,(int)(Vpp2Num(outBaseAmp,CH0)));
	AD9959_Set_Phase(CH0,0);
	
	AD9959_Set_Fre(CH1,outAmFre*MHz_);
	AD9959_Set_Amp(CH1,(int)(Vpp2Num(outAmAmp,CH1)));
	AD9959_Set_Phase(CH1,0);
	
	outPha = 50*(1000.0/(outBaseFre*1.0))*360.0;
	PCout(12) = 0;
	
	AD9959_Set_Fre(CH3,outBaseFre*MHz_);
	AD9959_Set_Amp(CH3,(int)(Vpp2Num(outBaseAmp,CH3)));
	AD9959_Set_Phase(CH3,Angle2Num(outPha));
	
	AD9959_Set_Fre(CH2,outAmFre*MHz_);
	AD9959_Set_Amp(CH2,(int)(Vpp2Num(outAmAmp,CH2))); 
	AD9959_Set_Phase(CH2,Angle2Num(outPha));
	
	IO_Update();
	
	PE4302_0_Set(Db2Num(20));
	
	nop_delay(50);
	PCout(12) = 1; // 开启载波
	//while(1);
	
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
			AD9959_Set_Amp(CH0,(int)(Vpp2Num(outBaseAmp,CH0)));
			AD9959_Set_Phase(CH0,0);
			
			AD9959_Set_Fre(CH3,outBaseFre*MHz_);
			AD9959_Set_Amp(CH3,(int)(Vpp2Num(outBaseAmp,CH3)));
			AD9959_Set_Phase(CH3,Angle2Num(outPha));
			
			IO_Update();
			
			baseFreAdjust = 0;
		}
		if(baseAmpAdjust!=0)
		{	
			outBaseAmp += baseAmpAdjust*10;  // NEED TO CHANGE
			if(outBaseAmp < 10)
			{
				outBaseAmp = 10;
			}
			else if(outBaseAmp > 100)
			{
				outBaseAmp = 100;
			}

			AD9959_Set_Fre(CH0,outBaseFre*MHz_);
			AD9959_Set_Amp(CH0,(int)(Vpp2Num(outBaseAmp,CH0)));
			AD9959_Set_Phase(CH0,0);
			Serial_Printf("Amp:%d\r\n",outBaseAmp);
			AD9959_Set_Fre(CH3,outBaseFre*MHz_);
			AD9959_Set_Amp(CH3,(int)(Vpp2Num(outBaseAmp,CH3)));
			AD9959_Set_Phase(CH3,Angle2Num(outPha));
			
			IO_Update();
			baseAmpAdjust = 0;
		}
		if(AmAmpAdjust!=0)
		{	
			outAmAmp += AmAmpAdjust*40;
			if(outAmAmp < 120)
			{
				outAmAmp = 120;
			}
			else if(outAmAmp > 360)
			{
				outAmAmp = 360;
			}
			
			if(CWorAM==1) // AM
			{
				AD9959_Set_Fre(CH1,outAmFre*MHz_);
				AD9959_Set_Amp(CH1,(int)(Vpp2Num(outAmAmp,CH1)));
				AD9959_Set_Phase(CH1,0);
			
				AD9959_Set_Fre(CH2,outAmFre*MHz_);
				AD9959_Set_Amp(CH2,(int)(Vpp2Num(outAmAmp,CH2))); 
				AD9959_Set_Phase(CH2,Angle2Num(outPha));
			}
			else if(CWorAM==0) // CW
			{
				AD9959_Set_Fre(CH1,outAmFre*MHz_);
				AD9959_Set_Amp(CH1,0);
				AD9959_Set_Phase(CH1,0);
			
				AD9959_Set_Fre(CH2,outAmFre*MHz_);
				AD9959_Set_Amp(CH2,0); 
				AD9959_Set_Phase(CH2,Angle2Num(outPha));
			}
				
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
			AD9959_Set_Fre(CH3,outBaseFre*MHz_);
			AD9959_Set_Amp(CH3,(int)(Vpp2Num(outBaseAmp,CH3)));
			AD9959_Set_Phase(CH3,Angle2Num(outPha));
			
			if(CWorAM == 1)
			{
				AD9959_Set_Fre(CH2,outAmFre*MHz_);
				AD9959_Set_Amp(CH2,(int)(Vpp2Num(outAmAmp,CH2))); 
				AD9959_Set_Phase(CH2,Angle2Num(outPha));
			}
			else if(CWorAM == 0)
			{
				AD9959_Set_Fre(CH2,outAmFre*MHz_);
				AD9959_Set_Amp(CH2,0); 
				AD9959_Set_Phase(CH2,Angle2Num(outPha));
			}
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
			AD9959_Set_Fre(CH3,outBaseFre*MHz_);
			AD9959_Set_Amp(CH3,(int)(Vpp2Num(outBaseAmp,CH3)));
			AD9959_Set_Phase(CH3,Angle2Num(outPha));
			
			AD9959_Set_Fre(CH2,outAmFre*MHz_);
			AD9959_Set_Amp(CH2,(int)(Vpp2Num(outAmAmp,CH2))); 
			AD9959_Set_Phase(CH2,Angle2Num(outPha));
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
	else if(Channel == CH3)
	{
		if(vpp<100)
			return (int)(vpp*2.144);
		else
			return (int)(vpp*2.169);
	}
	else if(Channel == CH2)
	{
		if(vpp<300)
			return (int)(vpp*1.865);
		else
			return (int)(vpp*1.880);
	}
	return (int)(vpp*2.144);
}

int Angle2Num(int angle)
{
	return (int)(angle*16384.0/360.0);
}

int Db2Num(int DB)
{
	if(DB==2)
		return 0;
	else if(DB==4)
		return 5;
	else if(DB==6)
		return 9;
	else if(DB==8)
		return 13;
	else if(DB==10)
		return 17;
	else if(DB==12)
		return 22;
	else if(DB==14)
		return 26;
	else if(DB==16)
		return 30;
	else if(DB==18)
		return 33;
	else if(DB==20)
		return 37;
	else if(DB==0)
		return 0; // FIXME
	else 
		return 0;
}

