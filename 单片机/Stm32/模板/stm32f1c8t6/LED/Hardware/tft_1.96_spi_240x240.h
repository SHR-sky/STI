#ifndef __tft_196_spi_240x240_H
#define __tft_196_spi_240x240_H
#include <stdint.h>

#define DISPLAY_OVERTURN 0			//是否翻转显示 1：开启翻转 0：不开启
#define DISPLAY_BOTTOM_TO_TOP 0		//屏幕显示方向 1：从下往上显示 0：从上往下显示
#define DISPLAY_RIGHT_TO_LEFT 0		//屏幕显示方向 1：从右往左显示 0：从左往右显示

#if DISPLAY_OVERTURN
#define TFT_SCREEN_WIDTH 	240
#define TFT_SCREEN_HEIGHT 	240
#define TFT_Y_OFFSET 		0	//Y轴（列）的偏移地址
#define TFT_X_OFFSET 		20	//X轴（行）的偏移地址
#else
#define TFT_SCREEN_WIDTH 	240
#define TFT_SCREEN_HEIGHT 	240
#define TFT_Y_OFFSET 		20	//Y轴（列）的偏移地址
#define TFT_X_OFFSET 		0	//X轴（行）的偏移地址
#endif

//颜色常量定义
#define RED   	0XF800
#define GREEN   0X07E0
#define BLUE  	0X001F
#define WHITE	0xFFFF	

#define TFT_BACKGROUND_COLOR WHITE	//背景颜色
/**
引脚定义
	CLK:PA4
	SDA:PA5
	RST:PA6
	D/C:PA7
	BLK:PC14
	CS:PA8	
*/

#define TFT_BLK PCout(13) //背光引脚
#define TFT_CS PCout(14)  //片选引脚
//-----------------TFT端口定义----------------  					   
#define TFT_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)  //CLK
#define TFT_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define TFT_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)  //SDA
#define TFT_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define TFT_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)   //RST
#define TFT_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define TFT_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)    //D/C
#define TFT_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define TFT_BLK_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_14)  //背光
#define TFT_BLK_Set() GPIO_SetBits(GPIOC,GPIO_Pin_14)

#define TFT_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)   //CS    片选线
#define TFT_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_8)


void tft_196_240x240_init(void);
void open_backlight(void);
void close_backlight(void);
void TFT_full_color(unsigned int color);
void TFT_display_image(const uint16_t *address, uint16_t startX, uint16_t start_y);
void TFT_display_char16_16(const uint8_t *address ,uint16_t startX,uint16_t startY,
							uint16_t textColor, uint16_t backgroundColor);
void TFT_display_char16_16_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color);
void TFT_display_point(uint16_t startX, uint16_t startY, uint16_t color);
void TFT_display_en(const uint8_t *address ,const uint8_t char_en ,uint16_t startX,uint16_t startY,uint16_t textColor, 
					uint16_t backgroundColor);
void TFT_display_en_string(const uint8_t *address ,const uint8_t *str ,uint16_t startX,uint16_t startY,uint16_t textColor, 
					uint16_t backgroundColor);
#endif
