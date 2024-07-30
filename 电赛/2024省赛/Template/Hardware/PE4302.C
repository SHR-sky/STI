#include "PE4302.H"

/**************************************************************** 
函数名称: PE_GPIO_Init 
功    能: 管脚初始化 
参    数: 无
返回值  : 无 
*****************************************************************/ 
void PE_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	 //使能PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE
	GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);					 //PBE 输出高
}
/**************************************************************** 
函数名称: PE4302_0_Set 
功    能: 衰减值设置 
参    数: db，范围：0~63，对应衰减0~31.5dB
返回值  : 无 
*****************************************************************/ 
void PE4302_0_Set(unsigned char db)
{
  unsigned char i;
  unsigned char W_DB;
  db = db&0X3F;
	
  PE_CLK_0;
	delay_us(500);
  PE_LE_0_EN;
	delay_us(500);
  for(i = 0; i < 6;i++)
  {
    W_DB = (db>>5);
    if(W_DB == 1)
    {
      PE_DAT_1;
    }
    else
    {
      PE_DAT_0;
    }
		delay_us(500);
    PE_CLK_1;
		delay_us(500);
    db = (db << 1)&0X3F;
    PE_CLK_0;
		delay_us(500);
  }
  PE_LE_0_DIS;
	delay_us(500);
	PE_DAT_0;
}

