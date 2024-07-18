/**********************************************************
                       康威电子
本程序仅供学习使用，未经作者许可，不得用于其它任何用途										 
功能：ADS8688，16位AD 
			显示：12864cog
接口：接口请参照ads8688.h
时间：2023/06/07
版本：1.1
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com

**********************************************************/
#include "ADS8688.h"

/**********************************************************************************************************
*	函 数 名: ADS8688_IO_Init
*	功能说明: ADS8688 IO口初始化
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void ADS8688_IO_Init(void)	   //ADS8688 IO口初始化
{
	/*
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	*/

	
	
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
/************打开时钟*************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);  
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE ); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	
/************引脚配置*************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //SPI_NSS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		//SPI_SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;		//SPI_MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //浮空输入 GPIO_Mode_IN
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // GPIO_PuPd_NOPULL
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;        //SPI_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); // 复用为SPI1

/************SPI初始化配置*************/
 // 复用为SPI1	SPI_Cmd(SPI1, DISABLE);	 //必须先禁用,才能改变MODE
	SPI_InitStructure.SPI_Direction =SPI_Direction_2Lines_FullDuplex; //两线全双工
	SPI_InitStructure.SPI_Mode =SPI_Mode_Master; //主
	SPI_InitStructure.SPI_DataSize =SPI_DataSize_8b; //8位
	SPI_InitStructure.SPI_CPOL =SPI_CPOL_Low; //CPOL=0
	SPI_InitStructure.SPI_CPHA =SPI_CPHA_1Edge; //CPHA=0
	SPI_InitStructure.SPI_NSS =SPI_NSS_Hard; //硬件NSS 不需要控制CS
	// 需要人为开启SPI_Cmd(SPI1, ENABLE)与关闭
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_4; //4分频
	SPI_InitStructure.SPI_FirstBit =SPI_FirstBit_MSB; //高位在前
	SPI_InitStructure.SPI_CRCPolynomial =7; //CRC7
	SPI_Init(SPI1,&SPI_InitStructure);
	
// 	SPI_Cmd(SPI1, ENABLE);			//先不打开SPI
 	SPI_SSOutputCmd(SPI1, ENABLE);   		//SPI的NSS引脚控制开启
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
*	函 数 名: ADS8688_Init
*	功能说明: ADS8688初始化
*	形    参: ch_en：要使能的通道
*	返 回 值: 无
**********************************************************************************************************/
void ADS8688_Init(uint8_t ch_en)	   //ADS8688初始化
{
	ADS8688_IO_Init();
	SOFT_RESET(); //复位
	delay_ms(25);
	Set_Auto_Scan_Sequence(ch_en);//使能通道
	ADS8688_WriteReg(CH_PD,~ch_en);//通道退出低功耗状态 通道上电
	delay_ms(50);
	while(ADS8688_ReadReg(AUTO_SEQ_EN)!=ch_en)//是否使能成功
	{
		Set_Auto_Scan_Sequence(ch_en);//使能通道
		ADS8688_WriteReg(CH_PD,~ch_en);//通道退出低功耗状态 通道上电
		delay_ms(50);
	}		
}

/**********************************************************************************************************
*	函 数 名: ADS8688_SPI_Read8Bit
*	功能说明: 从SPI总线读8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: ADS8688_SPI_Write8Bit
*	功能说明: 向SPI总线写8个bit数据。 不带CS控制。
*	形    参: data : 数据
*	返 回 值: 无
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
*	函 数 名: ADS8688_WriteCmd
*	功能说明: 写命令寄存器
*	形    参:  cmd : 命令
*	返 回 值: 无
**********************************************************************************************************/
void ADS8688_WriteCmd(uint16_t cmd)//写ADS8688命令寄存器
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
*	函 数 名: SOFT_RESET
*	功能说明: 软件复位
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SOFT_RESET(void)//软件复位
{
  ADS8688_WriteCmd(RST);
}

/**********************************************************************************************************
*	函 数 名: AUTO_RST_Mode
*	功能说明: 自动模式
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void AUTO_RST_Mode(void)//自动扫描模式
{
  ADS8688_WriteCmd(AUTO_RST);
}
/**********************************************************************************************************
*	函 数 名: MAN_CH_Mode
*	功能说明: 手动单次扫描模式通道选择
*	形    参:  ch : 要选择的通道
*	返 回 值: 无
**********************************************************************************************************/
void MAN_CH_Mode(uint16_t ch)//手动单次扫描模式
{
  ADS8688_WriteCmd(ch);	
}

/**********************************************************************************************************
*	函 数 名: ADS8688_ReadReg
*	功能说明: 读指定的寄存器
*	形    参:  addr : 寄存器地址
*	返 回 值: data : 读到的寄存器值。
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
*	函 数 名: ADS8688_WriteReg
*	功能说明: 写指定的寄存器
*	形    参:  addr : 寄存器地址
*			  		data : 寄存器值
*	返 回 值: 无
**********************************************************************************************************/
void ADS8688_WriteReg(uint8_t addr,uint8_t data)
{
	SPI_Cmd(SPI1,ENABLE);
	SPI1_WriteData(addr<<1| 0X01);
	SPI1_WriteData(data);
	SPI_Cmd(SPI1,DISABLE);
}
/**********************************************************************************************************
*	函 数 名: Set_Auto_Scan_Sequence
*	功能说明: 设置自动扫描序列通道
*	形    参:  seq：需要设置自动扫描序列使能寄存器的值
*	返 回 值: 无
**********************************************************************************************************/
void Set_Auto_Scan_Sequence(uint8_t seq)	
{
	ADS8688_WriteReg(AUTO_SEQ_EN, seq);
}
/**********************************************************************************************************
*	函 数 名: Set_CH_Range
*	功能说明: 设置指定通道测量范围
*	形    参:  ch : 通道
*			  		range : 测量范围
*	返 回 值: 无
**********************************************************************************************************/
void Set_CH_Range(uint8_t ch,uint8_t range)
{
  ADS8688_WriteReg(ch,range);
}

/**********************************************************************************************************
*	函 数 名: Get_AUTO_RST_Mode_Data
*	功能说明: 读取自动扫描模式AD值
*	形    参:data : 数据指针
*			  	chnum : 通道个数
*	返 回 值: 无
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
*	函 数 名: Get_MAN_CH_Mode_Data
*	功能说明: 读取单次扫描模式AD值
*	形    参: 无
*	返 回 值: 无
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

//int main(void)//8通道自动扫描转换示例
//{
////  ADS8688_Init();
//  
//  ADS8688_Write_Program_Register(0x02,0x00);//所有通道退出低功耗状态
//  ADS8688_Write_Program_Register(0x01,0xff);//使能所有通道
//  
//  Set_CH_Range_Select(0X05,0x02);//设置通道0的输入范围：0.625*Vref
//  Set_CH_Range_Select(0X06,0x02);//设置通道1的输入范围：0.625*Vref
//  Set_CH_Range_Select(0X07,0x02);//设置通道2的输入范围：0.625*Vref
//  Set_CH_Range_Select(0X08,0x02);//设置通道3的输入范围：0.625*Vref
//  Set_CH_Range_Select(0X09,0x02);//设置通道4的输入范围：0.625*Vref
//  Set_CH_Range_Select(0X0a,0x02);//设置通道5的输入范围：0.625*Vref
//  Set_CH_Range_Select(0X0b,0x02);//设置通道6的输入范围：0.625*Vref
//  Set_CH_Range_Select(0X0c,0x02);//设置通道7的输入范围：0.625*Vref

//  AUTO_RST_Mode();//进入自动扫描模式
//  while(1)
//  {
//    Get_AUTO_RST_Mode_Data(value,8);//自动扫描模式，自动扫描并转换8通道。转换数据存与Value数组中
//  }
//}


/*
int main(void)//读取单通道数据示例
{
  ADS8688_Init();
  
  ADS8688_Write_Program_Register(0x02,0x00);//所有通道退出低功耗状态
  ADS8688_Write_Program_Register(0x01,0xff);//使能所有通道
  Set_CH_Range_Select(0X05,0x00);//设置通道0的输入范围：2.5*Vref
  MAN_Ch_n_Mode(MAN_Ch_0);
  while(1)
  {
    value[0]=Get_MAN_Ch_n_Mode_Data();//读取0通道数据，对应硬件通道的CH1
  }
}
*/


/*
int main(void)//读取辅助通道数据，即AUX通道，此通道为直接输入至ADC，无PGA，输入范围为0-4.096V。
{
  ADS8688_Init();
  
  MAN_Ch_n_Mode(MAN_AUX);
  while(1)
  {
    value[0]=Get_MAN_Ch_n_Mode_Data();//读取辅助通道数据
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
