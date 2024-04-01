#include "tft_1.96_spi_240x240.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
/*************************************
*@brief open_backlight
*@details ����Ļ����
*@param[in] void
*@return void
*@author zx
*@date 2022-12-18
***************************************/
void open_backlight()
{
	TFT_BLK_Set();
}

/*************************************
*@brief close_backlight
*@details �ر���Ļ����
*@param[in] void
*@return void
*@author zx
*@date 2022-12-18
***************************************/
void close_backlight()
{
	TFT_BLK_Clr();
}

/*************************************
*@brief TFT_SEND_CMD
*@details д������
*@param[in] cmd 8λ����
*@return void
*@author zx
*@date 2022-12-18
***************************************/
void TFT_SEND_CMD(u8 cmd)
{	
	u8 i;			  
	TFT_DC_Clr();			
	TFT_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		TFT_SCLK_Clr();
		if(cmd&0x80)
		   TFT_SDIN_Set();
		else 
		   TFT_SDIN_Clr();
		TFT_SCLK_Set();
		cmd<<=1;   
	}				 		  
	TFT_CS_Set();
	TFT_DC_Set();   	  
} 
/*************************************
*@brief TFT_SEND_DATA
*@details д������
*@param[in] data 8λ����
*@return void
*@author zx
*@date 2022-12-18
***************************************/
void TFT_SEND_DATA(u8 data)
{	
	u8 i;			  
	TFT_DC_Set();			
	TFT_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		TFT_SCLK_Clr();
		if(data&0x80)
		   TFT_SDIN_Set();
		else 
		   TFT_SDIN_Clr();
		TFT_SCLK_Set();
		data<<=1;   
	}				 		  
	TFT_CS_Set();
//	TFT_DC_Set();   	  
} 


/*************************************
* @brief   LCD_SetWindows
* @details  ����LCD��ʾ����,������ɺ�Ϳ���������
			����ɫ�����ˣ�������һ��һ����������
* @param  startX���������x������
* 		   startY���������y������
* 		   width�����ڿ��
* 		   height�����ڸ߶�
* @return void  
*************************************/
void TFT_SetWindows(uint16_t startX,uint16_t startY,uint16_t width,uint16_t height)
{
#if TFT_X_OFFSET
	startX += TFT_X_OFFSET;
#endif	
#if TFT_Y_OFFSET
	startY += TFT_Y_OFFSET;
#endif		
	
	TFT_SEND_CMD(0x2A);		//��������X�����������0x2A
	//����SC[15:0]	->	������ʼ�е�ַ��Ҳ��������X����ʼ����
	TFT_SEND_DATA(startX>>8);				//��д��8λ
	TFT_SEND_DATA(startX&0xFF);			//��д��8λ
	//����EC[15:0]	->	���ô���X��������е�ַ����Ϊ����usXwidth�Ǵ��ڳ��ȣ�����ҪתΪ�е�ַ�ٷ���
	TFT_SEND_DATA((startX+width-1)>>8);				//��д��8λ
	TFT_SEND_DATA((startX+width-1)&0x00FF);			//��д��8λ

	TFT_SEND_CMD(0x2B);		//��������Y�����������0x2B
	//����SP[15:0]	->	������ʼ�е�ַ��Ҳ��������Y����ʼ����
	TFT_SEND_DATA(startY>>8);				//��д��8λ
	TFT_SEND_DATA(startY&0x00FF);			//��д��8λ
	//����EP[15:0]	->	���ô���Y��������е�ַ����Ϊ����usYheight�Ǵ��ڸ߶ȣ�����ҪתΪ�е�ַ�ٷ���
	TFT_SEND_DATA((startY+height-1)>>8);				//��д��8λ
	TFT_SEND_DATA((startY+height-1)&0x00FF);			//��д��8λ
	TFT_SEND_CMD(0x2C);			//��ʼ��GRAM��д����
}
/*************************************
*@brief TFT_display_point
*@details ���㺯��
*@param[in] startX��X��ʼ����
*			startY��Y��ʼ����
*			color�������ɫ
*@return void
*@author zx
*@date 2023-06-04
***************************************/
void TFT_display_point(uint16_t startX, uint16_t startY, uint16_t color)
{
	TFT_SetWindows(startX,startY, 1, 1);
	TFT_SEND_DATA(color>>8);
	TFT_SEND_DATA(color&0xff);
}

/*************************************
*@brief TFT_display_image
*@details ��ʾͼƬ����,16λ��ɫ�����ȷ���8λ�ٷ��Ͱ�λ
*@param[in] address:ͼƬ���ݵ�ַ
*			startX��X��ʼ����
*			startY��Y��ʼ����
*@return void
*@author zx
*@date 2023-06-03
***************************************/
void TFT_display_image(const uint16_t *address, uint16_t startX, uint16_t startY)
{
	uint16_t image_width;//ͼƬ��
	uint16_t image_hight;//ͼƬ��
	uint16_t x,y;
	image_width = address[0];
	image_hight = address[1];
	TFT_SetWindows(startX, startY, image_width, image_hight);
	for(y = 0; y < image_hight; y++)
	{
		for(x = 0; x < image_width; x++)
		{
			//����ͼƬ���ݣ�ÿ�η���16λ,�ȷ��߰�λ
			TFT_SEND_DATA(address[y*image_width + x + 2]>>8);
			TFT_SEND_DATA(address[y*image_width + x + 2]&0xff);
		}
	}	
}
/*************************************
*@brief TFT_display_en
*@details ��ʾһ��Ӣ����ĸ
*@param[in] address���ַ�����
			char_en:Ӣ����ĸ
*			startX��X��ʼ����
*			startY��Y��ʼ����
*			textColor��������ʾ��ɫ
*			backgroundColor:����ɫ
*@return void
*@author zx
*@date 2023-06-04
***************************************/
void TFT_display_en(const uint8_t *address ,const uint8_t char_en ,uint16_t startX,uint16_t startY,uint16_t textColor, 
					uint16_t backgroundColor)
{
	uint16_t column;
	uint8_t tm=0,temp;

	TFT_SetWindows(startX, startY, 8, 16);
	
	for(column = 0; column < 16; column++)  //column loop
	{
		temp =address[(char_en - 32) * 16 + column ];
		for(tm = 0; tm < 8; tm++)
		{
			if(temp&0x01)
			{
				TFT_SEND_DATA(textColor>>8);
				TFT_SEND_DATA(textColor);
			}
			else 
			{
				TFT_SEND_DATA(backgroundColor>>8);
				TFT_SEND_DATA(backgroundColor);
			}
			temp >>= 1;
		}
	}
}

/*************************************
*@brief TFT_display_en_string
*@details ��ʾӢ���ַ���
*@param[in] address���ַ�����
			str:Ӣ���ַ���
*			startX��X��ʼ����
*			startY��Y��ʼ����
*			textColor��������ʾ��ɫ
*			backgroundColor:����ɫ
*@return void
*@author RZY
*@date 2024-02-17
***************************************/
void TFT_display_en_string(const uint8_t *address ,const uint8_t *str ,uint16_t startX,uint16_t startY,uint16_t textColor, 
					uint16_t backgroundColor)
{
	uint16_t i;
	for(i = 0; i < strlen((const char*)str); i++)	//�˴�����ǿ������ת��������uint8_t*��char*���͵ĳ�ͻ
	{
		TFT_display_en(address ,str[i] , startX + i*8, startY, textColor, backgroundColor);
	}

}

/*************************************
*@brief TFT_display_char16_16
*@details ��ʾ16x16�ĺ���
*@param[in] address:ͼƬ���ݵ�ַ
*			startX��X��ʼ����
*			startY��Y��ʼ����
*			textColor��������ʾ��ɫ
*			backgroundColor:����ɫ
*@return void
*@author zx
*@date 2023-06-04
***************************************/
void TFT_display_char16_16(const uint8_t *address ,uint16_t startX,uint16_t startY,
							uint16_t textColor, uint16_t backgroundColor)
{
	unsigned int column;
	unsigned char tm=0,temp;

	TFT_SetWindows(startX, startY, 16, 16);
	
	for(column = 0; column < 32; column++)  //column loop
	{
		temp =* address;
		for(tm = 0; tm < 8; tm++)
		{
			if(temp&0x01)
			{
				TFT_SEND_DATA(textColor>>8);
				TFT_SEND_DATA(textColor);
			}
			else 
			{
				TFT_SEND_DATA(backgroundColor>>8);
				TFT_SEND_DATA(backgroundColor);
			}
			temp >>= 1;
		}
		address++;
	}
}  

/*************************************
*@brief TFT_display_char16_16_noBackColor
*@details ��ʾ16x16�ĺ���(����������ɫ���ο�)
*		  ��ʾ����һ��һ����ʾ��������ʾ��һ�е�ǰ��λ��
*		  Ȼ����ʾ���λ����ʾ���֮����ʾ�ڶ��У�
*		  ע�⣺����ȡģʱ�ǵ�λ��ǰ��λ�ں����򣩣������
*		  ��ȡģ������ȷ��
*@param[in] address:ͼƬ���ݵ�ַ
*			startX��X��ʼ����
*			startY��Y��ʼ����
*			color��������ʾ��ɫ
*@return void
*@author zx
*@date 2023-06-04
***************************************/
void TFT_display_char16_16_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color)
{
	uint16_t column;
	uint8_t tm=0,temp;
	uint16_t y = 0;
	for(column = 0; column < 16; column++)
	{
		temp =* address;
		for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				TFT_display_point(startX+ tm, startY+ y ,color);
			}
			
			temp >>= 1;
		}
		address++;
		temp =* address;
		for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				TFT_display_point(startX+ tm+8, startY+ y ,color);
			}
			
			temp >>= 1;
		}
//		if(column>0 && column%2 == 0)//�����������ĸ߶���ѹ����֮ǰ��һ��
		y++;
		address++;
	}	
}  

/*************************************
*@brief TFT_full_color
*@details ��������Ļ�������ɫ
*@param[in] void
*@return void
*@author zx
*@date 2022-12-18
***************************************/
void TFT_full_color(unsigned int color)
{
	unsigned int ROW,column;
	
	TFT_SetWindows(0,0,TFT_SCREEN_WIDTH,TFT_SCREEN_HEIGHT);
	for(ROW=0;ROW<TFT_SCREEN_HEIGHT;ROW++)             //ROW loop
	{ 
		for(column=0;column<TFT_SCREEN_WIDTH;column++) //column loop
		{

			TFT_SEND_DATA(color>>8);
			TFT_SEND_DATA(color);
		}
	}
}

/*************************************
*@brief tft_196_240x240_init
*@details tft��Ļ��ʼ����������Ҫ�Ƕ������ź���������
*@param[in] void
*@return void
*@author zx
*@date 2022-12-18
***************************************/
void tft_196_240x240_init()
{
	GPIO_InitTypeDef GPIO_InitStruture;                                  //����һ���ṹ�����
 	//Ӳ��SPI���ų�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	uint8_t lcd_data = 0x00;
	uint16_t start_x = 0;
	uint16_t end_x = TFT_SCREEN_WIDTH;
	uint16_t start_y = 0;
	uint16_t end_y = TFT_SCREEN_HEIGHT;
#if TFT_X_OFFSET
	start_x += TFT_X_OFFSET;
	end_x += TFT_X_OFFSET;
#endif	
#if TFT_Y_OFFSET
	start_y += TFT_Y_OFFSET;
	end_y += TFT_Y_OFFSET;
#endif	


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);     	//ʹ��A�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       	//�ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_4);
	
		//�������� PC13 ƬѡCS����PC14
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);                //ʹ��GPIOCʱ��
    //����GPIOB�Ĺ���ģʽ�ͳ�ʼ��
    GPIO_InitStruture.GPIO_Mode = GPIO_Mode_Out_PP;                    //�������
    GPIO_InitStruture.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;            //����PC13 PC14 
    GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;                   //�ٶ�50MHz
    GPIO_Init(GPIOC,&GPIO_InitStruture);
	
	//st7789����
	TFT_SCLK_Set();			//�ر�ע�⣡��
	TFT_RST_Clr();
	Delay_ms(1000);
	TFT_RST_Set();
	Delay_ms(1000);
    TFT_SEND_CMD(0x11); 			//Sleep Out
	Delay_ms(120);               //DELAY120ms 
	 	  //-----------------------ST7789V Frame rate setting-----------------//
//************************************************
	TFT_SEND_CMD(0x3A);        //��ɫ���ݸ�ʽRGB565 65k���ɫ
	TFT_SEND_DATA(0x05);
	TFT_SEND_CMD(0xC5); 		//VCOM1
	TFT_SEND_DATA(0x1A);
/*		
	(0,0)*********240***********->
	*
	*
	*
	240           240x240
	*
	*
	*
	��
**/
	TFT_SEND_CMD(0x36);                 // ��Ļ��ʾ��������

#if DISPLAY_BOTTOM_TO_TOP
	lcd_data |= (1<<7);
#else	
	lcd_data |= (0<<7);
#endif
#if DISPLAY_RIGHT_TO_LEFT
	lcd_data |= (1<<6);
#else	
	lcd_data |= (0<<6);
#endif

#if DISPLAY_OVERTURN//����ת��ʾ
	lcd_data |= (1<<5);//��ת��ʾ
#else
	lcd_data |= (0<<5);	
#endif
	TFT_SEND_DATA(lcd_data);

/*****��ʾλ�ã�ע�⣬x��y�Ǹ�����Ļ��ʾ���������ģ���һ��ͳһ**********/
	TFT_SEND_CMD(0x2B); //������ʾ���� x����ʼ���������� 
	TFT_SEND_DATA((start_x>>8)&0xff);
	TFT_SEND_DATA(start_x&0xff);
	TFT_SEND_DATA((end_x>>8)&0xff);
	TFT_SEND_DATA(end_x&0xff);

	TFT_SEND_CMD(0x2B); //������ʾ���� Y����ʼ����������
	TFT_SEND_DATA((start_y>>8)&0xff);
	TFT_SEND_DATA(start_y&0xff);
	TFT_SEND_DATA((end_y>>8)&0xff);
	TFT_SEND_DATA(end_y&0xff);
	
	//-------------ST7789V Frame rate setting-----------//
	TFT_SEND_CMD(0xb2);		//Porch Setting
	TFT_SEND_DATA(0x05);
	TFT_SEND_DATA(0x05);
	TFT_SEND_DATA(0x00);
	TFT_SEND_DATA(0x33);
	TFT_SEND_DATA(0x33);

	TFT_SEND_CMD(0xb7);			//Gate Control
	TFT_SEND_DATA(0x05);			//12.2v   -10.43v
	//--------------ST7789V Power setting---------------//
	TFT_SEND_CMD(0xBB);//VCOM
	TFT_SEND_DATA(0x3F);

	TFT_SEND_CMD(0xC0); //Power control
	TFT_SEND_DATA(0x2c);

	TFT_SEND_CMD(0xC2);		//VDV and VRH Command Enable
	TFT_SEND_DATA(0x01);

	TFT_SEND_CMD(0xC3);			//VRH Set
	TFT_SEND_DATA(0x0F);		//4.3+( vcom+vcom offset+vdv)

	TFT_SEND_CMD(0xC4);			//VDV Set
	TFT_SEND_DATA(0x20);				//0v

	TFT_SEND_CMD(0xC6);				//Frame Rate Control in Normal Mode
	TFT_SEND_DATA(0X01);			//111Hz

	TFT_SEND_CMD(0xd0);				//Power Control 1
	TFT_SEND_DATA(0xa4);
	TFT_SEND_DATA(0xa1);

	TFT_SEND_CMD(0xE8);				//Power Control 1
	TFT_SEND_DATA(0x03);

	TFT_SEND_CMD(0xE9);				//Equalize time control
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x08);
	//---------------ST7789V gamma setting-------------//
	TFT_SEND_CMD(0xE0); //Set Gamma
	TFT_SEND_DATA(0xD0);
	TFT_SEND_DATA(0x05);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x08);
	TFT_SEND_DATA(0x14);
	TFT_SEND_DATA(0x28);
	TFT_SEND_DATA(0x33);
	TFT_SEND_DATA(0x3F);
	TFT_SEND_DATA(0x07);
	TFT_SEND_DATA(0x13);
	TFT_SEND_DATA(0x14);
	TFT_SEND_DATA(0x28);
	TFT_SEND_DATA(0x30);
	 
	TFT_SEND_CMD(0XE1); //Set Gamma
	TFT_SEND_DATA(0xD0);
	TFT_SEND_DATA(0x05);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x08);
	TFT_SEND_DATA(0x03);
	TFT_SEND_DATA(0x24);
	TFT_SEND_DATA(0x32);
	TFT_SEND_DATA(0x32);
	TFT_SEND_DATA(0x3B);
	TFT_SEND_DATA(0x14);
	TFT_SEND_DATA(0x13);
	TFT_SEND_DATA(0x28);
	TFT_SEND_DATA(0x2F);

	TFT_SEND_CMD(0x21); 		//����
   
	TFT_SEND_CMD(0x29);         //������ʾ 
	
}

