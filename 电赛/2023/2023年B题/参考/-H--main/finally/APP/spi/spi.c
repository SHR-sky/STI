#include "spi.h"

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����Ϊ SPI1  sck
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����Ϊ SPI1  miso
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5����Ϊ SPI1  mosi
	
	//GPIOFB3,4,5��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5���ù������	 //����345
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��  B1�������
	
	ResetPin_Init();  //FPGA_RESET ��ʼ��

	
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
	SPI1_ReadWriteByte(0xff);//��������	
}

void SPI2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI1ʱ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB3����Ϊ SPI1  sck
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //PB4����Ϊ SPI1  miso
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB5����Ϊ SPI1  mosi
	
	//GPIOFB3,4,5��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PB3~5���ù������	 //����345
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��  B1�������
	
	ResetPin_Init();  //FPGA_RESET ��ʼ��

	
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//��λSPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	
//	SPI2_ReadWriteByte(0xff);//��������	
}

//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//�ȴ���������  
	
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����
		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
 		    
}

void ResetPin_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;           //B0��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB3~5���ù������    //����FPGA��λ	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB3~5���ù������	//�յ�FPGA����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PB3~5���ù������	//FPGA��ʹ���ж�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

uint8_t SPI1_RW1B(uint8_t txdata)
{
	u8 rxdata;
	CS_0;
	rxdata=SPI1_ReadWriteByte(txdata);
	CS_1;
	return rxdata;
}

uint16_t SPI1_RW2B(uint8_t txdata1,uint8_t txdata2)
{
	u16 rxdata;
	CS_0;
	rxdata=SPI1_ReadWriteByte(txdata1);
	CS_1;
	CS_0;
	rxdata=((rxdata<<8)|SPI1_ReadWriteByte(txdata2));
	CS_1;
	return rxdata;
}

uint8_t Wait_Fpga(void)
{
	SPI1_RW1B(8);
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)!=RESET )
	{
		FPGA_RESET_0;delay_ms(5);
		FPGA_RESET_1;delay_ms(10);
		return 0;
	}
	return 1;
}
uint16_t SPI_FPGA1[128],SPI_FPGA2[128];
u8 RESET_FLAG=0;
void SPI_Read_Fpga(uint8_t txdata,uint16_t *buf,u16 size)
{
	if(RESET_FLAG==0) {while(!Wait_Fpga());RESET_FLAG=1;}
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))
	{
		SPI1_RW2B(0x88,0x99);
	}
	delay_ms(2);
//	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))
//	{
//		SPI1_RW2B(0x88,0x99);
//	}
//	delay_ms(2);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_SET);
//	delay_ms(1);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET);
//	SPI1_RW1B(0x36);
//	delay_ms(1);
//	SPI1_RW1B(0x63);
	for(u16 i=0;i<size;i++)
	{
//		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7));
		if(i==0)
		{
			*buf=SPI1_RW2B(0x88,0x99);
			*buf=SPI1_RW2B(0x88,0x99);
			*buf=SPI1_RW2B(0x88,0x99);
		}
		if(i<128)
		{
			*buf=SPI1_RW2B(0x88,0x99);buf++;
//			buf[i]=SPI1_RW2B(0x88,0x99);
		}
		else {SPI1_RW2B(0x88,0x99);}
	}
}

void Deal_Data(uint16_t *spi_data,float *data,u16 size)
{
	for(u16 i=0;i<size;i++)
	{
		(*data)=(*spi_data-0x07ff)*5.0/4096;
		data++;
		spi_data++;
	}
}

void Add_SPI(uint16_t *data1,uint16_t *data2,uint16_t size)
{
	if(RESET_FLAG==0) {while(!Wait_Fpga());RESET_FLAG=1;}
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))
	{
		SPI1_RW2B(0x88,0x99);
	}
	delay_ms(1);
	
	SPI1_RW2B(0x88,0x99);
	SPI1_RW2B(0x88,0x99);
	SPI1_RW2B(0x88,0x99);
	SPI1_RW2B(0x88,0x99);
	SPI1_RW2B(0x88,0x99);
	SPI1_RW2B(0x88,0x99);
	for(u16 i=0;i<size;i++)
	{
		if(i%2==0) {*data1=SPI1_RW2B(0x88,0x99);*data1++;}
		else {*data2=SPI1_RW2B(0x88,0x99);*data2++;}
	}
}
