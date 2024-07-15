/**********************************************************
                       康威电子
功能：stm32f103rct6控制AD9954模块双向扫频
接口：控制引脚接口请参照AD9954.h
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
#include "AD9954.h"

int main(void)
{
	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(72);	//初始化延时函数
	delay_ms(1000);//延时一会儿，等待上电稳定,确保AD9954比控制板先上电。
	
	//代码移植建议
	//1.修改头文件AD9954.h中，自己控制板实际需要使用哪些控制引脚。如AD9954_CS脚改成PA0控制，则定义"#define AD9954_CS	PAout(0)" 
	//2.修改C文件AD9954.c中，AD9954_GPIO_Init函数，所有用到管脚的GPIO输出功能初始化
	//3.完成
	
	AD9954_Init();				//初始化控制AD9954需要用到的IO口,及寄存器
	
	AD9954_Set_LinearSweep(100000,500000,1,200,1,200,Dwell);//Dwell停留，输出扫频到终止频率后保持在终止频率。No_Dwell不停留，输出扫频到终止频率后回到起始频率；
							//起始100000Hz，终止500000Hz，上扫频步进1hz，上扫频时间约800ms；下扫频步进1hz，下扫频时间约800ms；扫频时间计算参考函数注解
	
	while(1)
	{
		PS0=1;//PS0控制扫频方向；高电平：从起始值扫到结束值
		delay_ms(800);
		
		PS0=0;//低电平：从结束值扫到起始值
		delay_ms(800);
	}
}





