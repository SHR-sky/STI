/**********************************************************
                       康威电子
本程序仅供学习使用，未经作者许可，不得用于其它任何用途										 
功能：ADS8688，16位AD 8通道单次采集 使用芯片内部基准4.096V 
显示：12864cog
接口：接口请参照ads8688.h 可直接连接本店主控板TLV5608接口
时间：2023/06/07
版本：1.1
作者：康威电子
其他：本程序只做模块功能验证，以便快速测试模块好坏。
			如需进行性能验证，请自行编程.更换高速MCU或FPGA实现高速采集！！！！

			当选择量程为双极性时，0V对应32767，因此V=(ADC值-32767)*量程/65535；当量程为单极性时，则无需减去32767。

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com
**********************************************************/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "usb_lib.h"
#include "math.h"
#include "hw_config.h"
#include "string.h"
#include "stdlib.h"
#include "lcd.h"
#include "timer.h"
#include "ads8688.h"

/**********************************************************
* 函数功能 ---> USB连接/断开
* 入口参数 ---> usb_sw：USB开启/关闭控制。0：关闭
*                                         1：打开
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/	   
void USB_Port_Set(u8 usb_sw)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //使能PORTA时钟
		   	 
	if(usb_sw)	_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // 断电模式
		GPIOA->CRH &= 0XFFF00FFF;
		GPIOA->CRH |= 0X00033000;
		PAout(12) = 0;	    		  
	}
}
 
void display(void)
{	
	LCD_Show_CEStr(0,0,"I1");
	LCD_Show_CEStr(64,0,"I2");
	LCD_Show_CEStr(0,2,"I3");
	LCD_Show_CEStr(64,2,"I4");
	LCD_Show_CEStr(0,4,"I5");
	LCD_Show_CEStr(64,4,"I6");
	LCD_Show_CEStr(0,6,"I7");
	LCD_Show_CEStr(64,6,"I8");
	LCD_Refresh_Gram();
}	

int main(void)
{
	uint8_t i=0;
	char showLcd[30];
	char infoBackPC[64];
	
	SystemInit();
	delay_init(72);		//延时初始化
	uart_init(115200);	//串口初始化
	key_init();//按键初始化
	initial_lcd();//液晶初始化
	LCD_Clear();
	LCD_Show_CEStr(10,0,"ADS8688-16bit");//ADS8688 16位adc
	LCD_Show_CEStr(40,2,"Sgl");//单次
	LCD_Show_CEStr(40,4,"Unit:1mV");//单位
	LCD_Refresh_Gram();
	delay_ms(1000);
	USB_Port_Set(0); 	//USB先断开
	delay_ms(600);
	USB_Port_Set(1);	//USB再次连接
	USB_Config();
	LCD_Clear();
	
	ADS8688_Init(CH0_EN|CH1_EN|CH2_EN|CH3_EN|CH4_EN|CH5_EN|CH6_EN|CH7_EN);//ADS8688初始化
  Set_CH_Range(CHIR_0,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_1,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_2,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_3,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_4,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_5,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_6,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_7,ADS8688_IR_N2_5V);//设置通道输入范围：±10.24V ±2.5*ref  ref=4.096V
	//通道每个量程配置可相同也可不同。
	display();
		
	TIM4_Int_Init(71,9999);//定时器，10ms中断，在定时器中断里一次读取8个通道ADC及AUX通道数据,即采样率为100SPS
	
	//读得越快采样率越高。这里建议定时器中断＞=5ms，如需更快读取速度请自行优化SPI通讯时序（例如采用硬件SPI，或更换更高速的单片机或FPGA）
	
	//ADS8688芯片标称的最大总吞吐量为500kSPS(单通道时)。如果仅选择两个通道则每个通道最大为250kSPS,以此类推。

  while(1)
  {
		if(get_ok==1)//ADC数据读取完成
		{
			get_ok=0;
			for(i = 0; i < 4; i++) //显示
			{
				sprintf(showLcd, "%06d", (int16_t)((value[i*2]-32767)*CONST_N2_5V_LSB_mV));//测量范围为±需要减去32767
				LCD_ShowString(0,16 , i*16, (const u8*)showLcd);
				
				sprintf(showLcd, "%06d", (int16_t)((value[i*2+1]-32767)*CONST_N2_5V_LSB_mV));	
				LCD_ShowString(0,80 , i*16, (const u8*)showLcd);
			}
			LCD_Refresh_Gram();
			for(i = 0; i < 8; i++)//发送
			{
				sprintf(infoBackPC, "AIN%d=%0.2fmV,%X ",i+1,((value[i]-32767)*CONST_N2_5V_LSB_mV),value[i]);			
				USB_TxWrite((uint8_t*)infoBackPC, strlen(infoBackPC)); //usb发送
				printf("%s",infoBackPC);  //串口发送	
			}
			USB_TxWrite("\r\n", 2); //usb发送
			printf("\r\n");  //串口发送	
		}
  }				
			
}

 












 
 
 
 
 
 
 
