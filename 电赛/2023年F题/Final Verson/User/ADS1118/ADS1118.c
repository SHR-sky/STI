#include "ADS1118.h"

#include "usart.h"

#include "delay.h"

/*
32-Bit模式下CS引脚可以一直保持为低，节省一个IO口。
32-Bit模式可以细分为两种，一种是把设置寄存器（16bit）写入两次，一种是写入一次后第二次（后16bit）写0。

16-Bit模式要求在每两次通信之间CS（片选）引脚要拉高一次。
每次通信可写入16bit的配置寄存器值和读取到16bit的转换寄存器值。

暂时只研究16bit
*/
#define INPUT  	PAin(4)				// 数据输入 , 连接芯片的OUT		
#define OUTPUT	PAout(5)			// 数据输出 , 连接芯片的DIN
#define CS  	PAout(6)			// 片选信号
#define SCK  	PAout(7)			// 时钟信号

#define CS1  	PAout(1)			// 片选1信号

u16 Conversion ;			// 存储从芯片读取的数据
float Voltage ;    			// 存储需要显示的测量电压值
float BaseV ;				// 采集电压的压基
u8 firstflag ;				// 第一次进入标志
u8 collect ;				// 每次采集的数据位置
float DP[8] ;				// 显示处理后的八通道数据

ConfigDef Config ;


void ADS1118GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out ;		// PA6作为片选输出信号，设置为推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	GPIO_SetBits(GPIOA,GPIO_Pin_6);							// 片选初始化为高
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out ;		// PA5作为数据输出信号，初始设置为推挽输出		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);						// 数据输出口初始化为低
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out ;		// PA7作为时钟信号，设置为推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);						// 时钟初始化为低
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out ;		// PA1作为片选1输出信号，设置为推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	GPIO_SetBits(GPIOA,GPIO_Pin_1);							// 片选初始化为高
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;			// PA4作为数据输入信号，设置为下拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_Init(GPIOA , &GPIO_InitStructure) ;
	
}
											
// 当新数据准备好检索时，此引脚变为低电平
// 在连续转换模式下，如果未从器件中检索到任何数据，则DOUT / DRDY在下一个数据就绪信号（DOUT / DRDY为低电平）之前的8 μs再次变为高电平
/**********************************************************
函数名称：RWByte
函数功能：SPI发送接收数据
函数参数：需要配置的寄存器数据
函数返回：采集到的16位数据
函数隶属：Display
创建日期：2020/04/17  14：23
作    者：
注    解：
**********************************************************/
u16 RWByte(u16 DATA)
{
	u8 t ;
	u16 returndata ;
	delay_ms(1);
	for(t=0;t<16;t++)										
	{
		if(DATA&0x8000)				// 每次向从机写八位数据,芯片在时钟下降沿锁存DIN数据
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
		if(INPUT == 1)					// 每次读取从机中的八位数据，芯片在时钟上升沿将数据移出
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
函数名称：ADS1118Init
函数功能：初始化时配置一些不常改变的寄存器值
函数参数：单次转换，工作模式，传输速率，上拉使能，更新数据
函数返回：无
函数隶属：main
创建日期：2020/04/17  14：23
作    者：
注    解：
**********************************************************/
void ADS1118Init(u8 ss,u8 mode ,u8 dr,u8 pue,u8 nop)
{
	Config.ConfigDef_T.SS = ss ;				// 设置为无效果
	Config.ConfigDef_T.MODE = mode ;			// 设置为连续转换模式
	Config.ConfigDef_T.DR = dr ;				// 设置转换速率为128 SPS
	Config.ConfigDef_T.PULL_UP_EN = pue ;		// 设置DOUT上拉使能
	Config.ConfigDef_T.NOP = nop ;				// 设置有效数据，更新配置寄存器
	Config.ConfigDef_T.CNV_RDY_FL = 0x01 ;		// 保留位，始终写1	
	
	Conversion = 0  ;			
	Voltage = 0  ;    			
	BaseV = 0  ;
	firstflag = 0 ;
}

/**********************************************************
函数名称：Getdata
函数功能：配置寄存器值并连续采集五次数据求平均值
函数参数：通道选择，工作模式，传输速率，上拉使能，更新数据
函数返回：无
函数隶属：main
创建日期：2020/04/17  14：23
作    者：
注    解：
**********************************************************/
void Getdata(u8 mux,u8 pga,u8 tsmode,u8 choose)
{
	float FV[10] ;			// 存储连续的五次转换数据
	u8 t ;
	float displaydata ;
	
	Config.ConfigDef_T.MUX = mux ;				// 设置为AIN0和GND
	Config.ConfigDef_T.PGA = pga ;				// 设置FSR=±4.096V
	Config.ConfigDef_T.TS_MODE = tsmode ;		// 设置温度传感器模式为ADC模式	
								
	switch (pga)
	{
		case 0 :
			BaseV = 187.5 ;						// 压基单位为uV
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
		if((INPUT == 0)||(firstflag == 0))									// CS需要周期性拉低来检测是否有新的数据产生(检测INPUT引脚是否有低电平)							
		{
			Conversion = RWByte(Config.Bytes);
			Voltage = (BaseV*Conversion)/1000000 ;			// 转换单位：uV→V	
			Conversion = 0 ;							// 数据显示之后清零	
			firstflag = 1 ;
		}

		CS = 1 ;
		CS1 = 1 ;
		FV[t] = Voltage ;
		delay_ms(15);									// 延迟时间不能低于15ms
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
	printf("采集到的数据为 =  ");
	for(x=0;x<8;x++)
	{
		printf("%3.3f ",DP[x]);
	}
	printf("\r\n");
}