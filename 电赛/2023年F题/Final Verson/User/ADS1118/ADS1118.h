#ifndef __ADS1118_H
#define __ADS1118_H

#include "sys.h"


/**����ת������**/
#define ADS1118_SS_NONE  0		// ��Ч
#define ADS1118_SS_ONCE  1		// ��������ת��

/**�����·����������**/
#define ADS1118_MUX_01	0		// 000 = AINP Ϊ AIN0 �� AINN Ϊ AIN1��Ĭ�ϣ�
#define ADS1118_MUX_03	1		// 000 = AINP Ϊ AIN0 �� AINN Ϊ AIN3
#define ADS1118_MUX_13	2		// 000 = AINP Ϊ AIN1 �� AINN Ϊ AIN3
#define ADS1118_MUX_23	3		// 000 = AINP Ϊ AIN2 �� AINN Ϊ AIN3
#define ADS1118_MUX_0G	4		// 000 = AINP Ϊ AIN0 �� AINN Ϊ GND
#define ADS1118_MUX_1G	5		// 000 = AINP Ϊ AIN1 �� AINN Ϊ GND
#define ADS1118_MUX_2G	6		// 000 = AINP Ϊ AIN2 �� AINN Ϊ GND
#define ADS1118_MUX_3G	7		// 000 = AINP Ϊ AIN3 �� AINN Ϊ GND

/**�ɱ������Ŵ�������**/
#define ADS1118_PGA_61  0		// 000 = FSR Ϊ ��6.144V
#define ADS1118_PGA_40  1		// 001 = FSR Ϊ ��4.096V
#define ADS1118_PGA_20  2		// 010 = FSR Ϊ ��2.048V��Ĭ�ϣ�
#define ADS1118_PGA_10  3		// 011 = FSR Ϊ ��1.024V
#define ADS1118_PGA_05  4		// 100 = FSR Ϊ ��0.512V
#define ADS1118_PGA_02  5		// 101 = FSR Ϊ ��0.256V

/**��������ģʽ����**/
#define ADS1118_MODE_LX  0		// ����ת��ģʽ
#define ADS1118_MODE_DC  1 		// �ϵ粢���õ���ת��ģʽ��Ĭ�ϣ�

/**���ݴ�������**/
#define ADS1118_DR_8      0		// 000 = 8SPS
#define ADS1118_DR_16     1		// 001 = 16SPS
#define ADS1118_DR_32     2		// 010 = 32SPS
#define ADS1118_DR_64     3		// 011 = 64SPS
#define ADS1118_DR_128    4		// 100 = 128SPS��Ĭ�ϣ�
#define ADS1118_DR_250    5		// 101 = 250SPS
#define ADS1118_DR_475    6		// 110 = 475SPS
#define ADS1118_DR_860    7		// 111 = 860SPS

/**�¶ȴ�����ģʽ**/
#define ADS1118_TS_MODE_ADC		0		// 0 = ADC ģʽ��Ĭ�ϣ�
#define ADS1118_TS_MODE_T		1		// 1 = �¶ȴ�����ģʽ

/**����ʹ��**/
#define ADS1118_PULL_UP_EN_N	0		// ���� DOUT/DRDY ���ŵ���������
#define ADS1118_PULL_UP_EN_E	1		// ʹ�� DOUT/DRDY ���ŵ��������裨Ĭ�ϣ�

/**���������Ƿ�д�����üĴ���**/
#define ADS1118_NOP_N	0		// 00 = ��Ч���ݣ� ���������üĴ�������
#define ADS1118_NOP_W	1		// 01 = ��Ч���ݣ� �������üĴ�����Ĭ�ϣ�

/**����**/
#define ADS1118_CNV_RDY_FL    1		    // ʼ��д�� 1h


/***************************����ADS1118�е��ĸ�16λ�Ĵ���********************************/
typedef union
{
	struct
	{
		u16 CNV_RDY_FL 	: 1 ; 			// [0]		ת����ɱ�־
		u16 NOP			: 2 ; 			// [1:2]	�޲���
		u16 PULL_UP_EN 	: 1 ; 			// [3]		����ʹ��
		u16 TS_MODE 	: 1 ; 			// [4]		�¶ȴ�����ģʽ
		u16 DR 			: 3 ;      		// [7:5]	���ݴ�������
		u16 MODE 		: 1 ;     		// [8]		�豸����ģʽ
		u16 PGA 		: 3 ;     		// [11:9]	�ɱ������Ŵ�������
		u16 MUX 		: 3 ;     		// [14:12]	�����·����������
		u16 SS 			: 1 ;     		// [15]		����״̬�򵥴�ת����ʼ
	}
	ConfigDef_T ;
	u16 Bytes ;
}
ConfigDef ;

typedef enum
{
	CS_0 = 0 ,
	CS_1
}
chipselect;

/***************************����ADS1118�е��ĸ�16λ�Ĵ���********************************/
extern ConfigDef Config ;




void ADS1118GPIOInit(void);
void ADS1118Init(u8 ss,u8 mode ,u8 dr,u8 pue,u8 nop) ;
void Getdata(u8 mux,u8 pga,u8 tsmode,u8 choose) ;
void data(void);

#endif