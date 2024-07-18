#include "sys.h"

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

double FFT_Cal_Av(void);

extern u16 ADC1_ConvertedValue[ADC1_DMA_Size]; // ADC1采样值，ADC2采样值
extern u16 ADC2_ConvertedValue[ADC2_DMA_Size];

int main()
{
	// 继电器 PC9
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	                   //PC port clock enable

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;                             //Initialize PD0~12
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	ADS8688_Init(CH0_EN|CH1_EN);
	Set_CH_Range(CHIR_0,ADS8688_IR_N2_5V);
	Set_CH_Range(CHIR_1,ADS8688_IR_N2_5V);
	Serial_Init();
	
	/*
	while(1)
	{
		MAN_CH_Mode(MAN_CH_0);					//选择通道
		Serial_Printf("%d\r\n",Get_MAN_CH_Mode_Data());//读取通道数据，
		delay_s(1);
		Serial_Printf("111\r\n");
	}
	*/
	
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
	Serial_Printf("OK!\r\n");
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

void judge_cir(void)
{
	
}
