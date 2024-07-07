#ifndef _spi_H
#define _spi_H

#include "system.h"

#define CS_1 GPIO_WriteBit(GPIOB,GPIO_Pin_1,SET)
#define CS_0 GPIO_WriteBit(GPIOB,GPIO_Pin_1,RESET)

#define FPGA_RESET_1 GPIO_WriteBit(GPIOB,GPIO_Pin_0,SET)
#define FPGA_RESET_0 GPIO_WriteBit(GPIOB,GPIO_Pin_0,RESET)

void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节
void ResetPin_Init(void);
uint8_t SPI1_RW1B(u8 txdata);
uint16_t SPI1_RW2B(uint8_t txdata1,uint8_t txdata2);
uint8_t Wait_Fpga(void);
void SPI_Read_Fpga(uint8_t txdata,uint16_t *buf,u16 size);
void Add_SPI(uint16_t *data1,uint16_t *data2,uint16_t size);
void Deal_Data(uint16_t *spi_data,float *data,u16 size);


#endif
