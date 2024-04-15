#include "ADS1118.h"

#include "usart.h"

#include "delay.h"

/*
32-Bitģʽ��CS���ſ���һֱ����Ϊ�ͣ���ʡһ��IO�ڡ�
32-Bitģʽ����ϸ��Ϊ���֣�һ���ǰ����üĴ�����16bit��д�����Σ�һ����д��һ�κ�ڶ��Σ���16bit��д0��

16-BitģʽҪ����ÿ����ͨ��֮��CS��Ƭѡ������Ҫ����һ�Ρ�
ÿ��ͨ�ſ�д��16bit�����üĴ���ֵ�Ͷ�ȡ��16bit��ת���Ĵ���ֵ��

��ʱֻ�о�16bit
*/
#define INPUT  	PAin(4)				// �������� , ����оƬ��OUT		
#define OUTPUT	PAout(5)			// ������� , ����оƬ��DIN
#define CS  	PAout(6)			// Ƭѡ�ź�
#define SCK  	PAout(7)			// ʱ���ź�

#define CS1  	PAout(1)			// Ƭѡ1�ź�

u16 Conversion ;			// �洢��оƬ��ȡ������
float Voltage ;    			// �洢��Ҫ��ʾ�Ĳ�����ѹֵ
float BaseV ;				// �ɼ���ѹ��ѹ��
u8 firstflag ;				// ��һ�ν����־
u8 collect ;				// ÿ�βɼ�������λ��
float DP[8] ;				// ��ʾ�����İ�ͨ������

ConfigDef Config ;


void ADS1118GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out ;		// PA6��ΪƬѡ����źţ�����Ϊ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	GPIO_SetBits(GPIOA,GPIO_Pin_6);							// Ƭѡ��ʼ��Ϊ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out ;		// PA5��Ϊ��������źţ���ʼ����Ϊ�������		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);						// ��������ڳ�ʼ��Ϊ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out ;		// PA7��Ϊʱ���źţ�����Ϊ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);						// ʱ�ӳ�ʼ��Ϊ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out ;		// PA1��ΪƬѡ1����źţ�����Ϊ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	GPIO_SetBits(GPIOA,GPIO_Pin_1);							// Ƭѡ��ʼ��Ϊ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;			// PA4��Ϊ���������źţ�����Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	
}
											
// ��������׼���ü���ʱ�������ű�Ϊ�͵�ƽ
// ������ת��ģʽ�£����δ�������м������κ����ݣ���DOUT / DRDY����һ�����ݾ����źţ�DOUT / DRDYΪ�͵�ƽ��֮ǰ��8 ��s�ٴα�Ϊ�ߵ�ƽ
/**********************************************************
�������ƣ�RWByte
�������ܣ�SPI���ͽ�������
������������Ҫ���õļĴ�������
�������أ��ɼ�����16λ����
����������Display
�������ڣ�2020/04/17  14��23
��    �ߣ�
ע    �⣺
**********************************************************/
u16 RWByte(u16 DATA)
{
	u8 t ;
	u16 returndata ;
	delay_ms(1);
	for(t=0;t<16;t++)										
	{
		if(DATA&0x8000)				// ÿ����ӻ�д��λ����,оƬ��ʱ���½�������DIN����
		{
			OUTPUT = 1 ;
		}
		else
		{
			OUTPUT = 0 ;
		}
		DATA<<=1;
		SCK = 1 ;
		delay_ms(1);
		returndata<<=1;
		if(INPUT == 1)					// ÿ�ζ�ȡ�ӻ��еİ�λ���ݣ�оƬ��ʱ�������ؽ������Ƴ�
		{
			returndata|=0x0001 ;
		}
		SCK = 0 ;
		delay_ms(1);
	}
	SCK = 0 ;
	delay_ms(1);
	OUTPUT = 0 ;
	
	return returndata ;
}

/**********************************************************
�������ƣ�ADS1118Init
�������ܣ���ʼ��ʱ����һЩ�����ı�ļĴ���ֵ
��������������ת��������ģʽ���������ʣ�����ʹ�ܣ���������
�������أ���
����������main
�������ڣ�2020/04/17  14��23
��    �ߣ�
ע    �⣺
**********************************************************/
void ADS1118Init(u8 ss,u8 mode ,u8 dr,u8 pue,u8 nop)
{
	Config.ConfigDef_T.SS = ss ;				// ����Ϊ��Ч��
	Config.ConfigDef_T.MODE = mode ;			// ����Ϊ����ת��ģʽ
	Config.ConfigDef_T.DR = dr ;				// ����ת������Ϊ128 SPS
	Config.ConfigDef_T.PULL_UP_EN = pue ;		// ����DOUT����ʹ��
	Config.ConfigDef_T.NOP = nop ;				// ������Ч���ݣ��������üĴ���
	Config.ConfigDef_T.CNV_RDY_FL = 0x01 ;		// ����λ��ʼ��д1	
	
	Conversion = 0  ;			
	Voltage = 0  ;    			
	BaseV = 0  ;
	firstflag = 0 ;
}

/**********************************************************
�������ƣ�Getdata
�������ܣ����üĴ���ֵ�������ɼ����������ƽ��ֵ
����������ͨ��ѡ�񣬹���ģʽ���������ʣ�����ʹ�ܣ���������
�������أ���
����������main
�������ڣ�2020/04/17  14��23
��    �ߣ�
ע    �⣺
**********************************************************/
void Getdata(u8 mux,u8 pga,u8 tsmode,u8 choose)
{
	float FV[10] ;			// �洢���������ת������
	u8 t ;
	float displaydata ;
	
	Config.ConfigDef_T.MUX = mux ;				// ����ΪAIN0��GND
	Config.ConfigDef_T.PGA = pga ;				// ����FSR=��4.096V
	Config.ConfigDef_T.TS_MODE = tsmode ;		// �����¶ȴ�����ģʽΪADCģʽ	
								
	switch (pga)
	{
		case 0 :
			BaseV = 187.5 ;						// ѹ����λΪuV
			break ;
		case 1 :
			BaseV = 125 ;
			break ;
		case 2 :
			BaseV = 62.5 ;
			break ;
		case 3 :
			BaseV = 31.25 ;
			break ;
		case 4 :
			BaseV = 15.625 ;
			break ;
		case 5 :
			BaseV = 7.8125 ;
			break ;
	}
	for(t=0;t<5;t++)
	{
		switch(choose)
		{
			case CS_0 :
				CS = 0 ;
				CS1 = 1 ;
				break;
			case CS_1 :
				CS = 1 ;
				CS1 = 0 ;
				break;
		}
		delay_ms(1);
		if((INPUT == 0)||(firstflag == 0))									// CS��Ҫ����������������Ƿ����µ����ݲ���(���INPUT�����Ƿ��е͵�ƽ)							
		{
			Conversion = RWByte(Config.Bytes);
			Voltage = (BaseV*Conversion)/1000000 ;			// ת����λ��uV��V	
			Conversion = 0 ;							// ������ʾ֮������	
			firstflag = 1 ;
		}

		CS = 1 ;
		CS1 = 1 ;
		FV[t] = Voltage ;
		delay_ms(15);									// �ӳ�ʱ�䲻�ܵ���15ms
	}
	displaydata = (FV[1]+FV[2]+FV[3]+FV[4] )/4;
	if(choose == CS_0)
	{
		switch(mux)
		{
			case ADS1118_MUX_0G:
				DP[0] = displaydata ;
				break ;
			case ADS1118_MUX_1G:
				DP[1] = displaydata ;
				break ;
			case ADS1118_MUX_2G:
				DP[2] = displaydata ;
				break ;
			case ADS1118_MUX_3G:
				DP[3] = displaydata ;
				break ;
		}
	}
	else if(choose == CS_1)
	{
		switch(mux)
		{
			case ADS1118_MUX_0G:
				DP[4] = displaydata ;
				break ;
			case ADS1118_MUX_1G:
				DP[5] = displaydata ;
				break ;
			case ADS1118_MUX_2G:
				DP[6] = displaydata ;
				break ;
			case ADS1118_MUX_3G:
				DP[7] = displaydata ;
				break ;	
		}
	}

}

void data(void)
{
	u8 x ;
	printf("�ɼ���������Ϊ =  ");
	for(x=0;x<8;x++)
	{
		printf("%3.3f ",DP[x]);
	}
	printf("\r\n");
}