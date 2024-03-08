#ifndef __tft_196_spi_240x240_H
#define __tft_196_spi_240x240_H
#include <stdint.h>

#define DISPLAY_OVERTURN 0			//�Ƿ�ת��ʾ 1��������ת 0��������
#define DISPLAY_BOTTOM_TO_TOP 0		//��Ļ��ʾ���� 1������������ʾ 0������������ʾ
#define DISPLAY_RIGHT_TO_LEFT 0		//��Ļ��ʾ���� 1������������ʾ 0������������ʾ

#if DISPLAY_OVERTURN
#define TFT_SCREEN_WIDTH 	240
#define TFT_SCREEN_HEIGHT 	240
#define TFT_Y_OFFSET 		0	//Y�ᣨ�У���ƫ�Ƶ�ַ
#define TFT_X_OFFSET 		20	//X�ᣨ�У���ƫ�Ƶ�ַ
#else
#define TFT_SCREEN_WIDTH 	240
#define TFT_SCREEN_HEIGHT 	240
#define TFT_Y_OFFSET 		20	//Y�ᣨ�У���ƫ�Ƶ�ַ
#define TFT_X_OFFSET 		0	//X�ᣨ�У���ƫ�Ƶ�ַ
#endif

//��ɫ��������
#define RED   	0XF800
#define GREEN   0X07E0
#define BLUE  	0X001F
#define WHITE	0xFFFF	

#define TFT_BACKGROUND_COLOR WHITE	//������ɫ
/**
���Ŷ���
	CLK:PA4
	SDA:PA5
	RST:PA6
	D/C:PA7
	BLK:PC14
	CS:PA8	
*/

#define TFT_BLK PCout(13) //��������
#define TFT_CS PCout(14)  //Ƭѡ����
//-----------------TFT�˿ڶ���----------------  					   
#define TFT_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)  //CLK
#define TFT_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define TFT_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)  //SDA
#define TFT_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define TFT_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)   //RST
#define TFT_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define TFT_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)    //D/C
#define TFT_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define TFT_BLK_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_14)  //����
#define TFT_BLK_Set() GPIO_SetBits(GPIOC,GPIO_Pin_14)

#define TFT_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)   //CS    Ƭѡ��
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
