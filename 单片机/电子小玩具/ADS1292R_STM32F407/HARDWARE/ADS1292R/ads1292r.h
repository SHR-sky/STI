#ifndef __ADS1292R_H
#define __ADS1292R_H

#include "spi.h"
// #include "hmi_uart.h"

#define ADS1292R_SCLK_Pin 					GPIO_Pin_13
#define ADS1292R_SCLK_GPIO_Port 		GPIOB
#define ADS1292R_MISO_Pin 					GPIO_Pin_14
#define ADS1292R_MISO_GPIO_Port 		GPIOB
#define ADS1292R_MOSI_Pin 					GPIO_Pin_15
#define ADS1292R_MOSI_GPIO_Port 		GPIOB

#define ADS1292R_CS_Pin 						GPIO_Pin_12
#define ADS1292R_CS_GPIO_Port 			GPIOB
#define ADS1292R_DRDY_Pin 					GPIO_Pin_8
#define ADS1292R_DRDY_GPIO_Port 		GPIOA
#define ADS1292R_DRDY_EXTI_IRQn 		EXTI9_5_IRQn
#define ADS1292R_DRDY						 		GPIO_ReadInputDataBit(ADS1292R_DRDY_GPIO_Port, ADS1292R_DRDY_Pin) //PA8
#define ADS1292R_START_Pin 					GPIO_Pin_11
#define ADS1292R_START_GPIO_Port 		GPIOB
#define ADS1292R_RES_Pin 						GPIO_Pin_10
#define ADS1292R_RES_GPIO_Port 			GPIOB

#define ADS1292R_SCLK_Port 					ADS1292R_SCLK_GPIO_Port
#define ADS1292R_MISO_Port 					ADS1292R_MISO_GPIO_Port
#define ADS1292R_MOSI_Port 					ADS1292R_MOSI_GPIO_Port
#define ADS1292R_CS_Port 						ADS1292R_CS_GPIO_Port
#define ADS1292R_DRDY_Port 					ADS1292R_DRDY_GPIO_Port
#define ADS1292R_START_Port 				ADS1292R_START_GPIO_Port
#define ADS1292R_RES_Port 					ADS1292R_RES_GPIO_Port

#define ADS1292R_CS_HIGH 						GPIO_SetBits(ADS1292R_CS_Port, ADS1292R_CS_Pin)
#define ADS1292R_CS_LOW 						GPIO_ResetBits(ADS1292R_CS_Port, ADS1292R_CS_Pin)
#define ADS1292R_RES_HIGH 					GPIO_SetBits(ADS1292R_RES_Port, ADS1292R_RES_Pin)
#define ADS1292R_RES_LOW 						GPIO_ResetBits(ADS1292R_RES_Port, ADS1292R_RES_Pin)
#define ADS1292R_START_HIGH 				GPIO_SetBits(ADS1292R_START_Port, ADS1292R_START_Pin)
#define ADS1292R_START_LOW 					GPIO_ResetBits(ADS1292R_START_Port, ADS1292R_START_Pin)

/* ADS1292R command definition */
#define ADS1292R_WAKEUP 						0X02
#define ADS1292R_STANDBY 						0X04
#define ADS1292R_ADSRESET 					0X06
#define ADS1292R_START 							0X08
#define ADS1292R_STOP 							0X0A
#define ADS1292R_OFFSETCAL 					0X1A
/* data read command */
#define ADS1292R_RDATAC							0X10
#define ADS1292R_SDATAC 						0X11
#define ADS1292R_RDATA 							0X12
/* reg read command */
#define ADS1292R_RREG 							0X20 //读取  001r rrrr(首字节) 000n nnnn(2字节) only defined high 8 bits, low 8 bits are set when sending command
#define ADS1292R_WREG 							0X40 //写入  010r rrrr(首字节) 000n nnnn(2字节) 
                           								/*	r rrrr=register address
		                                          n nnnn=data to send/write*/

/* ADS1292R internal reg address definition */
#define ADS1292R_ID 0X00        		//ID control reg
#define ADS1292R_CONFIG1 0X01   		//configuration reg 1
#define ADS1292R_CONFIG2 0X02   		//configuration reg 2
#define ADS1292R_LOFF 0X03      		//lead-off control reg
#define ADS1292R_CH1SET 0X04    		//channel 1 setting
#define ADS1292R_CH2SET 0X05    		//channel 2 setting
#define ADS1292R_RLD_SENS 0X06  		//RLD sense selection
#define ADS1292R_LOFF_SENS 0X07 		//lead-off sense selection
#define ADS1292R_LOFF_STAT 0X08 		//lead-off status
#define ADS1292R_RESP1 0X09     		//respiration control reg 1
#define ADS1292R_RESP2 0X0A     		//respiration control reg 2
#define ADS1292R_GPIO 0X0B      		//GPIO control reg

/* reg configuration */
/* ID  address = 0x00 */
#define DEVICE_ID_ADS1292 	0X53		// 0110 0011
#define DEVICE_ID_ADS1292R 	0X73		// 0111 0011
/* CONFIG1  address = 0x01 */
#define DATA_RATE_125SPS	0X00			// 采样率
#define DATA_RATE_250SPS	0X01
#define DATA_RATE_500SPS	0X02
#define DATA_RATE_1kSPS 	0X03
#define DATA_RATE_2kSPS 	0X04
#define DATA_RATE_4kSPS 	0X05
#define DATA_RATE_8kSPS 	0X06
/* CONFIG2  address = 0x02 */
#define PDB_LOFF_COMP_OFF 0 				//Lead-off comparators disabled(default)	导联脱落比较器掉电（默认）
#define PDB_LOFF_COMP_ON 	1

#define PDB_REFBUF_OFF		0					//Reference buffer is powered down (default)	基准电压缓冲器掉电（默认）
#define PDB_REFBUF_ON 		1

#define VREF_242V 				0 				//2.42 V reference(default)	内部参考电压2.42V（默认）
#define VREF_4V 					1  				//4.033 V referenceV				内部参考电压4.033V

#define CLK_EN_OFF 				0 				//Oscillator clock output disabled(default)	振荡器时钟输出禁用（默认）
#define CLK_EN_ON 				1

#define INT_TEST_OFF 			0 				//close internal test signal(default)	关闭内部测试信号（默认）
#define INT_TEST_ON 			1

#define TEST_FREQ_DC 			0 				//test with DC(default) 测试信号频率为直流（默认）
#define TEST_FREQ_AC 			1 				//1Hz square wave			 测试信号频率为1Hz方波
/* CHSET  address = 0x04 */
#define PD_ON 						0 				//normal operation(default) 正常操作（默认）
#define PD_OFF 						1					//通道1断电

#define GAIN_6 0 										//gain 6(default) PGA增益为6倍（默认）
#define GAIN_1 1
#define GAIN_2 2
#define GAIN_3 3
#define GAIN_4 4
#define GAIN_8 5
#define GAIN_12 6

#define MUX_Normal_input 	0					//Normal electrode input(default) 正常电极输入（默认）
#define MUX_input_shorted 1					//Input shorted 								 	输入短路（用于测量偏移）
#define MUX_RLD_MEASURE 	2					//RLD_MEASURE											右腿驱动电路测量
#define MUX_MVDD					3					//MVDD for supply measurement			用于MVDD的电源测量，参考手册8.6.1.5页
#define MUX_Temperature	 	4					//Temperature Sensor							温度传感器
#define MUX_Test_signal 	5   			//Test signal											测试信号								
#define MUX_RLD_DRP 			6					//RLD_DRP 												正输入连接到RLDIN
#define MUX_RLD_DRM 			7					//RLD_DRM 												负输入连接到RLDIN
#define MUX_RLD_DRPM 			8					//RLD_DRPM 												正、负输入均连接到RLDIN
#define MUX_RSP_IN3P 			9					//Route IN3P and IN3N to channel 1 inputs		将IN3P IN3N连接到通道1输入
#define MUX_Reserved			10				//Reserved												保留
/* RLD_SENS  address = 0x06 */
#define PDB_RLD_OFF				0 				//右腿驱动电路电源关闭(默认)
#define PDB_RLD_ON 				1

#define RLD_LOFF_SENSE_OFF 					0 					//右腿驱动电路导联脱落传感器关闭(默认)
#define RLD_LOFF_SENSE_ON 					1

#define RLD_CANNLE_OFF	 	0 				//(default)
#define RLD_CANNLE_ON 		1
/* LOFF_SENS  address = 0x07 */
#define FLIP2_OFF 				0 				// Current direction selection(default)
#define FLIP2_ON 					1

#define FLIP1_OFF 				0 				//(default)
#define FLIP1_ON 					1

#define LOFF_CANNLE_OFF 	0 				//(default)
#define LOFF_CANNLE_ON 		1
/* RESP1  address = 0x09 */
#define RESP_DEMOD_OFF 		0 				//(default)
#define RESP_DEMOD_ON 		1

#define RESP_MOD_OFF 			0					//(default)
#define RESP_MOD_ON 			1

#define RESP_CTRL_CLOCK_INTERNAL 		0
#define RESP_CTRL_CLOCK_EXTERNAL 		1
/* RESP2  address = 0x0A */
#define CALIB_OFF 				0 				//(default)
#define CALIB_ON 					1

#define FREQ_32K 					0					 //(default)
#define FREQ_64K 					1

#define RLDREF_INT_EXTERN 					0    			 //external fed signal RLDREF
#define RLDREF_INT_INTERNALLY 			1					 //internal

typedef struct
{
    uint8_t Data_Rate; //ADC channel samping rate
} ADS1292_CONFIG1;

typedef struct
{
    uint8_t Pdb_Loff_Comp; //Lead-off comparator power-down
    uint8_t Pdb_Refbuf;    //Reference buffer power-down
    uint8_t Vref;          //internal voltage reference
    uint8_t Clk_EN;        //CLK connection
    uint8_t Int_Test;      //Test signal selection
    uint8_t Test_Freq;     //Test signal frequency
} ADS1292_CONFIG2;

typedef struct
{
    uint8_t PD;   //Channel power-down
    uint8_t GAIN; //PGA gain setting
    uint8_t MUX;  //input selection
} ADS1292_CHSET;

typedef struct
{
    uint8_t Pdb_Rld;        //RLD buffer power
    uint8_t Rld_Loff_Sense; //RLD lead-off sense function
    uint8_t Rld2N;          //Channel 2 RLD negative inputs
    uint8_t Rld2P;          //Channel 2 RLD positive inputs
    uint8_t Rld1N;          //Channel 1 RLD negative inputs
    uint8_t Rld1P;          //RLD1P: Channel 1 RLD positive inputs
} ADS1292_RLD_SENS;

typedef struct
{
    uint8_t Flip2; //Current direction selection
    uint8_t Flip1;
    uint8_t Loff2N; //Channel 2 lead-off detection negative inputs
    uint8_t Loff2P; //Channel 2 lead-off detection positive inputs
    uint8_t Loff1N; //Channel 1 lead-off detection negative inputs
    uint8_t Loff1P; //Channel 1 lead-off detection positive inputs
} ADS1292_LOFF_SENS;

typedef struct
{
    uint8_t RESP_DemodEN; //Enables respiration demodulation circuitry
    uint8_t RESP_modEN;   //Enables respiration modulation circuitry
    uint8_t RESP_ph;
    uint8_t RESP_Ctrl;
} ADS1292_RESP1;

typedef struct
{
    uint8_t Calib;      //Calibration on
    uint8_t freq;       //Respiration control frequency
    uint8_t Rldref_Int; //RLDREF signal
} ADS1292_RESP2;

void ADS1292R_SetBuffer(void);

uint8_t ADS1292R_SPI_TransmitReceive(uint8_t tx_data);
uint8_t ADS1292R_SendCmd(uint8_t cmd);
uint8_t ADS1292R_RwReg(uint8_t cmd, uint8_t data);
void ADS1292R_WriteMultiRegs(uint8_t reg, uint8_t *data, uint8_t len);
void ADS1292R_ReadMultiRegs(uint8_t reg, uint8_t *data, uint8_t len);

void ADS1292R_GetValue(void);

void ADS1292R_Init(void);
void ADS1292R_ADCStartNormal(void);
void ADS1292R_ADCStartTest(void);

#endif
