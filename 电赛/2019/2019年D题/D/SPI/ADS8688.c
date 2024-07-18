/**********************************************************
                       ��������
���������ѧϰʹ�ã�δ��������ɣ��������������κ���;										 
���ܣ�ADS8688��16λAD 
			��ʾ��12864cog
�ӿڣ��ӿ������ads8688.h
ʱ�䣺2023/06/07
�汾��1.1
���ߣ���������
������

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com

**********************************************************/
#include "ADS8688.h"

/**********************************************************************************************************
*	�� �� ��: ADS8688_IO_Init
*	����˵��: ADS8688 IO�ڳ�ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void ADS8688_IO_Init(void)	   //ADS8688 IO�ڳ�ʼ��
{
	/*
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ�ܶ˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	*/

	
	
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
/************��ʱ��*************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);  
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE ); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	
/************��������*************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //SPI_NSS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //�����������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		//SPI_SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //�����������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;		//SPI_MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //�������� GPIO_Mode_IN
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // GPIO_PuPd_NOPULL
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;        //SPI_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //�����������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); // ����ΪSPI1

/************SPI��ʼ������*************/
 // ����ΪSPI1	SPI_Cmd(SPI1, DISABLE);	 //�����Ƚ���,���ܸı�MODE
	SPI_InitStructure.SPI_Direction =SPI_Direction_2Lines_FullDuplex; //����ȫ˫��
	SPI_InitStructure.SPI_Mode =SPI_Mode_Master; //��
	SPI_InitStructure.SPI_DataSize =SPI_DataSize_8b; //8λ
	SPI_InitStructure.SPI_CPOL =SPI_CPOL_Low; //CPOL=0
	SPI_InitStructure.SPI_CPHA =SPI_CPHA_1Edge; //CPHA=0
	SPI_InitStructure.SPI_NSS =SPI_NSS_Hard; //Ӳ��NSS ����Ҫ����CS
	// ��Ҫ��Ϊ����SPI_Cmd(SPI1, ENABLE)��ر�
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_4; //4��Ƶ
	SPI_InitStructure.SPI_FirstBit =SPI_FirstBit_MSB; //��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial =7; //CRC7
	SPI_Init(SPI1,&SPI_InitStructure);
	
// 	SPI_Cmd(SPI1, ENABLE);			//�Ȳ���SPI
 	SPI_SSOutputCmd(SPI1, ENABLE);   		//SPI��NSS���ſ��ƿ���
	SPI_CalculateCRC(SPI2,DISABLE);
	SPI_ClearFlag(SPI1,SPI_FLAG_TXE);
	SPI_ClearFlag(SPI1,SPI_FLAG_RXNE);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);

	/*
	DAISY_L;
	CS_H;
	RST_H;	
	CLK_L;
	delay_ms(5);
	*/
}
/**********************************************************************************************************
*	�� �� ��: ADS8688_Init
*	����˵��: ADS8688��ʼ��
*	��    ��: ch_en��Ҫʹ�ܵ�ͨ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void ADS8688_Init(uint8_t ch_en)	   //ADS8688��ʼ��
{
	ADS8688_IO_Init();
	SOFT_RESET(); //��λ
	delay_ms(25);
	Set_Auto_Scan_Sequence(ch_en);//ʹ��ͨ��
	ADS8688_WriteReg(CH_PD,~ch_en);//ͨ���˳��͹���״̬ ͨ���ϵ�
	delay_ms(50);
	while(ADS8688_ReadReg(AUTO_SEQ_EN)!=ch_en)//�Ƿ�ʹ�ܳɹ�
	{
		Set_Auto_Scan_Sequence(ch_en);//ʹ��ͨ��
		ADS8688_WriteReg(CH_PD,~ch_en);//ͨ���˳��͹���״̬ ͨ���ϵ�
		delay_ms(50);
	}		
}

/**********************************************************************************************************
*	�� �� ��: ADS8688_SPI_Read8Bit
*	����˵��: ��SPI���߶�8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
/*
uint8_t ADS8688_SPI_Read8Bit(void)
{
	uint8_t i=0;
  uint8_t read=0;
  for(i=0;i<8;i++)
  {
    read<<=1;
    CLK_H;
		delay_us(2);
    if(READ_SDO) read++;
    CLK_L;
		delay_us(2);
  }
  return read;
}
*/
/**********************************************************************************************************
*	�� �� ��: ADS8688_SPI_Write8Bit
*	����˵��: ��SPI����д8��bit���ݡ� ����CS���ơ�
*	��    ��: data : ����
*	�� �� ֵ: ��
**********************************************************************************************************/
/*
void ADS8688_SPI_Write8Bit(uint8_t data)
{
	uint8_t i=0;
  for(i=0;i<8;i++)
  {
    if(data&0x80)	SDI_H;
    else	SDI_L;
		data<<=1;
    CLK_H;
		delay_us(2);
    CLK_L;
		delay_us(2);
  }
}
*/
/**********************************************************************************************************
*	�� �� ��: ADS8688_WriteCmd
*	����˵��: д����Ĵ���
*	��    ��:  cmd : ����
*	�� �� ֵ: ��
**********************************************************************************************************/
void ADS8688_WriteCmd(uint16_t cmd)//дADS8688����Ĵ���
{
	SPI_Cmd(SPI1,ENABLE);
	SPI1_WriteData(cmd>>8 & 0XFF);
	SPI1_WriteData(cmd & 0XFF);
	SPI1_WriteData(0x00);
	SPI1_WriteData(0x00);
	SPI_Cmd(SPI1,DISABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: SOFT_RESET
*	����˵��: �����λ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SOFT_RESET(void)//�����λ
{
  ADS8688_WriteCmd(RST);
}

/**********************************************************************************************************
*	�� �� ��: AUTO_RST_Mode
*	����˵��: �Զ�ģʽ
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void AUTO_RST_Mode(void)//�Զ�ɨ��ģʽ
{
  ADS8688_WriteCmd(AUTO_RST);
}
/**********************************************************************************************************
*	�� �� ��: MAN_CH_Mode
*	����˵��: �ֶ�����ɨ��ģʽͨ��ѡ��
*	��    ��:  ch : Ҫѡ���ͨ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void MAN_CH_Mode(uint16_t ch)//�ֶ�����ɨ��ģʽ
{
  ADS8688_WriteCmd(ch);	
}

/**********************************************************************************************************
*	�� �� ��: ADS8688_ReadReg
*	����˵��: ��ָ���ļĴ���
*	��    ��:  addr : �Ĵ�����ַ
*	�� �� ֵ: data : �����ļĴ���ֵ��
**********************************************************************************************************/
uint8_t ADS8688_ReadReg(uint8_t addr)
{
  uint8_t data = 0;
	SPI_Cmd(SPI1,ENABLE);
	SPI1_WriteData(addr<<1);
	data = SPI1_ReadData();
	data = SPI1_ReadData();
	SPI_Cmd(SPI1,DISABLE);
  return data;
}
/**********************************************************************************************************
*	�� �� ��: ADS8688_WriteReg
*	����˵��: дָ���ļĴ���
*	��    ��:  addr : �Ĵ�����ַ
*			  		data : �Ĵ���ֵ
*	�� �� ֵ: ��
**********************************************************************************************************/
void ADS8688_WriteReg(uint8_t addr,uint8_t data)
{
	SPI_Cmd(SPI1,ENABLE);
	SPI1_WriteData(addr<<1| 0X01);
	SPI1_WriteData(data);
	SPI_Cmd(SPI1,DISABLE);
}
/**********************************************************************************************************
*	�� �� ��: Set_Auto_Scan_Sequence
*	����˵��: �����Զ�ɨ������ͨ��
*	��    ��:  seq����Ҫ�����Զ�ɨ������ʹ�ܼĴ�����ֵ
*	�� �� ֵ: ��
**********************************************************************************************************/
void Set_Auto_Scan_Sequence(uint8_t seq)	
{
	ADS8688_WriteReg(AUTO_SEQ_EN, seq);
}
/**********************************************************************************************************
*	�� �� ��: Set_CH_Range
*	����˵��: ����ָ��ͨ��������Χ
*	��    ��:  ch : ͨ��
*			  		range : ������Χ
*	�� �� ֵ: ��
**********************************************************************************************************/
void Set_CH_Range(uint8_t ch,uint8_t range)
{
  ADS8688_WriteReg(ch,range);
}

/**********************************************************************************************************
*	�� �� ��: Get_AUTO_RST_Mode_Data
*	����˵��: ��ȡ�Զ�ɨ��ģʽADֵ
*	��    ��:data : ����ָ��
*			  	chnum : ͨ������
*	�� �� ֵ: ��
**********************************************************************************************************/
void Get_AUTO_RST_Mode_Data(uint16_t *data, uint8_t chnum)
{
	uint8_t i=0,datal=0,datah=0;
	for (i=0; i<chnum; i++)
	{
		SPI_Cmd(SPI1,ENABLE);
		SPI1_WriteData(0x00);
		SPI1_WriteData(0x00);
		datah = SPI1_ReadData();
		datal = SPI1_ReadData();
		//delay_ms(1);
		SPI_Cmd(SPI1,DISABLE);
		*(data+i) = datah<<8 | datal;
	}
}
/**********************************************************************************************************
*	�� �� ��: Get_MAN_CH_Mode_Data
*	����˵��: ��ȡ����ɨ��ģʽADֵ
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
uint16_t Get_MAN_CH_Mode_Data(void)
{
	u8 datah=0,datal=0;
	SPI_Cmd(SPI1,ENABLE);
	SPI1_WriteData(0x00);
	SPI1_WriteData(0x00);
	datah = SPI1_ReadData();
	datal = SPI1_ReadData();
	SPI_Cmd(SPI1,DISABLE);
	return (datah<<8 | datal);
}

//int main(void)//8ͨ���Զ�ɨ��ת��ʾ��
//{
////  ADS8688_Init();
//  
//  ADS8688_Write_Program_Register(0x02,0x00);//����ͨ���˳��͹���״̬
//  ADS8688_Write_Program_Register(0x01,0xff);//ʹ������ͨ��
//  
//  Set_CH_Range_Select(0X05,0x02);//����ͨ��0�����뷶Χ��0.625*Vref
//  Set_CH_Range_Select(0X06,0x02);//����ͨ��1�����뷶Χ��0.625*Vref
//  Set_CH_Range_Select(0X07,0x02);//����ͨ��2�����뷶Χ��0.625*Vref
//  Set_CH_Range_Select(0X08,0x02);//����ͨ��3�����뷶Χ��0.625*Vref
//  Set_CH_Range_Select(0X09,0x02);//����ͨ��4�����뷶Χ��0.625*Vref
//  Set_CH_Range_Select(0X0a,0x02);//����ͨ��5�����뷶Χ��0.625*Vref
//  Set_CH_Range_Select(0X0b,0x02);//����ͨ��6�����뷶Χ��0.625*Vref
//  Set_CH_Range_Select(0X0c,0x02);//����ͨ��7�����뷶Χ��0.625*Vref

//  AUTO_RST_Mode();//�����Զ�ɨ��ģʽ
//  while(1)
//  {
//    Get_AUTO_RST_Mode_Data(value,8);//�Զ�ɨ��ģʽ���Զ�ɨ�貢ת��8ͨ����ת�����ݴ���Value������
//  }
//}


/*
int main(void)//��ȡ��ͨ������ʾ��
{
  ADS8688_Init();
  
  ADS8688_Write_Program_Register(0x02,0x00);//����ͨ���˳��͹���״̬
  ADS8688_Write_Program_Register(0x01,0xff);//ʹ������ͨ��
  Set_CH_Range_Select(0X05,0x00);//����ͨ��0�����뷶Χ��2.5*Vref
  MAN_Ch_n_Mode(MAN_Ch_0);
  while(1)
  {
    value[0]=Get_MAN_Ch_n_Mode_Data();//��ȡ0ͨ�����ݣ���ӦӲ��ͨ����CH1
  }
}
*/


/*
int main(void)//��ȡ����ͨ�����ݣ���AUXͨ������ͨ��Ϊֱ��������ADC����PGA�����뷶ΧΪ0-4.096V��
{
  ADS8688_Init();
  
  MAN_Ch_n_Mode(MAN_AUX);
  while(1)
  {
    value[0]=Get_MAN_Ch_n_Mode_Data();//��ȡ����ͨ������
  }
}

*/


u8 SPI1_ReadData(void)
{	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}


u8 SPI1_WriteData(u8 Data)
{
 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, Data);
}

void SPI1_IRQHandler(void)
{
	if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == SET)
	{
		SPI_ClearFlag(SPI1,SPI_FLAG_TXE);
	}
	if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET)
	{
		SPI_ClearFlag(SPI1,SPI_FLAG_RXNE);
	}
}	
