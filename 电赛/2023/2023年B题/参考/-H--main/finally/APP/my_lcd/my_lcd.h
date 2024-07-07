#ifndef __MY_LCD
#define __MY_LCD
#include "system.h"
#include "SysTick.h"
#define SIZE 128
//#define PI 3.1415926

void LCD_ShowFloat(u16 x,u16 y,float num,u8 len,u8 size,u8 mode);  //小数点后len位，size为12、16、24， mode=0不叠加
void Draw_Coordinate(u16 start_x,  u16 start_y,  u16 end_x,  u16 end_y,  u8 mode); //坐标系 mode=1 包含负
void Draw_Line(u16 data_x[], u16 data_y[],u16 size);               //根据数组画线
void Creat_Wave(u8 vol,float fre,u8 pha, u16 size,float *data_x);
void Creat_TriWave(u8 vol,float fre,u8 pha, u16 size,float *data_x);
void Add_Wave(float *wave_a,float *wave_b,float *wave_c,float *wave,u16 size);
void LCD_DL_F(u16 x,u16 y,float *data_x,float *data_y,u16 size,u16 color);
void LCD_ShowFontHZ_16(u16 x, u16 y, u8 *cn);
	
#endif
