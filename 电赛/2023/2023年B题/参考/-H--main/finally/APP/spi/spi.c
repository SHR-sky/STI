#include "spi.h"

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1  sck
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1  miso
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1  mosi
	
	//GPIOFB3,4,5初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	 //本来345
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  B1推挽输出
	
	ResetPin_Init();  //FPGA_RESET 初始化

	
	//这里只针对SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	SPI1_ReadWriteByte(0xff);//启动传输	
}

void SPI2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能SPI1时钟
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB3复用为 SPI1  sck
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //PB4复用为 SPI1  miso
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB5复用为 SPI1  mosi
	
	//GPIOFB3,4,5初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PB3~5复用功能输出	 //本来345
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  B1推挽输出
	
	ResetPin_Init();  //FPGA_RESET 初始化

	
	//这里只针对SPI口初始化
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//复位SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
//	SPI2_ReadWriteByte(0xff);//启动传输	
}

//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//等待发送区空  
	
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
 		    
}

void ResetPin_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;           //B0初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB3~5复用功能输出    //出错FPGA复位	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB3~5复用功能输出	//收到FPGA拉高
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PB3~5复用功能输出	//FPGA读使能判断
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
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
