/**********************************************************
                       ��������
										 
���ܣ�ADF4351���Ҳ���Ƶ�������Χ35M-4400M������0.1M��stm32f103rct6����
			��ʾ��12864
�ӿڣ�CLK-PC11 DATA-PC10 LE-PC9 CE-PC12 ���ؽӿڳ�ɫ���� �����ӿ������key.h
ʱ�䣺2015/11/3
�汾��1.0
���ߣ���������
������ģ�����100M�ο����û�����Ҫ���Ĳο�Ƶ�ʣ����������ο�ADF4351.C��		->244��

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
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
char str[30];	//��ʾ����
extern u8 _return;
int main(void)
{
	u16 i=0;

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//�����жϷ���
	delay_init(72);	//��ʼ����ʱ����
	LED_Init();	//��ʼ��LED�ӿ�
	key_init();//������ʼ��
	initial_lcd();//Һ����ʼ��
	
	LCD_Clear();
	delay_ms(300);
	LCD_Refresh_Gram();
	Timerx_Init(99,71);
	LCD_Clear();
	LCD_Show_CEStr(0,0,"5");//Һ������
	
	//4351��ʼ��
	ADF4351Init();
	ADF4351WriteFreq(400);									//ʹ���400MƵ��
	while(1);
																//ȡ����ע�ͣ���ù̶������400M
	while(1)
	{
		KeyRead();//��ȡ����ֵ
		Set_PointFre(Keycode, 0);//���潻������
		if(_return){_return=0;LCD_Refresh_Gram();}//������ʾ
		KEY_EXIT();	//ע����ֵ
	}
}

