#include "my_lcd.h"
#include "tftlcd.h"
#include "arm_math.h"
//#include "font.h"
#include "my_font.h"



void LCD_ShowFloat(u16 x,u16 y,float num,u8 len,u8 size,u8 mode)  //小数点后len位，size为12、16、24， mode=0不叠加
{
	u8 ax;
	u16 dotx;
	if(num<0) {arm_abs_f32(&num,&num,1);LCD_ShowChar(x-(size/2),y,'-',size,mode);}
	ax=num;
	LCD_ShowNum(x,y,ax,1,size);
	LCD_ShowChar(x+(size/2),y,'.',size,mode);
	num-=ax;
	dotx=num*LCD_Pow(10,len);
	LCD_ShowxNum(x+(size/2)*2,y,dotx,len,size,mode);
}

void Draw_Coordinate(u16 start_x,  u16 start_y,  u16 end_x,  u16 end_y,  u8 mode)    //5像素每格  800*480
{
	u8 i,j;
	LCD_DrawLine_Color(start_x,start_y,end_x,start_y,BLACK);       ///x轴
	LCD_DrawLine_Color(start_x,start_y,start_x,end_y,BLACK);         ///y轴
	if(mode==1) {LCD_DrawLine_Color(start_x,start_y,start_x,2*start_y-end_y,BLACK);}
	LCD_DrawLine_Color(end_x,start_y,end_x-5,start_y-5,BLACK);
	LCD_DrawLine_Color(end_x,start_y,end_x-5,start_y+5,BLACK);     ///x箭头
	LCD_DrawLine_Color(start_x,end_y,start_x-5,end_y+5,BLACK);
	LCD_DrawLine_Color(start_x,end_y,start_x+5,end_y+5,BLACK);   ///y箭头
	LCD_ShowNum(start_x-10,start_y+10,0,1,16);
	for(i=0;i<(end_x-start_x)/5;i++) 
	{
		if(i%5==0)
		{
			LCD_DrawLine_Color(start_x+i*5,start_y,start_x+i*5,start_y-5,BLACK);
			if(mode!=1) LCD_ShowNum(start_x+i*5-10,start_y+10,i,2,12);
		}
		else
		{
			LCD_DrawLine_Color(start_x+i*5,start_y,start_x+i*5,start_y-2,BLACK);
		}
	}
	for(j=0;j<(start_y-end_y)/5;j++)
	{
		if(j%5==0)
		{
			LCD_DrawLine_Color(start_x,start_y-j*5,start_x+5,start_y-j*5,BLACK);
		}
		else
		{
			LCD_DrawLine_Color(start_x,start_y-j*5,start_x+2,start_y-j*5,BLACK);
		}
	}
	
	if(mode==1)
	{
		for(j=0;j<(start_y-end_y)/5;j++)
		{
			if(j%5==0)
			{
				LCD_DrawLine_Color(start_x,start_y+j*5,start_x+5,start_y+j*5,BLACK);
			}
			else
			{
				LCD_DrawLine_Color(start_x,start_y+j*5,start_x+2,start_y+j*5,BLACK);
			}
		}
	}
}

u16 DATA_X[SIZE]={0,5,5,10,10,15,15,20,20,25,25,30,30,35,35,40,40,45,45,50,50};
u16 DATA_Y[SIZE]={0,0,5,5,10,10,15,15,20,20,25,25,30,30,35,35,40,40,45,45,50};
void Draw_Line(u16 data_x[], u16 data_y[],u16 size)
{
	u16 i;
	for(i=1;i<size;i++)
	{
		LCD_DrawLine_Color(10+data_x[i-1],750-data_y[i-1],10+data_x[i],750-data_y[i],RED);
	}
}

float FDATA_X[SIZE];
float FDATA_Y[SIZE];
float BASE;
void Creat_Wave(u8 vol,float fre,u8 pha, u16 size,float *data_x)   //vol 幅度 fre 频率 pha相位 角度  
{
	float base=2*PI*fre/size;
	BASE=base;
	float start=pha*PI/180;
	for(u16 i=0;i<size;i++)
	{
		*data_x=vol*arm_sin_f32(start+base*i);
		data_x++;
	}
}


void Creat_TriWave(u8 vol,float fre,u8 pha, u16 size,float *data_x)  //相位0~90
{
	u8 flag=0;
	float base=1.0*vol*fre*4/size;
	float start=2.0*vol*pha/180;
	float value_now=start+base;
	float max=vol*1.00;
	for(u16 i=0;i<size;i++)
	{
		if(flag==0&&value_now<max)//(*data_x+base<max))
		{
			value_now+=base;
			*data_x=value_now;
		}
		else if(flag==0&&(value_now+base>=max))
		{
			value_now-=base;
			*data_x=value_now;
			flag=1;
		}
		else if(flag==1&&(value_now-base>-max))
		{
			value_now-=base;
			*data_x=value_now;
		}
		else if(flag==1&&(value_now-base<=-max))
		{
			value_now+=base;
			*data_x=value_now;
			flag=0;
		}
//		*data_x=vol*arm_sin_f32(start+base*i);
		data_x++;
	}
}

void Add_Wave(float *wave_a,float *wave_b,float *wave_c,float *wave,u16 size)
{
	for(u16 i=0;i<size;i++)
	{
		*wave=(*wave_a)+(*wave_b)+(*wave_c);
		*wave++;
		*wave_a++;
		*wave_b++;
		*wave_c++;
	}
}

void LCD_DL_F(u16 x,u16 y,float *data_x,float *data_y,u16 size,u16 color)
{
	u8 i;
	int16_t xs_point,ys_point,xe_point,ye_point;
	for(i=0;i<size-1;i++)
	{
		xs_point=(*data_x)*100;
		ys_point=(*data_y)*100;
		*data_x++;
		*data_y++;
		xe_point=(*data_x)*100;
		ye_point=(*data_y)*100;
//		LCD_DrawLine_Color(x+i*5,y-ys_point,x+i*5+5,y-ye_point,color);
		LCD_DrawLine_Color(x+i*2,y-ys_point,x+i*2+2,y-ye_point,color);
	}
	
}

#if 1
void LCD_ShowFontHZ_16(u16 x, u16 y, u8 *cn)
{
	u8 i, j, wordNum;
	u16 color;
	u16 x0=x; 
	u16 y0=y; 
	while (*cn != '\0')
	{
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar16x16[wordNum].Index[0]==*cn)
			     &&(CnChar16x16[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<32; i++)//116 
				{	//MSK的位数
					color=CnChar16x16[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_DrawFRONT_COLOR(x,y,FRONT_COLOR);
						} 						
						else
						{
							LCD_DrawFRONT_COLOR(x,y,BACK_COLOR);
						} 
						color<<=1;
						x++;
						if((x-x0)==16)//32
						{
							x=x0;
							y++;
							if((y-y0)==16)//29
							{
								y=y0;
							}
						}
					}//for(j=0;j<8;j++)结束
				}	
			}
			
		} //for (wordNum=0; wordNum<20; wordNum++)结束 	
		cn += 2;
		x += 16;//32
		x0=x;
	}
}	
#endif

