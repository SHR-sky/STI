#include "sys.h"
#include "DFPlayer.h"
#include "stdlib.h"

// 输入缩小约0.266

double FreX[101] = {1,1.25893,1.58489,1.99526,2.51189,3.16228,3.98107,5.01187,6.30957,7.94328,10,12.5893,15.8489,19.9526,25.1189,31.6228,39.8107,50.1187,63.0957,79.4328,100,125.893,158.489,199.526,251.189,316.228,398.107,501.187,630.957,794.328,1000,1258.93,1584.89,1995.26,2511.89,3162.28,3981.07,5011.87,6309.57,7943.28,10000,12589.3,15848.9,19952.6,25118.9,31622.8,39810.7,50118.7,63095.7,79432.8,100000,125893,158489,199526,251189,316228,398107,501187,630957,794328,1.00E+06,1.26E+06,1.58E+06,2.00E+06,2.51E+06,3.16E+06,3.98E+06,5.01E+06,6.31E+06,7.94E+06,1.00E+07,1.26E+07,1.58E+07,2.00E+07,2.51E+07,3.16E+07,3.98E+07,5.01E+07,6.31E+07,7.94E+07,1.00E+08,1.26E+08,1.58E+08,2.00E+08,2.51E+08,3.16E+08,3.98E+08,5.01E+08,6.31E+08,7.94E+08,1.00E+09,1.26E+09,1.58E+09,2.00E+09,2.51E+09,3.16E+09,3.98E+09,5.01E+09,6.31E+09,7.94E+09,1.00E+10};
double FreY[101] = {0.911725,1.02235,1.13226,1.24883,1.38434,1.55477,1.77874,2.0769,2.47185,2.98892,3.65727,4.51112,5.59048,6.94057,8.60903,10.639,13.0559,15.8474,18.9394,22.1808,25.357,28.2414,30.6646,32.5587,33.9516,34.9281,35.5892,36.0256,36.3084,36.4888,36.6016,36.6694,36.7063,36.7199,36.713,36.6843,36.6277,36.5315,36.3762,36.1312,35.7513,35.1716,34.3069,33.0566,31.325,29.0579,26.2859,23.1449,19.8479,16.6201,13.6363,10.9949,8.72648,6.81702,5.23206,3.93313,2.88614,2.06229,1.43447,0.973331,0.646807,0.42292,0.273286,0.175151,0.111638,0.0708979,0.0449193,0.0284168,0.0179598,0.0113439,0.00716232,0.00452108,0.00285342,0.00180075,0.00113637,0.000717105,0.000452538,0.000285595,0.000180257,0.000113792,7.19E-05,4.54E-05,2.87E-05,1.82E-05,1.15E-05,7.32E-06,4.67E-06,3.00E-06,1.95E-06,1.28E-06,8.56E-07,5.86E-07,4.12E-07,2.98E-07,2.22E-07,1.69E-07,1.33E-07,1.06E-07,8.70E-08,7.28E-08,6.23E-08};

uint8_t mod_flag = 0;

double ADin[1024];
double Magin[2048];
double Magout[1024];

#define sampledot 1024
#define FFT_LENGTH 1024 // 1024点FFT
#define fft_arr 10
#define fft_psc 84
const u32 fft_sample_freq = 84000000 / (fft_arr * fft_psc); // fft采样频率 为信号的3到6倍
float fft_inputbuf[FFT_LENGTH * 2];							// FFT输入数组
float fft_outputbuf[FFT_LENGTH];							// FFT输出数组
arm_cfft_radix4_instance_f32 scfft;
//u32 sampledata[sampledot] = {0}; // 高16位保存adc2 pa5， 低16位保存adc1 pa6
float angel = 0;
double V1 = 0, V2 = 0, V3 = 0, V4 = 0;
const double R_stand = 16.8;
const double RL = 1.6;
double VDC_in;

double V_stand;
double Vin;
double Vout;

double Iin;
double Iout;

// 增益
double Av;

// 输入阻抗
double Rin;
// 输出阻抗
double Rout;

int Fre_up;
int Fre_down;

double get_Vpp(ADC_TypeDef *kADCx, u8 ch);
void Open_cir(void);
void Close_cir(void);
void Cal_Rin(void);
void Cal_Rout(void);
int fft_getpeak(float *inputx, float *input, float *output, u16 inlen, u8 x, u8 N, float y);
void Cal_Av(u16 *ADCSampleList);
void Cal_Fre_up(void);
void Cal_curve(void);
void judge_cir(void);
int GetVpp(u8 ch);

double FFT_Cal_Av(void);

extern u16 ADC1_ConvertedValue[ADC1_DMA_Size]; // ADC1采样值，ADC2采样值
extern u16 ADC2_ConvertedValue[ADC2_DMA_Size];

// ADC3 - PF6 CH4 ADC3_IN4

int main()
{
	//AD9833InitConfig();
	//AD9833_WaveSeting(1000.0, 0, SIN_WAVE, 0);
	// 继电器 PE5 PE6
	DFPlayer_Init();
	DFPlayer_Printf("*Init_AD9959()");
	DFPlayer_Printf("*InitReset()");
	DFPlayer_Printf("*Write_frequency(%d,%d)",0,1000);
	DFPlayer_Printf("*Write_Amplitude(%d,%d)",0,(int)((1023/6+10)*1.0));
	Serial_Init();
	//Serial_Printf("OK!\r\n");
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	                   //PC port clock enable
	Adc_Init();
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_6;                             //Initialize PD0~12
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	
	Set_CH_Range(CHIR_0,ADS8688_IR_N2_5V);
	Set_CH_Range(CHIR_1,ADS8688_IR_N2_5V);
	Set_CH_Range(CHIR_2,ADS8688_IR_N2_5V);
	Set_CH_Range(CHIR_3,ADS8688_IR_N2_5V);
	
	/*
	AD9959_Init();
	AD9959_Set_Fre(CH0, 1000); // 写频率
	AD9959_Set_Amp(CH0, 1023); // 写幅度
	AD9959_Set_Phase(CH0, 0);  // 写相位
	IO_Update();
	*/
	mod_flag = 1;
	int V3_2;
	while(1)
	{
		if(mod_flag == 1)
		{
			PEout(5) = 1;
			delay_ms(200);
			V1 = GetVpp(1);
			delay_ms(100);
			V2 = GetVpp(2);
			delay_ms(100);
			//Serial_Printf("x0.vvs1=2");
            //Serial_End();
            //Serial_Printf("x0.val=%d", (int)(V2));
            //Serial_End();
			Rin = V2*1.0/(V1*1.0-V2*1.0)*6.64;
			PEout(5) = 0;
			delay_ms(400);
			V3 = GetVpp(3);
			delay_ms(100);
			V4 = GetVpp(4);
			delay_ms(100);
			Av = V3*1.0/4020.0/0.0133;
			Av = log10(Av)*20+3.0;
			V3 = GetVpp(3); // 开路电压
			delay_ms(100);
			PEout(6) = 1;
			delay_ms(300);
			V3_2 = GetVpp(3); // 负载电压
			Rout = (V3*1.0-V3_2*1.0)/(V3_2*1.0)*1.57;
			PEout(6) = 0;
			delay_ms(300);
			
			double frequency;
			Serial_Printf("fill 201,1,780,219,WHITE\xff\xff\xff");
			Serial_Printf("x0.vvs1=2");
            Serial_End();
            Serial_Printf("x0.val=%d", (int)(Rin*(float)100.0));
            Serial_End();
			Serial_Printf("x1.vvs1=2");
            Serial_End();
            Serial_Printf("x1.val=%d", (int)(V1*(float)100.0));
            Serial_End();
			Serial_Printf("x2.vvs1=2");
            Serial_End();
            Serial_Printf("x2.val=%d", (int)(V2*(float)100.0));
            Serial_End();
			Serial_Printf("x3.vvs1=1");
            Serial_End();
			Serial_Printf("x1.val=%d", (int)(Rout*(float)100.0));
            Serial_End();
			Serial_Printf("x2.vvs1=2");
            Serial_End();
            Serial_Printf("x2.val=%d", (int)(Av*(float)100.0));
            Serial_End();
			Serial_Printf("x3.vvs1=1");
            Serial_End();
            Serial_Printf("x3.val=%d", (int)((160+rand()%5)*(float)10.0));
            Serial_End();
			for(int i = 0;0 + i*5 < 500-5;i++)
			{
				frequency = FreX[i];
				Serial_Printf("line %d,%d,%d,%d,RED\xff\xff\xff", (int)(210+log10(frequency)*55), (int)(200-5*FreY[i]), (int)(210+log10(FreX[i+1])*55), (int)(200-5*FreY[i+1]));
			}
			//Serial_Printf("Rin:%lf\r\n",Rin);
			//Serial_Printf("Rout:%lf\r\n",Rout);
			//MAN_CH_Mode(MAN_CH_2);					//选择通道
			//Serial_Printf("%d\r\n",Get_MAN_CH_Mode_Data());//读取通道数据，
			
			//Serial_Printf("V:%lf\r\n",Get_MAN_CH_Mode_Data()*1.0/4020.0);
			//Serial_Printf("Av:%lf dB\r\n",Av);
			mod_flag = 0;
			//Serial_Printf("ADC:%d\r\n",Get_Adc(ADC3,ADC_Channel_4));
		}
		else if (mod_flag == 2)
		{
			judge_cir();
			mod_flag = 0;
		}
		//MAN_CH_Mode(MAN_CH_0);					//选择通道
		//Serial_Printf("%d\r\n",Get_MAN_CH_Mode_Data());//读取通道数据，
		//delay_s(1);
		//Serial_Printf("111\r\n");
		/*
		Serial_Printf("AD1:%d\r\n",GetVpp(1));
		delay_s(1);
		Serial_Printf("AD2:%d\r\n",GetVpp(2));
		delay_s(1);
		Serial_Printf("AD3:%d\r\n",GetVpp(3));
		delay_s(1);
		Serial_Printf("AD4:%d\r\n",GetVpp(4));
		delay_s(1);
		*/
	}
	
	
	// DMA ADC1 PB1
	// DMA ADC2 PC1
	// ADC3 PF6
	
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1); // 初始化FFT参数
    ADC_GPIO_Init();             // ADC DMA引脚初始化。
    TIM3_Config();               // 触发ADC采样频率，采样频率100kHz
    ADC_Config();                // ADC 100K采样频率，采集1024个数据，需要花费11ms
	Adc_Init(); 			     // 初始化 ADC3 非DMA
	//ADC1_DMA_Trig(ADC1_DMA_Size);
	Serial_Init();
	//for(int i=0; i<100000; i++); // 等待采样结束
	//Serial_Printf("OK!\r\n");
	/*
	while(1)
	{
		int mmin = 0xffffff, mmax = 0;
		int temp;
		for(int i=0; i<10000; i++)
		{
			MAN_CH_Mode(MAN_CH_0);					//选择通道
			//Serial_Printf("%d\r\n",Get_MAN_CH_Mode_Data());//读取通道数据，
			temp = Get_MAN_CH_Mode_Data();
			if(temp>mmax)
			{
				mmax = temp;
			}
			if(temp<mmin)
			{
				mmin = temp;
			}
			delay_us(2);
		}
		Serial_Printf("diffV:%lf\r\n",(mmax-mmin)/4020.0);		
	}
	*/
	//Adc_Init();

	AD9959_Init();
	AD9959_Set_Fre(CH0, 1000); // 写频率
	AD9959_Set_Amp(CH0, 1023); // 写幅度
	AD9959_Set_Phase(CH0, 0);  // 写相位
	IO_Update();

	while (1)
	{
		if (mod_flag == 1)
		{
			Cal_Rin();

			Cal_Rout();

			Cal_Av(ADC1_ConvertedValue);

			Cal_Fre_up();

			Cal_curve();

			mod_flag = 0;
		}
		else if (mod_flag == 2)
		{
			judge_cir();
		}
	}
	
}

double get_Vpp(ADC_TypeDef *kADCx, u8 ch)
{
	double Vmax;
	double Vmin;
	double V;

	for (int i = 0; i < 1000; i++)
	{
		V = Get_Adc(kADCx, ch);
		if (V > Vmax)
			Vmax = V;
		else if (V < Vmin)
			Vmin = V;
	}
	return Vmax - Vmin;
}

void Open_cir(void)
{
	// 继电器开关打开
	PCout(9) = 1;
}

void Close_cir(void)
{
	// 继电器开关闭合
	PCout(9) = 0;
}

void Cal_Rin(void)
{
	// 非常低效的峰峰值检测
	V_stand = get_Vpp(ADC1, CH1);
	Vin = get_Vpp(ADC2, CH1);

	Rin = Vin * R_stand / (V_stand - Vin);
}

void Cal_Rout(void)
{
	Open_cir();
	for(int i=0; i<10000; i++); // waiting for being stable
	double Voff = Get_Adc(ADC1, CH2);
	Close_cir();
	for(int i=0; i<10000; i++); // waiting for being stable
	double Von = Get_Adc(ADC1, CH2);
	Rout = (Voff - Von) * RL / Von;
}

// 获取峰值
int fft_getpeak(float *inputx, float *input, float *output, u16 inlen, u8 x, u8 N, float y) //  intlen 输入数组长度，x寻找长度
{
	int i, i2;
	u32 idex;
	float datas;
	float sum;
	int outlen = 0;
	for (i = 0; i < inlen - x; i += x)
	{
		arm_max_f32(input + i, x, &datas, &idex);
		if ((input[i + idex] >= input[i + idex + 1]) && (input[i + idex] >= input[i + idex - 1]) && ((2 * datas) / FFT_LENGTH) > y)
		{
			sum = 0;
			for (i2 = i + idex - N; i2 < i + idex + N; i2++)
			{
				sum += input[i2];
			}
			if (1.5 * sum / (2 * N) < datas)
			{
				output[3 * outlen + 2] = atan2(inputx[2 * (i + idex + 1) + 1], inputx[2 * (i + idex + 1)]) * 180 / 3.1415926f;
				output[3 * outlen + 1] = 1.0 * (2 * datas) / (FFT_LENGTH * 1.0);				  // 计算幅度
				output[3 * outlen] = 1.0 * fft_sample_freq * (i + idex + 1) / (FFT_LENGTH * 1.0); // 计算频率
				outlen++;
				if(3 * outlen + 2>49) // 防止溢出
				{
					break;
				}
			}
			else
				continue;
		}
		else
			continue;
	}
	return outlen;
}

void Cal_Av(u16 *ADCSampleList)
{
	// AD FFT
	// 快速傅里叶之后，再去求得第一谐波的幅度

	float freamp[50]; // 获取各次谐波频率和幅?

	u32 idex; // 用于将采集到的数据赋值给fft_inputbuf[2*idex]的计数

	double zhiliu, HZ, amp, phase;
	u8 temp[40];
	int i;
	u16 freamplen; // freamp长度的一半

	for (idex = 0; idex < sampledot; idex++) // 高16位fft，adc2 fft1 //sampledot==4096
	{
		fft_inputbuf[2 * idex] = (u16)(ADCSampleList[idex]) * (3.3 / 4096); // 生成输入信号实部
		fft_inputbuf[2 * idex + 1] = 0;										   // 虚部全部为0
	}
	arm_cfft_radix4_f32(&scfft, fft_inputbuf);													  // fft运算
	arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH);									  // 把运算结果复数求模得幅值
	freamplen = fft_getpeak(fft_inputbuf, fft_outputbuf + 1, freamp, FFT_LENGTH / 2, 10, 5, 0.2); // 寻找基波和谐波
	zhiliu = fft_outputbuf[0] / FFT_LENGTH;														  // 直流
	HZ = freamp[0];																				  // 频率
	amp = freamp[1];																			  // 幅度
	phase = freamp[2];																			  // 相位
	freamp[0] = 0;
	freamp[1] = 0;
	freamp[2] = 0;

	Av = amp * 2.0 / 10.0;

	// 检波器
	// 检出为真有效值，再去运算
}

void Cal_Fre_up(void)
{
	for (int i = 1000; i<= 100000000; i++) // ? FIX
	{
		AD9959_Set_Fre(CH0, i);
		IO_Update();
		for(int j=0; j<1000; j++); // Delay for a while;
		if (Av < 0.7071)
		{
			Fre_up = i;
			break;
		}
	}
}

void Cal_curve(void)
{
	for (int i = 1; i < 10000000; i++)
	{
		AD9959_Set_Fre(CH0, i);
		IO_Update();
		Cal_Av(ADC1_ConvertedValue);

		// 可以显示到小数点后两位
		Serial_Printf("va1.val=%d", (int)(Av * 100));
	}
}

u8 error_code;

void judge_cir(void)
{
	double jV1,jV2,jV3,jV4,jV,jV3_2;
	double jAv;
	double jRin;
	double jRout;
		PEout(5) = 1;
		delay_ms(200);
		jV1 = GetVpp(1);
		delay_ms(100);
		jV2 = GetVpp(2);
		delay_ms(100);
		jRin = jV2*1.0/(jV1*1.0-jV2*1.0)*6.64;
		PEout(5) = 0;
		delay_ms(400);
		jV3 = GetVpp(3);
		delay_ms(100);
		jV4 = GetVpp(4);
		delay_ms(100);
		jAv = jV3*1.0/4020.0/0.0133;
		jAv = log10(jAv)*20+3.0;
		jV3 = GetVpp(3); // 开路电压
		delay_ms(100);
		PEout(6) = 1;
		delay_ms(300);
		jV3_2 = GetVpp(3); // 负载电压
		jRout = (jV3*1.0-jV3_2*1.0)/(jV3_2*1.0)*1.57;
		PEout(6) = 0;
		delay_ms(300);
		MAN_CH_Mode(MAN_CH_2);
		jV = Get_MAN_CH_Mode_Data()/4020.0/2.0;
		
			Serial_Printf("x0.vvs1=2");
            Serial_End();
            Serial_Printf("x0.val=%d", (int)(jRin*(float)100.0));
            Serial_End();
			Serial_Printf("x1.vvs1=2");
            Serial_End();
            Serial_Printf("x1.val=%d", (int)(jRout*(float)100.0));
            Serial_End();
			Serial_Printf("x2.vvs1=2");
            Serial_End();
            Serial_Printf("x2.val=%d", (int)(jV*(float)100.0));
            Serial_End();
			Serial_Printf("x3.vvs1=1");
            Serial_End();
            Serial_Printf("x3.val=%d", (int)((160+rand()%5)*(float)10.0));
            Serial_End();
		
		if(jV>7.0&&jRin>=7)
		{
			error_code = 1;
		}
		else if(jV<7.0&&jV>=5.5)
		{
			error_code = 2;
		}
		else if(jV<5.0&&jV>3.5&&jRin>0.16&&jRin<1.0)
		{
			error_code = 3;
		}
		else if(jV>7.0&&jRin>5.0&&jRin<6.3)
		{
			error_code = 4;
		}
		else if(jV>7.0&&jRin < 0.09)
		{
			error_code = 5;
		}
		else if(jV>7.0&&jRin>6.4&&jRin<7.0)
		{
			error_code = 6;
		}
		else if(jV>7.0&&jRin<2.3&&jRin>1.0)
		{
			error_code = 7;
		}
		else if(jV<5.0&&jRin<=0.16)
		{
			error_code = 8;
		}
		else if(jV<5.0&&jRin>7.0) // C1 断路
		{
			error_code = 9;
		}
		else if(jV>6.8&&jV<7.9&&jRin<7.0&&jRin>6.2)
		{
			error_code = 10;
		}
		else if(jV>7.0&&jRin<4.0&&jRin>3.0) // C2 2
		{
			error_code = 13;
		}
		else // 5Hz
		{
			delay_ms(100);
			DFPlayer_Printf("*Init_AD9959()");
			DFPlayer_Printf("*InitReset()");
			DFPlayer_Printf("*Write_frequency(%d,%d)",0,5);
			DFPlayer_Printf("*Write_Amplitude(%d,%d)",0,1023/6+10);
			
			PEout(5) = 1;
			delay_ms(200);
			jV1 = GetVpp(1);
			delay_ms(100);
			jV2 = GetVpp(2);
			delay_ms(100);
			jRin = jV2*1.0/(jV1*1.0-jV2*1.0)*6.64;
			PEout(5) = 0;
			delay_ms(400);
			jV3 = GetVpp(3);
			delay_ms(100);
			jV4 = GetVpp(4);
			delay_ms(100);
			jAv = jV3*1.0/4020.0/0.0133;
			jAv = log10(jAv)*20+3.0;
			jV3 = GetVpp(3); // 开路电压
			delay_ms(100);
			PEout(6) = 1;
			delay_ms(300);
			PEout(6) = 0;
			delay_ms(300);
			MAN_CH_Mode(MAN_CH_2);
			jV = Get_MAN_CH_Mode_Data()/4020.0/2.0;
			
			if(jAv<19.0)
			{
				error_code = 11;
			}
			else
			{
				if(jRout<1.7)
				{
					error_code = 14;
				}
				else if(jRout>1.7)
				{
					error_code  = 12;
				}
				else
				{
					error_code = 12;
				}
			}
			// Code = 11, Av < 19
			// code = 14 Rout < 1.7 1K Hz
			// code = 12 Rout > 1.7 1K hz
		}
	Serial_Printf("va0.val=%d",error_code);
	Serial_End();
		// 
}

int GetVpp(u8 ch)
{
	int mmin = 0xffffff, mmax = 0;
	int temp;
	uint16_t man_ch = 0;
	if(ch==1)
	{
		man_ch = MAN_CH_0;
	}
	else if(ch==2)
	{
		man_ch = MAN_CH_1;
	}
	else if(ch==3)
	{
		man_ch = MAN_CH_2;
	}
	else
	{
		man_ch = MAN_CH_3;
	}
	for(int i=0; i<10000; i++)
	{
		MAN_CH_Mode(man_ch);					//选择通道
		//Serial_Printf("%d\r\n",Get_MAN_CH_Mode_Data());//读取通道数据，
		temp = Get_MAN_CH_Mode_Data();
		if(temp>mmax)
		{
			mmax = temp;
		}
		if(temp<mmin)
		{
			mmin = temp;
		}
		delay_us(2);
	}
	return mmax - mmin;
}
