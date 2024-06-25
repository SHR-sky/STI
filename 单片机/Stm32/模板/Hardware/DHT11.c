#include "DHT11.h"

uint8_t dat[4]; // 接收数据

// 数据输入输出为PB8
void Data_Out_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_Structure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Structure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Structure);
}

void Data_In_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_Structure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Structure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_Structure);
}

/*
	首先主机发送开始信号，即：先拉高数据线
	再拉低数据线，保持 t1（至少 18ms）时间
	然后拉高数据线 t2（20~40us）时间，然后读取 DHT11 的响应

	正常的话，DHT11 会拉低数据线，保持 t3（40~50us）时间，作为响应信号
	然后 DHT11 拉高数据线，保持 t4（40~50us）时间后，开始输出数据。
*/

void tDHT11_start(void)
{
	Data_Out_Init();
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	delay_us(1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	delay_ms(20); // 拉低18ms以上  20ms
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	delay_us(30); // 拉高20ms-40ms
	Data_In_Init();
	delay_us(10);
}

uint8_t tDHT11_rec_byte(void) // 接收一个字节
{
	uint8_t i, dat = 0;
	for (i = 0; i < 8; i++) // 从高到低依次接收8位数据
	{
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0); // 等待50us低电平过去
		delay_us(40);									   // 延时40us，如果还为高则数据为1，否则为0
		dat <<= 1;										   // 移位使正确接收8位数据，数据为0时直接移位
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 1) // 数据为1时，使dat加1来接收数据1
		{
			dat += 1;
		}
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 1); // 等待数据线拉低
	}
	return dat;
}

void tDHT11_rec_data(uint8_t *temp, uint8_t *dec) // 接收40位的数据
{
	uint8_t R_H, R_L, T_H, T_L, revise;
	tDHT11_start();
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
	{

		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0); // 处理响应低电平，等待拉高

		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 1); // 处理响应高电平，等待拉低

		R_H = tDHT11_rec_byte();	// 接收湿度高八位
		R_L = tDHT11_rec_byte();	// 接收湿度低八位
		T_H = tDHT11_rec_byte();	// 接收温度高八位
		T_L = tDHT11_rec_byte();	// 接收温度低八位
		revise = tDHT11_rec_byte(); // 接收校正位

		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0); // 等待从机拉低

		if ((R_H + R_L + T_H + T_L) == revise) // 校正
		{
			dat[0] = T_H; // 温度高8位(表示整数部分)
			dat[1] = T_L; // 温度低8位(表示小数部分)
			dat[2] = R_H; // 湿度高8位(表示整数部分)
			dat[3] = R_L; // 湿度低8位(表示小数部分)

			*temp = dat[0]; // 温度
			*dec = dat[2];	// 湿度
		}
	}
}
