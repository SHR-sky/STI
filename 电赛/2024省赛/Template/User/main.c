#include "sys.h"

extern uint16_t meDA1_Value[DA1_Value_Length];

// CH0 直达信号载波
// CH1 直达信号AM
// CH3 多径信号载波
// CH2 多径信号AM

#define MHz_ 1000000 

#define VCA_NUM 1100

#define REF_VCC_100mv 116

#define REF_VCC_1000mv 358 // 368

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
int outAmAmp = 50; // AMvpp
int outDelayBase = 0;
int outDelayAM = 0;
int outPha = 0;
int outDB = 0;
int Db2Num(int DB);
int Vpp2Num_CH0(int vpp);
int Vpp2Num_CH3(int vpp);
int Vpp2Num_CH1(int vpp);
int Vpp2Num_CH2(int vpp);
int Vpp2Num(int vpp, uint8_t Channel);
int Angle2Num(int angle);
int VppByFre(int vpp);
double DB2Time(int DB);

int Manual_Update_CH0 = 1;
int Manual_Update_CH1 = 1;
int Manual_Update_CH2 = 1;
int Manual_Update_CH3 = 1;

double CH0_Manual = 0.76;
double CH1_Manual = 1.0;
double CH2_Manual = 1.0;
double CH3_Manual = 0.62;

int AD9959_Update_Reset = 0;

int delayManual = 0;
int phaManual = 0;

int Manual_Update_DB = 0;
int DbManual = 0;

int DCValue = 623;


#define RELAY_CTR PCout(13)

// PC13 继电器
// PC12 DIS
// __nop() 5.95ns

int main()
{
	DCValue = 160;
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
		meDA1_Value[i] = DCValue;
	}
	DA1_Init();
	TIM4_Init();
	TIM_Cmd(TIM4, ENABLE);
	
	PE_GPIO_Init();
	
	Serial_Init();
	//Serial_Printf("Init Finish!\r\n");
	
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
	AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
	
	if(CWorAM==1)
	{
		AD9959_Set_Fre(CH2,outAmFre*MHz_);
		AD9959_Set_Amp(CH2,(int)(Vpp2Num(outAmAmp,CH2))); 
		AD9959_Set_Phase(CH2,Angle2Num(outPha+outDelayAM));
	}
	else if(CWorAM==0)
	{
		AD9959_Set_Fre(CH2,outAmFre*MHz_);
		AD9959_Set_Amp(CH2,0); 
		AD9959_Set_Phase(CH2,Angle2Num(outPha+outDelayAM));		
	}
	IO_Update();
	
	//PE4302_0_Set(Db2Num(20));
	
	nop_delay(50);
	PCout(12) = 1; // 开启载波

	
	while(1)
	{
		if(AD9959_Update_Reset == 1)
		{
			AD9959_Update_Reset = 0;
			IntReset();
			for(int i=0; i<1000; i++);
			AD9959_Init();
			AD9959_Set_Fre(CH0,outBaseFre*MHz_);
			AD9959_Set_Amp(CH0,(int)(Vpp2Num_CH0(outBaseAmp)*1.0*CH0_Manual));
			AD9959_Set_Phase(CH0,0);
			
			AD9959_Set_Fre(CH3,outBaseFre*MHz_);
			AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual/DB2Time(outDB)));
			AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
		
			IO_Update();
			NVIC_SystemReset();
		}
		if(CWorAM == 0) // CW
		{
			PCout(13) = 0;
			PCout(12) = 0;
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
				Serial_Printf("n0.val=%d",outBaseFre);
				Serial_End();
				AD9959_Set_Fre(CH0,outBaseFre*MHz_);
				AD9959_Set_Amp(CH0,(int)(Vpp2Num_CH0(outBaseAmp)*1.0*CH0_Manual));
				AD9959_Set_Phase(CH0,0);
			
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual/DB2Time(outDB)));
				AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
			
				IO_Update();
			
				baseFreAdjust = 0;
			}
			
			if(baseAmpAdjust!=0||Manual_Update_CH0 ==1||Manual_Update_CH3 ==1)
			{	
				outBaseAmp += 10*baseAmpAdjust;
				//outBaseAmp += baseAmpAdjust*10;  // NEED TO CHANGE
				
				
				if(outBaseAmp < 10)
				{
					outBaseAmp = 10;
				}
				else if(outBaseAmp > 100)
				{
					outBaseAmp = 100;
				}
				Serial_Printf("n1.val=%d",outBaseAmp*10);
				Serial_End();

				AD9959_Set_Fre(CH0,outBaseFre*MHz_);
				AD9959_Set_Amp(CH0,Vpp2Num_CH0(outBaseAmp)*1.0*CH0_Manual);
				AD9959_Set_Phase(CH0,0);
				//Serial_Printf("Amp:%d\r\n",outBaseAmp);
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual);
				AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
				
				IO_Update();
				baseAmpAdjust = 0;
				Manual_Update_CH0 =0;
				Manual_Update_CH3 =0;
			}
			if(AmAmpAdjust!=0)
			{	
				outAmAmp += 10*AmAmpAdjust;
				/*
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
					AD9959_Set_Amp(CH1,outAmAmp);
					AD9959_Set_Phase(CH1,0);
				
					AD9959_Set_Fre(CH2,outAmFre*MHz_);
					AD9959_Set_Amp(CH2,outAmAmp); 
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
				*/
				AmAmpAdjust = 0;
				//IO_Update();
			}
			if(delayTime!=0||delayManual!=0)
			{
				outDelayBase -= delayTime*30.0/(1000.0/(outBaseFre*1.0))*360.0;
				outDelayBase -= delayManual/(1000.0/(outBaseFre*1.0))*360.0;
				if(outDelayBase < 0)
				{
					outDelayBase = outDelayBase + 360;
				}
				else if(outDelayBase > 360)
				{
					outDelayBase -= 360;
				}
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual/DB2Time(outDB)));
				AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
			
				if(CWorAM == 1)
				{
					AD9959_Set_Fre(CH2,outAmFre*MHz_);
					AD9959_Set_Amp(CH2,(int)(Vpp2Num_CH2(outAmAmp))*1.0*CH2_Manual); 
					AD9959_Set_Phase(CH2,Angle2Num(outPha+outDelayAM));
				}
				else if(CWorAM == 0)
				{
					AD9959_Set_Fre(CH2,outAmFre*MHz_);
					AD9959_Set_Amp(CH2,0); 
					AD9959_Set_Phase(CH2,Angle2Num(outPha+outDelayAM));
				}
				IO_Update();
				delayTime = 0; // 30ns
				delayManual = 0;
			}
			if(pha!=0||phaManual!=0)
			{
				outPha += 30*pha;
				outPha -= phaManual;
				if(outPha < 0)
				{
					outPha = outPha + 360;
				}
				else if(outPha > 360)
				{
					outPha -= 360;
				}
				//Serial_Printf("Pha:%d\r\n",outPha);
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual));
				AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
			
				AD9959_Set_Fre(CH2,outAmFre*MHz_);
				AD9959_Set_Amp(CH2,(int)(Vpp2Num_CH2(outAmAmp)*1.0*CH2_Manual)); 
				AD9959_Set_Phase(CH2,Angle2Num(outPha+outDelayAM));
				IO_Update();
				pha = 0;
				phaManual = 0;
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
				//Serial_Printf("outDB:%d\r\n",outDB);
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual/DB2Time(outDB)));
				//Serial_Printf("Amp:%d\r\n",(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual/DB2Time(outDB)));
				AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase)); // 
				DBAdjust = 0;
			}
		}
		else if(CWorAM==1)
		{
			PCout(13) = 1;
			PCout(12) = 1;
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
				Serial_Printf("n0.val=%d",outBaseFre);
				Serial_End();
				AD9959_Set_Fre(CH0,outBaseFre*MHz_);
				AD9959_Set_Amp(CH0,(int)(Vpp2Num_CH0(outBaseAmp)*1.0*CH0_Manual));
				AD9959_Set_Phase(CH0,0);
			
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual));
				AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
			
				IO_Update();
			
				baseFreAdjust = 0;
			}
			if(baseAmpAdjust!=0||Manual_Update_CH0 ==1||Manual_Update_CH3 ==1)
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
				Serial_Printf("n1.val=%d",outBaseAmp*10);
				Serial_End();

				AD9959_Set_Fre(CH0,outBaseFre*MHz_);
				AD9959_Set_Amp(CH0,(int)(Vpp2Num_CH0(outBaseAmp)*1.0*CH0_Manual));
				AD9959_Set_Phase(CH0,0);
				//Serial_Printf("Amp:%d\r\n",outBaseAmp);
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual));
				AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
				
				IO_Update();
				baseAmpAdjust = 0;
				Manual_Update_CH0 =0;
				Manual_Update_CH3 =0;
			}
			if(AmAmpAdjust!=0||Manual_Update_CH1 ==1||Manual_Update_CH2 ==1)
			{	
				outAmAmp += 10*AmAmpAdjust;
				
				if(outAmAmp < 30)
				{	
					outAmAmp = 30;
				}
				else if(outAmAmp > 90)
				{
					outAmAmp = 90;
				}
				
				Serial_Printf("n2.val=%d",outAmAmp);
				Serial_End();
				//Serial_Printf("outAmAmp:%d\r\n",outAmAmp);
				if(CWorAM==1) // AM
				{
					AD9959_Set_Fre(CH1,outAmFre*MHz_);
					AD9959_Set_Amp(CH1,(int)(Vpp2Num_CH1(outAmAmp)*1.0*CH1_Manual));
					//AD9959_Set_Amp(CH1,outAmAmp);
					AD9959_Set_Phase(CH1,0);
				
					AD9959_Set_Fre(CH2,outAmFre*MHz_);
					AD9959_Set_Amp(CH2,(int)(Vpp2Num_CH2(outAmAmp)*1.0*CH2_Manual)); 
					//AD9959_Set_Amp(CH2,outAmAmp);
					AD9959_Set_Phase(CH2,Angle2Num(outPha+outDelayAM));
				}
				
				AmAmpAdjust = 0;
				Manual_Update_CH1 =0;
				Manual_Update_CH2 =0;
				IO_Update();
			}
			if(delayTime!=0||delayManual!=0)
			{
				outDelayAM -= delayTime*30.0/(1000.0/(outAmFre*1.0))*360.0;
				outDelayBase -= delayTime*30.0/(1000.0/(outBaseFre*1.0))*360.0;
				outDelayAM -= delayManual*1.0/(1000.0/(outAmFre*1.0))*360.0;
				outDelayBase -= delayManual*1.0/(1000.0/(outBaseFre*1.0))*360.0;
				if(outDelayAM < 0)
				{
					outDelayAM = outDelayAM + 360;
				}
				else if(outDelayAM > 360)
				{
					outDelayAM -= 360;
				}
				if(outDelayBase < 0)
				{
					outDelayBase = outDelayBase + 360;
				}
				else if(outDelayBase > 360)
				{
					outDelayBase -= 360;
				}
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual));
				AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
			
				if(CWorAM == 1)
				{
					AD9959_Set_Fre(CH2,outAmFre*MHz_);
					AD9959_Set_Amp(CH2,(int)(Vpp2Num_CH2(outAmAmp)*1.0*CH2_Manual)); 
					AD9959_Set_Phase(CH2,Angle2Num(outPha+outDelayAM));
				}
				IO_Update();
				delayTime = 0; // 30ns
				delayManual = 0;
			}
			if(pha!=0||phaManual!=0)
			{
				outPha += 30*pha;
				outPha -= phaManual;
				if(outPha < 0)
				{
					outPha = outPha + 360;
				}
				else if(outPha > 360)
				{
					outPha -= 360;
				}
				//Serial_Printf("Pha:%d\r\n",outPha);
				AD9959_Set_Fre(CH3,outBaseFre*MHz_);
				AD9959_Set_Amp(CH3,(int)(Vpp2Num_CH3(outBaseAmp)*1.0*CH3_Manual));
				AD9959_Set_Phase(CH3,Angle2Num(outPha+outDelayBase));
			
				AD9959_Set_Fre(CH2,outAmFre*MHz_);
				AD9959_Set_Amp(CH2,(int)(Vpp2Num_CH2(outAmAmp)*1.0*CH2_Manual)); 
				AD9959_Set_Phase(CH2,Angle2Num(outPha+outDelayAM));
				IO_Update();
				pha = 0;
				phaManual = 0;
			}
			if(DBAdjust != 0||Manual_Update_DB==1)
			{
				outDB += 2*DBAdjust;
				//Serial_Printf("DB:%d",outDB);
				
				if(outDB <= 0)
				{
					outDB = 0;
				}
				else if(outDB >= 20)
				{
					outDB = 20;
				}
				if(Db2Num(outDB)+DbManual<0)
					PE4302_0_Set(0);
				else if(Db2Num(outDB)+DbManual>63)
					PE4302_0_Set(63);
				else
					PE4302_0_Set(Db2Num(outDB)+DbManual);
				DBAdjust = 0;
				Manual_Update_DB = 0;
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

int Vpp2Num_CH0(int vpp)
{
	if(vpp==10)
	{
		return VppByFre(17);
	}
	else if(vpp == 20)
	{
		return VppByFre(33);
	}
	else if(vpp == 30)
	{
		return VppByFre(49);
	}
	else if(vpp == 40)
	{
		return VppByFre(65);
	}
	else if(vpp == 50)
	{
		return VppByFre(81);
	}
	else if(vpp == 60)
	{
		return VppByFre(97);
	}
	else if(vpp == 70)
	{
		return VppByFre(112);
	}
	else if(vpp == 80)
	{
		return VppByFre(128);
	}
	else if(vpp == 90)
	{
		return VppByFre(144);
	}
	else if(vpp == 100)
	{
		return VppByFre(161);
	}
	return VppByFre(75);
}

int Vpp2Num_CH3(int vpp)
{
	if(vpp==10)
	{
		return 25;
	}
	else if(vpp == 20)
	{
		return 64;
	}
	else if(vpp == 30)
	{
		return 99;
	}
	else if(vpp == 40)
	{
		return 133;
	}
	else if(vpp == 50)
	{
		return 167;
	}
	else if(vpp == 60)
	{
		return 200;
	}
	else if(vpp == 70)
	{
		return 235;
	}
	else if(vpp == 80)
	{
		return 267;
	}
	else if(vpp == 90)
	{
		return 300;
	}
	else if(vpp == 100)
	{
		return 339;
	}
	return 95;
}

int Vpp2Num_CH1(int vpp)
{
	if(vpp==30)
	{
		return 50;
	}
	else if(vpp == 40)
	{
		return 65;
	}
	else if(vpp == 50)
	{
		return 79;
	}
	else if(vpp == 60)
	{
		return 92;
	}
	else if(vpp == 70)
	{
		return 114;
	}
	else if(vpp == 80)
	{
		return 138;
	}
	else if(vpp == 90)
	{
		return 152;
	}
	return 100;
}
   
int Vpp2Num_CH2(int vpp)
{
	if(vpp==30)
	{
		return 88;
	}
	else if(vpp == 40)
	{
		return 102;
	}
	else if(vpp == 50)
	{
		return 126;
	}
	else if(vpp == 60)
	{
		return 150;
	}
	else if(vpp == 70)
	{
		return 203;
	}
	else if(vpp == 80)
	{
		return 227;
	}
	else if(vpp == 90)
	{
		return 251;
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
		return 46;
	else if(DB==0)
		return 8; // FIXME
	else 
		return 0;
}

double DB2Time(int DB)
{
	switch(DB)
	{
		case 0:
		{
			return 1.0;
		}
		case 2:
		{
			return 1.259;
		}
		case 4:
		{
			return 1.585;
		}
		case 6:
		{
			return 1.995;
		}
		case 8:
		{
			return 2.512;
		}
		case 10:
		{
			return 3.162;
		}
		case 12:
		{
			return 3.981;
		}
		case 14:
		{
			return 5.012;
		}
		case 16:
		{
			return 6.310;
		}
		case 18:
		{
			return 7.943;
		}
		case 20:
		{
			return 10.0;
		}
		default :
		{
			return 1;
		}
	}
}

int VppByFre(int vpp)
{
	if(outBaseFre==30)
		return round(1.106*vpp);
	else if(outBaseFre==31)
	{
		return round(1.093*vpp);
	}
	else if(outBaseFre==32)
	{
		return round(1.073*vpp);
	}
	else if(outBaseFre==33)
	{
		return round(1.055*vpp);
	}
	else if(outBaseFre==34)
	{
		return round(1.025*vpp);
	}
	else if(outBaseFre==35)
	{
		return round(1.0*vpp);
	}
	else if(outBaseFre==36)
	{
		return round(0.972*vpp);
	}
	else if(outBaseFre==37)
	{
		return round(0.936*vpp);
	}
	else if(outBaseFre==38)
	{
		return round(0.906*vpp);
	}
	else if(outBaseFre==39)
	{
		return round(0.869*vpp);
	}
	else if(outBaseFre==40)
	{
		return round(0.836*vpp);
	}
	else 
		return vpp;
}
