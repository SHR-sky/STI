#include "tft_1.96_spi_240x240.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
/*************************************
*@brief open_backlight
*@details 打开屏幕背光
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
*@details 关闭屏幕背光
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
*@details 写入命名
*@param[in] cmd 8位命令
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
*@details 写入数据
*@param[in] data 8位数据
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
* @details  设置LCD显示窗口,设置完成后就可以连续发
			送颜色数据了，无需再一次一次设置坐标
* @param  startX：窗口起点x轴坐标
* 		   startY：窗口起点y轴坐标
* 		   width：窗口宽度
* 		   height：窗口高度
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
	
	TFT_SEND_CMD(0x2A);		//发送设置X轴坐标的命令0x2A
	//参数SC[15:0]	->	设置起始列地址，也就是设置X轴起始坐标
	TFT_SEND_DATA(startX>>8);				//先写高8位
	TFT_SEND_DATA(startX&0xFF);			//再写低8位
	//参数EC[15:0]	->	设置窗口X轴结束的列地址，因为参数usXwidth是窗口长度，所以要转为列地址再发送
	TFT_SEND_DATA((startX+width-1)>>8);				//先写高8位
	TFT_SEND_DATA((startX+width-1)&0x00FF);			//再写低8位

	TFT_SEND_CMD(0x2B);		//发送设置Y轴坐标的命令0x2B
	//参数SP[15:0]	->	设置起始行地址，也就是设置Y轴起始坐标
	TFT_SEND_DATA(startY>>8);				//先写高8位
	TFT_SEND_DATA(startY&0x00FF);			//再写低8位
	//参数EP[15:0]	->	设置窗口Y轴结束的列地址，因为参数usYheight是窗口高度，所以要转为行地址再发送
	TFT_SEND_DATA((startY+height-1)>>8);				//先写高8位
	TFT_SEND_DATA((startY+height-1)&0x00FF);			//再写低8位
	TFT_SEND_CMD(0x2C);			//开始往GRAM里写数据
}
/*************************************
*@brief TFT_display_point
*@details 画点函数
*@param[in] startX：X起始坐标
*			startY：Y起始坐标
*			color：点的颜色
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
*@details 显示图片函数,16位颜色数据先发高8位再发低八位
*@param[in] address:图片数据地址
*			startX：X起始坐标
*			startY：Y起始坐标
*@return void
*@author zx
*@date 2023-06-03
***************************************/
void TFT_display_image(const uint16_t *address, uint16_t startX, uint16_t startY)
{
	uint16_t image_width;//图片宽
	uint16_t image_hight;//图片高
	uint16_t x,y;
	image_width = address[0];
	image_hight = address[1];
	TFT_SetWindows(startX, startY, image_width, image_hight);
	for(y = 0; y < image_hight; y++)
	{
		for(x = 0; x < image_width; x++)
		{
			//发送图片数据，每次发送16位,先发高八位
			TFT_SEND_DATA(address[y*image_width + x + 2]>>8);
			TFT_SEND_DATA(address[y*image_width + x + 2]&0xff);
		}
	}	
}
/*************************************
*@brief TFT_display_en
*@details 显示一个英文字母
*@param[in] address：字符数据
			char_en:英文字母
*			startX：X起始坐标
*			startY：Y起始坐标
*			textColor：字体显示颜色
*			backgroundColor:背景色
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
*@details 显示英文字符串
*@param[in] address：字符数据
			str:英文字符串
*			startX：X起始坐标
*			startY：Y起始坐标
*			textColor：字体显示颜色
*			backgroundColor:背景色
*@return void
*@author RZY
*@date 2024-02-17
***************************************/
void TFT_display_en_string(const uint8_t *address ,const uint8_t *str ,uint16_t startX,uint16_t startY,uint16_t textColor, 
					uint16_t backgroundColor)
{
	uint16_t i;
	for(i = 0; i < strlen((const char*)str); i++)	//此处采用强制类型转换，避免uint8_t*与char*类型的冲突
	{
		TFT_display_en(address ,str[i] , startX + i*8, startY, textColor, backgroundColor);
	}

}

/*************************************
*@brief TFT_display_char16_16
*@details 显示16x16的汉字
*@param[in] address:图片数据地址
*			startX：X起始坐标
*			startY：Y起始坐标
*			textColor：字体显示颜色
*			backgroundColor:背景色
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
*@details 显示16x16的汉字(不带背景颜色，镂空)
*		  显示规则：一行一行显示，首先显示第一行的前八位，
*		  然后显示后八位，显示完成之后显示第二行，
*		  注意：数据取模时是低位在前高位在后（逆序），具体根
*		  据取模方向来确定
*@param[in] address:图片数据地址
*			startX：X起始坐标
*			startY：Y起始坐标
*			color：字体显示颜色
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
//		if(column>0 && column%2 == 0)//如果开启字体的高读会压缩到之前的一半
		y++;
		address++;
	}	
}  

/*************************************
*@brief TFT_full_color
*@details 在整个屏幕上填充颜色
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
*@details tft屏幕初始化函数，主要是定义引脚和启动驱动
*@param[in] void
*@return void
*@author zx
*@date 2022-12-18
***************************************/
void tft_196_240x240_init()
{
	GPIO_InitTypeDef GPIO_InitStruture;                                  //定义一个结构体变量
 	//硬件SPI引脚初始化
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


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);     	//使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       	//速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_4);
	
		//背光引脚 PC13 片选CS引脚PC14
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);                //使能GPIOC时钟
    //配置GPIOB的工作模式和初始化
    GPIO_InitStruture.GPIO_Mode = GPIO_Mode_Out_PP;                    //推挽输出
    GPIO_InitStruture.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;            //引脚PC13 PC14 
    GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;                   //速度50MHz
    GPIO_Init(GPIOC,&GPIO_InitStruture);
	
	//st7789驱动
	TFT_SCLK_Set();			//特别注意！！
	TFT_RST_Clr();
	Delay_ms(1000);
	TFT_RST_Set();
	Delay_ms(1000);
    TFT_SEND_CMD(0x11); 			//Sleep Out
	Delay_ms(120);               //DELAY120ms 
	 	  //-----------------------ST7789V Frame rate setting-----------------//
//************************************************
	TFT_SEND_CMD(0x3A);        //颜色数据格式RGB565 65k真彩色
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
	↓
**/
	TFT_SEND_CMD(0x36);                 // 屏幕显示方向设置

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

#if DISPLAY_OVERTURN//不翻转显示
	lcd_data |= (1<<5);//翻转显示
#else
	lcd_data |= (0<<5);	
#endif
	TFT_SEND_DATA(lcd_data);

/*****显示位置，注意，x和y是根据屏幕显示方向来定的，不一定统一**********/
	TFT_SEND_CMD(0x2B); //设置显示区域 x轴起始及结束坐标 
	TFT_SEND_DATA((start_x>>8)&0xff);
	TFT_SEND_DATA(start_x&0xff);
	TFT_SEND_DATA((end_x>>8)&0xff);
	TFT_SEND_DATA(end_x&0xff);

	TFT_SEND_CMD(0x2B); //设置显示区域 Y轴起始及结束坐标
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

	TFT_SEND_CMD(0x21); 		//反显
   
	TFT_SEND_CMD(0x29);         //开启显示 
	
}

