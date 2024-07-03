/**********************************************************
                       康威电子
										 
功能：ADF4351正弦波点频输出，范围35M-4400M，步进0.1M，stm32f103rct6控制
			显示：12864
接口：CLK-PC11 DATA-PC10 LE-PC9 CE-PC12 板载接口橙色排针 按键接口请参照key.h
时间：2015/11/3
版本：1.0
作者：康威电子
其他：模块板载100M参考，用户若需要更改参考频率，程序调整请参考ADF4351.C中		->244行

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/

#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "ADF4351.h"
#include "task_manage.h"
#include "timer.h"
char str[30];	//显示缓存
extern u8 _return;
int main(void)
{
	u16 i=0;

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(72);	//初始化延时函数
	LED_Init();	//初始化LED接口
	key_init();//按键初始化
	initial_lcd();//液晶初始化
	
	LCD_Clear();
	delay_ms(300);
	LCD_Refresh_Gram();
	Timerx_Init(99,71);
	LCD_Clear();
	LCD_Show_CEStr(0,0,"5");//液晶测试
	
	//4351初始化
	ADF4351Init();
	ADF4351WriteFreq(400);									//使输出400M频率
	while(1);
																//取消此注释，获得固定输出，400M
	while(1)
	{
		KeyRead();//读取按键值
		Set_PointFre(Keycode, 0);//界面交互处理
		if(_return){_return=0;LCD_Refresh_Gram();}//更新显示
		KEY_EXIT();	//注销键值
	}
}

