/**********************************************************
                       康威电子
										 
功能：stm32f103rct6控制，20MHz时钟，20倍频。 AD9954正弦波点频输出，LCD显示，
			按键调节频率，长按中间键切换扫频！！！！！
			显示：12864cog
接口：控制接口请参照AD9954.h  按键接口请参照key.h
时间：2023/06/xx
版本：4.3
作者：康威电子
其他：本程序只供学习使用，盗版必究。

					AD9959			单片机
硬件连接:	AD9954_CS 	PA3
					AD9954_SCLK PA4	
					AD9954_SDIO PA5	
					AD9954_OSK 	PC0
					PS1 				PA2
					PS0 				PB10
					IOUPDATE 		PA7
					AD9954_SDO	PA8
					AD9954_IOSY PB1
					AD9954_RES 	PA6
					AD9954_PWR 	PB0
					GND				--GND(0V)  
					
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/ 

**********************************************************/

#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "lcd.h"
#include "AD9954.h" 
#include "key.h"
#include "timer.h"
#include "task_manage.h"

char str[30];	//显示缓存
extern u8 _return;
int main(void)
{
	u16 i=0;

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(72);	//初始化延时函数
	LED_Init();	//初始化LED接口
	key_init();
	initial_lcd();
	LCD_Clear();
	delay_ms(100);
	LCD_Refresh_Gram();
	
	//定时器
	Timerx_Init(99,71);
	LCD_Clear();
	
	AD9954_Init();
	AD9954_Set_Fre(1000);//写频率1kHz
	AD9954_Set_Amp(16383);//0~16383对应峰峰值0mv~500mv(左右)
	
	//1、
	//后续代码涉及界面及按键交互功能，频率或幅度或其他参数可能被重写，可能导致此处更改参数无效
	//上述情况，取消下面注释即可，可直接更改频率及幅度，
//	AD9954_SETFRE(10000);	//写频率10kHz
//	Write_ASF(0X03FFF);//0X0000~0X3FFF对应峰峰值0mv~500mv(左右)
//	while(1);
	
	
	//2、	
//	关于扫频的说明
//	该程序的扫频功能利用定时器中断，不断写入自加的频率实现，
//	在timer.C的TIM4_IRQHandler中将自加后的频率写入
	while(1)
	{
		KeyRead();
		Set_PointFre(Keycode, 0);
		if(_return){_return=0;LCD_Refresh_Gram();}
		KEY_EXIT();
	}
}

