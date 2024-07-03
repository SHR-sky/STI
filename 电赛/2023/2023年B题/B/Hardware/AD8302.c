#include "AD8302.h"
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long int
#define PI 3.1415926536

float mypower(int index, float base)
{
	float result = 1;
	for(int i = 0; i < index; i++)
	{
		result *= base;
	}
    return result;
}

float GetMag(void) //输出幅度比
{
	float temp = Get_Adc_Average(ADC_Channel_1,100);

	float volt,volt_dis;
	temp=temp*(3.3/4096.0);
	volt=temp;
	Serial_Printf("%d",(int)volt);
	volt_dis = volt - (int)volt;
	volt_dis *= 10000;
	Serial_Printf("%d",(int)volt_dis);

	temp = (float)33.333333*(temp)-(float)30.0;
	temp=mypower(10,temp/(float)20.0);
	return temp;
}

float GetPhs(void)//输出相位差
{
	float temp = Get_Adc_Average(ADC_Channel_3,100);
	//float temp = Get_Adc(ADC_Channel_3);
	float volt;
	temp=temp*((float)3.3/(float)4096.0);
	volt=temp;
	Serial_Printf("%d",(int)volt);
	volt = volt - (int)volt;
	volt *= (float)10000;
	Serial_Printf("%d",(int)volt);
	//Vphs 的输出为 30mV～1.8V 表示 180 度～0 度，中间值 0.9V 为 90 度
	temp = (float)-98.901099*(temp)+(float)181.978022;
	//temp = -100.0*(temp)+180.0;
	return temp;
}

float GetRe(float Mag,float Phs)//输出阻抗实部
{
	Phs=Phs*(float)2.0*(float)PI/(float)180.0;
	if(Mag*cos(Phs)<0)
		return -Mag*cos(Phs);
	return Mag*cos(Phs);
}

float GetIm(float Mag,float Phs)//输出阻抗虚部
{
	Phs=Phs*(float)2.0*(float)PI/(float)180.0;
	return Mag*(float)sin(Phs);
}
