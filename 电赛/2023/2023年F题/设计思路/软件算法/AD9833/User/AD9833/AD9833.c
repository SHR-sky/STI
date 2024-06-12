#include "AD9833.h"

//***************************
//		Pin assign	   	
//			STM32			AD9833
//		GPIOB_Pin_15 		---> FSYNC
//		GPIOB_Pin_14 		---> SCK/CLK
//		GPIOB_Pin_13 		---> DAT
//		GPIOB_Pin_12		---> CS
//***************************	

/*�˿ڶ��� */ 
#define PIN_GROUP GPIOB

#define PIN_FSYNC GPIO_Pin_15

#define PIN_SCK GPIO_Pin_14

#define PIN_DAT GPIO_Pin_13

#define PIN_CS GPIO_Pin_12  //���ֵ�λ��Ƭѡ

#define RESET_FSYNC() GPIO_ResetBits(PIN_GROUP, PIN_FSYNC)
#define SET_FSYNC() GPIO_SetBits(PIN_GROUP, PIN_FSYNC)

#define RESET_SCK() GPIO_ResetBits(PIN_GROUP, PIN_SCK)
#define SET_SCK() GPIO_SetBits(PIN_GROUP, PIN_SCK)

#define RESET_DAT() GPIO_ResetBits(PIN_GROUP, PIN_DAT)
#define SET_DAT() GPIO_SetBits(PIN_GROUP, PIN_DAT)

#define RESET_CS() GPIO_ResetBits(PIN_GROUP, PIN_CS)
#define SET_CS() GPIO_SetBits(PIN_GROUP, PIN_CS)

void AD9833InitConfig(void) {
	// ����GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	// ����GPIO�ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = PIN_FSYNC|PIN_SCK|PIN_DAT|PIN_CS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(PIN_GROUP,&GPIO_InitStruct);
}

// �ӳ�
void AD9833_Delay() {
	for(int i=0; i<1; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: AD9833_Write
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: TxData : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD9833_Write(unsigned int TxData)
{
	unsigned char i;

	SET_SCK();
	//AD9833_Delay();
	SET_FSYNC();
	//AD9833_Delay();
	RESET_FSYNC();
	//AD9833_Delay();
	for(i = 0; i < 16; i++)
	{
		if (TxData & 0x8000) // ��ȡ���λ
			SET_DAT(); // ����Ϊ��Ϊ1
		else
			RESET_DAT(); // ����Ϊ��Ϊ0
		
		AD9833_Delay();
		RESET_SCK();
		AD9833_Delay();		
		SET_SCK();
		
		TxData <<= 1;
	}
	SET_FSYNC();
} 

/*
*********************************************************************************************************
*	�� �� ��: AD9833_AmpSet
*	����˵��: �ı�����źŷ���ֵ
*	��    ��: 1.amp ������ֵ  0- 255
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void AD9833_AmpSet(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;
   	
	RESET_CS();
	temp =0x1100|amp;
	for(i=0;i<16;i++)
	{
		RESET_SCK();	
		if(temp&0x8000)
			SET_DAT();
		else
			RESET_DAT();
		temp<<=1;
	    SET_SCK();
	    AD9833_Delay();
	}
	
   	SET_CS();
}

/*
*********************************************************************************************************
*	�� �� ��: AD9833_WaveSeting
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: 1.Freq: Ƶ��ֵ, 0.1 hz - 12Mhz ��λ:HZ
			  2.Freq_SFR: 0 �� 1
			  3.WaveMode: TRI_WAVE(���ǲ�),SIN_WAVE(���Ҳ�),SQU_WAVE(����)
			  4.Phase : ���εĳ���λ
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************����Ƶ�ʵ�16����ֵ***********************************/
		frequence_mid=268435456/25;//�ʺ�25M����
		//���ʱ��Ƶ�ʲ�Ϊ25MHZ���޸ĸô���Ƶ��ֵ����λMHz ��AD9833���֧��25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //���frequence_hex��ֵ��32λ��һ���ܴ�����֣���Ҫ��ֳ�����14λ���д���
		frequence_LSB=frequence_hex; //frequence_hex��16λ�͸�frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;//ȥ�������λ��16λ����ȥ����λ������14λ
		frequence_MSB=frequence_hex>>14; //frequence_hex��16λ�͸�frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;//ȥ�������λ��16λ����ȥ����λ������14λ

		Phs_data=Phase|0xC000;	//��λֵ
		AD9833_Write(0x0100); //��λAD9833,��RESETλΪ1
		AD9833_Write(0x2100); //ѡ������һ��д�룬B28λ��RESETλΪ1

		if(Freq_SFR==0)				  //���������õ�����Ƶ�ʼĴ���0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //ʹ��Ƶ�ʼĴ���0�������
			AD9833_Write(frequence_LSB); //L14��ѡ��Ƶ�ʼĴ���0�ĵ�14λ��������
			AD9833_Write(frequence_MSB); //H14 Ƶ�ʼĴ����ĸ�14λ��������
			AD9833_Write(Phs_data);	//������λ
			//AD9833_Write(0x2000); /**����FSELECTλΪ0��оƬ���빤��״̬,Ƶ�ʼĴ���0�������**/
	    }
		if(Freq_SFR==1)				//���������õ�����Ƶ�ʼĴ���1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//ʹ��Ƶ�ʼĴ���1�������
			AD9833_Write(frequence_LSB); //L14��ѡ��Ƶ�ʼĴ���1�ĵ�14λ����
			AD9833_Write(frequence_MSB); //H14 Ƶ�ʼĴ���1Ϊ
			AD9833_Write(Phs_data);	//������λ
			//AD9833_Write(0x2800); /**����FSELECTλΪ0������FSELECTλΪ1����ʹ��Ƶ�ʼĴ���1��ֵ��оƬ���빤��״̬,Ƶ�ʼĴ���1�������**/
		}

		if(WaveMode==TRI_WAVE) //������ǲ�����
		 	AD9833_Write(0x2002); 
		if(WaveMode==SQU_WAVE)	//�����������
			AD9833_Write(0x2028); 
		if(WaveMode==SIN_WAVE)	//������Ҳ���
			AD9833_Write(0x2000); 

}
