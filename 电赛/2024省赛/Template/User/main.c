#include "sys.h"

extern uint16_t meDA1_Value[DA1_Value_Length];

// CH0 直达信号载波
// CH1 直达信号AM
// CH3 多径信号载波
// CH2 多径信号AM

#define MHz_ 1000000 

#define VCA_NUM 1100

#define REF_VCC_100mv 116

#define REF_VCC_1000mv 300 // 368

int baseFreAdjust = 0;
int baseAmpAdjust = 0;  // 载波可调
int AmAmpAdjust = 0;  // AM波可调
int delayTime = 0; // 延迟时间
int pha = 0; // 延迟相位
int DBAdjust = 0;

int CWorAM = 0;

int outBaseFre = 35; // 载波频率m
int outAmFre = 2; // AM频率
int outBaseAmp = 50; // 载波vpp
int outAmAmp = 60; // AMvpp
int outPha = 0;
int outDB = 0;
int Db2Num(int DB);
int Vpp2Num_CHO(int vpp);
int Vpp2Num_CH3(int vpp);
int Vpp2Num_CH1(int vpp);
int Vpp2Num_CH2(int vpp);
int Vpp2Num(int vpp, uint8_t Channel);
int Angle2Num(int angle);

int DCValue = 623;

int CW_DC1Value = 150;
int CW_DC2Value = 300;

#define RELAY_CTR PCout(13)

// PC13 继电器
// PC12 DIS
// __nop() 5.95ns

int main()
{
	DCValue = REF_VCC_1000mv;
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
		meDA1_Value[i] = CW_DC2Value;
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
	
	if(CWorAM==1)
	{
		AD9959_Set_Fre(CH1,outAmFre*MHz_);
		AD9959_Set_Amp(CH1,(int)(Vpp2Num(outAmAmp,CH1)));
		AD9959_Set_Phase(CH1,0);
	}
	else if(CWorAM==0)
	{
		AD9959_Set_Fre(CH1,outAmFre*MHz_);
		AD9959_Set_Amp(CH1,0);
		AD9959_Set_Phase(CH1,0);		
	}
	
	outPha = 0;
	//outPha = 50*(1000.0/(outBaseFre*1.0))*360.0;
	PCout(12) = 0;
	
	AD9959_Set_Fre(CH3,outBaseFre*MHz_);
	AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)));
	AD9959_Set_Phase(CH3,Angle2Num(outPha));
	
	if(CWorAM==1)
	{
		AD9959_Set_Fre(CH2,outAmFre*MHz_);
		AD9959_Set_Amp(CH2,(int)(Vpp2Num(outAmAmp,CH2))); 
		AD9959_Set_Phase(CH2,Angle2Num(outPha));
	}
	else if(CWorAM==0)
	{
		AD9959_Set_Fre(CH2,outAmFre*MHz_);
		AD9959_Set_Amp(CH2,0); 
		AD9959_Set_Phase(CH2,Angle2Num(outPha));		
	}
	IO_Update();
	
	//PE4302_0_Set(Db2Num(20));
	
	nop_delay(50);
	PCout(12) = 1; // 开启载波

	
	while(1)
	{
		if(CWorAM == 0) // CW
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
				outBaseAmp += baseAmpAdjust;

				//outBaseAmp += baseAmpAdjust*10;  // NEED TO CHANGE
				/*
				if(outBaseAmp < 10)
				{
					outBaseAmp = 10;
				}
				else if(outBaseAmp > 100)
				{
					outBaseAmp = 100;
				}
				*/
				AD9959_Set_Fre(CH0,outBaseFre*MHz_);
				AD9959_Set_Amp(CH0,Vpp2Num_CHO(outBaseAmp));
				AD9959_Set_Phase(CH0,0);
				Serial_Printf("Amp:%d\r\n",outBaseAmp);
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				Vpp2Num_CHO(40);
				AD9959_Set_Amp(CH3,outBaseAmp);
				AD9959_Set_Phase(CH3,Angle2Num(outPha));
				
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
				
				Serial_Printf("outAmAmp:%d\r\n",outAmAmp);
				if(CWorAM==1) // AM
				{
					AD9959_Set_Fre(CH1,outAmFre*MHz_);
					AD9959_Set_Amp(CH1,Vpp2Num_CH1(outAmAmp));
					AD9959_Set_Phase(CH1,0);
				
					AD9959_Set_Fre(CH2,outAmFre*MHz_);
					AD9959_Set_Amp(CH2,Vpp2Num_CH2(outAmAmp)); 
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
				outPha -= pha;
				if(outPha < 0)
				{
					outPha = outPha + 360;
				}
				else if(outPha > 360)
				{
					outPha -= 360;
				}
				Serial_Printf("Pha:%d\r\n",outPha);
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num(outBaseAmp,CH3)));
				AD9959_Set_Phase(CH3,Angle2Num(outPha)); // 
			
				AD9959_Set_Fre(CH2,outAmFre*MHz_);
				AD9959_Set_Amp(CH2,(int)(Vpp2Num(outAmAmp,CH2))); 
				AD9959_Set_Phase(CH2,Angle2Num(outPha)); //Angle2Num(outPha)
				IO_Update();
				pha = 0;
			}
			if(DBAdjust != 0)
			{
				outDB += DBAdjust*2;
				if(outDB <= 0)
				{
					outDB = 0;
				}
				else if(outDB >= 20)
				{
					outDB = 20;
				}
				PE4302_0_Set(Db2Num(outDB));
				DBAdjust = 0;
			}
		}
		else if(CWorAM==1)
		{
			DCValue = REF_VCC_100mv;
			for(int i=0; i<DA1_Value_Length; i++)
			{
				meDA1_Value[i] = DCValue;
			}
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
				outBaseAmp += baseAmpAdjust*10;

				//outBaseAmp += baseAmpAdjust*10;  // NEED TO CHANGE
				
				if(outBaseAmp < 10)
				{
					outBaseAmp = 10;
				}
				else if(outBaseAmp > 100)
				{
					outBaseAmp = 100;
				}
				

				AD9959_Set_Fre(CH0,outBaseFre*MHz_);
				AD9959_Set_Amp(CH0,Vpp2Num_CHO(outBaseAmp));
				AD9959_Set_Phase(CH0,0);
				Serial_Printf("Amp:%d\r\n",outBaseAmp);
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,Vpp2Num_CH3(outBaseAmp));
				AD9959_Set_Phase(CH3,Angle2Num(outPha));
				
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
				
				Serial_Printf("outAmAmp:%d\r\n",outAmAmp);
				if(CWorAM==1) // AM
				{
					AD9959_Set_Fre(CH1,outAmFre*MHz_);
					AD9959_Set_Amp(CH1,Vpp2Num_CH1(outAmAmp));
					AD9959_Set_Phase(CH1,0);
				
					AD9959_Set_Fre(CH2,outAmFre*MHz_);
					AD9959_Set_Amp(CH2,Vpp2Num_CH2(outAmAmp)); 
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
				outPha += 5*pha;
				if(outPha < 0)
				{
					outPha = outPha + 360;
				}
				else if(outPha > 360)
				{
					outPha -= 360;
				}
				Serial_Printf("Pha:%d\r\n",outPha);
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num(outBaseAmp,CH3)));
				AD9959_Set_Phase(CH3,Angle2Num(outPha));
			
				AD9959_Set_Fre(CH2,outAmFre*MHz_);
				AD9959_Set_Amp(CH2,(int)(Vpp2Num(outAmAmp,CH2))); 
				AD9959_Set_Phase(CH2,Angle2Num(outPha));
				IO_Update();
				pha = 0;
			}
			if(DBAdjust != 0)
			{
				outDB += DBAdjust;
				Serial_Printf("DB:%d",outDB);
				/*
				if(outDB <= 0)
				{
					outDB = 0;
				}
				else if(outDB >= 20)
				{
					outDB = 20;
				}*/
				PE4302_0_Set(outDB);
				DBAdjust = 0;
			}
		}
		
	}
}


int Vpp2Num(int vpp, uint8_t Channel)
{
	if(Channel == CH0)
	{
		if(vpp == 100)
			return (int)(vpp*1.950);
		else if(vpp == 90)
			return (int)(vpp*1.973);
		else if(vpp == 80)
			return (int)(vpp*1.985);
		else
			return (int)(vpp*1.965);
	}
	else if(Channel == CH1)
	{
		if(vpp<300)
			return (int)(vpp*1.875);
		else
			return (int)(vpp*1.880);
	}
	else if(Channel == CH3)
	{
		if(vpp<100)
			return (int)(vpp*2.06);
		else
			return (int)(vpp*2.083);
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
	if(angle == 0)
		return 16383-80;
	else if(angle == 30)
		return ((int)(327*16384.0/360.0)-80)%16383;
	else if(angle == 60)
		return ((int)(297*16384.0/360.0)-80)%16383;
	else if(angle == 90)
		return ((int)(267*16384.0/360.0)-80)%16383;
	else if(angle == 120)
		return ((int)(237*16384.0/360.0)-80)%16383;
	else if(angle == 150)
		return ((int)(206*16384.0/360.0)-80)%16383;
	else if(angle == 180)
		return ((int)(176*16384.0/360.0)-80)%16383;
	else
		return ((int)((360-angle-3)*16384.0/360.0)-80)%16383;
}

int Vpp2Num_CHO(int vpp)
{
	if(vpp==10)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 150;
		}
		return 60;
	}
	else if(vpp == 20)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 150;
		}
		return 121;
	}
	else if(vpp == 30)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 250;
		}
		return 63;
	}
	else if(vpp == 40)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 300;
		}		
		return 63;
	}
	else if(vpp == 50)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 300;
		}	
		return 78;
	}
	else if(vpp == 60)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 300;
		}
		return 94;
	}
	else if(vpp == 70)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 300;
		}
		return 110;
	}
	else if(vpp == 80)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 300;
		}
		return 126;
	}
	else if(vpp == 90)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 300;
		}
		return 142;
	}
	else if(vpp == 100)
	{
		if(CWorAM==0)
		for(int i=0; i<DA1_Value_Length; i++)
		{
			meDA1_Value[i] = 300;
		}
		return 157;
	}
	return 75;
}

int Vpp2Num_CH3(int vpp)
{
	if(vpp==10)
	{
		return 91;
	}
	else if(vpp == 20)
	{
		return 172;
	}
	else if(vpp == 30)
	{
		return 58;
	}
	else if(vpp == 40)
	{
		return 144;
	}
	else if(vpp == 50)
	{
		return 94;
	}
	else if(vpp == 60)
	{
		return 214;
	}
	else if(vpp == 70)
	{
		return 132;
	}
	else if(vpp == 80)
	{
		return 150;
	}
	else if(vpp == 90)
	{
		return 169;
	}
	else if(vpp == 100)
	{
		return 188;
	}
	return 95;
}

int Vpp2Num_CH1(int vpp)
{
	if(vpp==30)
	{
		return 69;
	}
	else if(vpp == 40)
	{
		return 88;
	}
	else if(vpp == 50)
	{
		return 99;
	}
	else if(vpp == 60)
	{
		return 132;
	}
	else if(vpp == 70)
	{
		return 155;
	}
	else if(vpp == 80)
	{
		return 182;
	}
	else if(vpp == 90)
	{
		return 198;
	}
	return 100;
}
   
int Vpp2Num_CH2(int vpp)
{
	if(vpp==30)
	{
		return 111;
	}
	else if(vpp == 40)
	{
		return 146;
	}
	else if(vpp == 50)
	{
		return 171;
	}
	else if(vpp == 60)
	{
		return 197;
	}
	else if(vpp == 70)
	{
		return 218;
	}
	else if(vpp == 80)
	{
		return 244;
	}
	else if(vpp == 90)
	{
		return 266;
	}
	return 171;
}

int Db2Num(int DB)
{   
	if(DB==2)
		return 11; // 
	else if(DB==4)
		return 15;
	else if(DB==6)
		return 19;
	else if(DB==8)
		return 23;
	else if(DB==10)
		return 27;
	else if(DB==12)
		return 31;
	else if(DB==14)
		return 35;
	else if(DB==16)
		return 39;
	else if(DB==18)
		return 43;
	else if(DB==20)
		return 47;
	else if(DB==0)
		return 8; // FIXME
	else 
		return 0;
}

