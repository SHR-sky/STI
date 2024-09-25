#include "ads1292r.h"
#include "delay.h"
#include "usart.h"
#include "exti.h"
#include "spi.h"
#include "sys.h"

// #define ADS1292R_DEBUG

uint8_t ADS1292R_data_buf[9] = {0};   //ADS1292R receive data buffer 接受数据缓冲区
volatile uint8_t ADS1292R_tmp[9] = {0};
volatile uint8_t ADS1292R_receive_flag = 0;
volatile u16 counter = 0;
uint8_t ADS1292R_reg[12];             //ADS1292R register array      寄存器数组

ADS1292_CONFIG1 ADS1292R_config1 = {DATA_RATE_500SPS};     //set sample rate to 500 采样率为500Hz

ADS1292_CONFIG2 ADS1292R_config2 = {PDB_LOFF_COMP_ON,      //Lead-off comparators enabled      导联脱落检测开启
                                    PDB_REFBUF_ON,         //Reference buffer enabled          内部参考缓冲器开启
                                    VREF_242V,             //2.42V reference voltage           2.42V作为参考电压
                                    CLK_EN_OFF,            //interal clock output disabled     内部时钟输出关闭
                                    INT_TEST_OFF,          //close test signal                 测试信号关闭
                                    TEST_FREQ_DC
                                   };         //set test signal ferquency is DC   测试信号频率为直流

ADS1292_CHSET ADS1292R_ch1set = {PD_ON,                    //ch1 power on                      通道1 开启供电
                                 GAIN_2,                   //set PGA gain is 2                 通道1 PGA增益设置为2
                                 MUX_Normal_input          //normal electrode input            通道1 正常输入模式
                                };

ADS1292_CHSET ADS1292R_ch2set = {PD_ON,                    //ch2 power on                      通道2 开启供电
                                 GAIN_6,                   //set PGA gain is 6                 通道2 PGA增益设置为6
                                 MUX_Normal_input          //normal electrode input            通道2 正常输入模式
                                };

ADS1292_RLD_SENS ADS1292R_rld_sens = {PDB_RLD_ON,          //RLD buffer power on               右腿驱动电路 电源开启
                                      RLD_LOFF_SENSE_OFF,  //RLD lead-off sense off            右腿驱动电路 导联脱落传感关闭
                                      /*(RLD lead-off testing and RLD input may not work together)（RLD驱动和输入不能同时工作）*/
                                      RLD_CANNLE_ON,       //RLD2 input on                     通道2 RLD负极输入打开
                                      RLD_CANNLE_ON,       //RLD2 input on                     通道2 RLD正极输入打开
                                      /*breathing channel don't need RLD input  呼吸通道不需要RLD输入*/
                                      RLD_CANNLE_OFF,      //通道1不连接，作为呼吸测量通道
                                      RLD_CANNLE_OFF      //通道1不连接，作为呼吸测量通道
                                     };

ADS1292_LOFF_SENS ADS1292R_loff_sens = {FLIP2_OFF,         //disable the controller of current directions for lead-off derivation for channel 2        禁用通道2的导联脱落检测的电流方向控制器
                                        FLIP1_OFF,         //disable the controller of current directions for lead-off derivation for channel 1        禁用通道1的导联脱落检测的电流方向控制器
                                        RLD_CANNLE_ON,     //enable lead-off detection         通道2负输入端开启导联脱落检测
                                        RLD_CANNLE_ON,     //enable lead-off detection         通道2正输入端开启导联脱落检测
                                        RLD_CANNLE_OFF,    //breathing channel don't need lead-off detection   呼吸通道(通道1)负输入端不需要导联脱落检测
                                        RLD_CANNLE_OFF
                                       };   //breathing channel don't need lead-off detection   呼吸通道(通道1)正输入端不需要导联脱落检测

ADS1292_RESP1 ADS1292R_resp1 = {RESP_DEMOD_ON,             //enable channel 1 demodulation circuitry                                             启用通道1上的呼吸解调电路
                                RESP_MOD_ON,               //enable channel 1 modulation circuitry                                               启用通道1上的呼吸调制电路
                                0X0A,                      //the phase of respiration demodulation control signal 112.5 degrees                  呼吸解调控制信号的相位是112.5°
                                RESP_CTRL_CLOCK_INTERNAL
                               }; //set internal respiration with interal clock                                         用内部时钟

ADS1292_RESP2 ADS1292R_resp2 = {CALIB_OFF,                //disable offset calibration                                                           校准禁用
                                //turn on channel offset correction and execute OFFSETCAL command if change the gain during conversion           如果在转换过程中改变增益，则打开通道偏移校正并执行OFFSETCAL命令
                                FREQ_32K,                 //respiration control frequency                                                        呼吸控制频率设置为32kHz
                                RLDREF_INT_INTERNALLY
                               };   //RLDREF generated internality (AVDD + AVSS) / 2                                       RLDREF信号源使用内部馈电

/**
  * @name   ADS1292R_SetBuffer(void)
  *
  * @brief  Sets register arrays. 设置寄存器
  *
  * @note
  *
  * @retval None
  */
void ADS1292R_SetBuffer(void)
{
    ADS1292R_reg[ADS1292R_ID] = DEVICE_ID_ADS1292R;                          //ID只读

    ADS1292R_reg[ADS1292R_CONFIG1] = 0x00;                                   //0000 0aaa [7] 0连续转换模式 1单次模式  [6:3] 必须为0
    ADS1292R_reg[ADS1292R_CONFIG1] |= ADS1292R_config1.Data_Rate;            //          [2:0] aaa 采样率设置采样率

    ADS1292R_reg[ADS1292R_CONFIG2] = 0x80;                                   //1abc d0ef [7] 必须为1  [2] 必须为0
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Test_Freq;            //          [0]f 设置测试信号为1HZ、±1mV方波，0为直流
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Int_Test << 1;        //          [1]e 是否打开内部测试信号
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Clk_EN << 3;          //          [3]d CLK引脚输出时钟脉冲？
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Vref << 4;            //          [4]c 内部参考电压设置，默认2.42V
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Pdb_Refbuf << 5;      //          [5]b 内部参考缓冲器是否掉电
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Pdb_Loff_Comp << 6;   //          [6]a 导联脱落比较器是否掉电

    ADS1292R_reg[ADS1292R_LOFF] = 0xF0;                                      //abc1 de0f [7:5] 设置导联脱落比较器阈值 111为正极70% 负极30% [4]必须为1
    //          [3:2] 导联脱落电流幅值 00为6nA(默认) [1] 必须为0 [0] 导联脱落检测方式 0 DC(默认) 1 AC

    ADS1292R_reg[ADS1292R_CH1SET] = 0x00;                                    //abbb cccc
    ADS1292R_reg[ADS1292R_CH1SET] |= ADS1292R_ch1set.MUX;                    //          [3:0]cccc 设置通道1输入模式
    ADS1292R_reg[ADS1292R_CH1SET] |= ADS1292R_ch1set.GAIN << 4;              //          [6:4]bbb 设置PGA增益
    ADS1292R_reg[ADS1292R_CH1SET] |= ADS1292R_ch1set.PD << 7;                //          [7]  a   通道1断电？

    ADS1292R_reg[ADS1292R_CH2SET] = 0x00;                                    //abbb cccc
    ADS1292R_reg[ADS1292R_CH2SET] |= ADS1292R_ch1set.MUX;                    //          [3:0]cccc 设置通道1输入模式
    ADS1292R_reg[ADS1292R_CH2SET] |= ADS1292R_ch1set.GAIN << 4;              //          [6:4]bbb 设置PGA增益
    ADS1292R_reg[ADS1292R_CH2SET] |= ADS1292R_ch1set.PD << 7;                //          [7]  a   通道2断电？

    ADS1292R_reg[ADS1292R_RLD_SENS] = 0XC0;                                  //00ab cdef [7:6] 11 PGA斩波频率 fMOD/4
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld1P;              //          [0]f 该位控制通道1正输入  用于右腿驱动的输出
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld1N << 1;         //          [1]e 这个位控制通道1负输入  用于右腿驱动的输出
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld2P << 2;         //          [2]d 该位控制通道2正输入  用于右腿驱动的输出
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld2N << 3;         //          [3]c 这个位控制通道2负输入  用于右腿驱动的输出
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld_Loff_Sense << 4;//          [4]b 该位使能RLD导联脱落检测功能
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Pdb_Rld << 5;       //          [5]a 该位决定RLD缓冲电源状态

    ADS1292R_reg[ADS1292R_LOFF_SENS] = 0X00;                                 //00ab cdef [7:6] 必须为0
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Loff1P;           //          [0]f 该位控制通道1正输入端的导联脱落检测
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Loff1N << 1;      //          [1]e 该位控制通道1负输入端的导联脱落检测
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Loff2P << 2;      //          [2]d 该位控制通道2正输入端的导联脱落检测
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Loff2N << 3;      //          [3]c 该位控制通道2负输入端的导联脱落检测
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Flip1 << 4;       //          [4]b 这个位用于控制导联脱落检测通道1的电流的方向
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Flip2 << 5;       //          [5]a 这个位用于控制导联脱落检测通道2的电流的方向

    ADS1292R_reg[ADS1292R_LOFF_STAT] = 0X00; /*[6]=0 set the ratio between fCLK and fMOD, fCLK=fMOD/4
                                            [4:0]read only, set status of lead-off and elevtrode status */
    //[6]0 设置fCLK和fMOD之间的模分频比 fCLK=fMOD/4  [4:0]只读，导联脱落和电极连接状态

    ADS1292R_reg[ADS1292R_RESP1] = 0X02;                                     //abcc cc1d [1]必须为1
    ADS1292R_reg[ADS1292R_RESP1] |= ADS1292R_resp1.RESP_Ctrl;                //          [0]d  这个位设置呼吸回路的模式
    ADS1292R_reg[ADS1292R_RESP1] |= ADS1292R_resp1.RESP_ph << 2;             //          [5:2]c 这些位控制呼吸解调控制信号的相位
    ADS1292R_reg[ADS1292R_RESP1] |= ADS1292R_resp1.RESP_modEN << 6;          //          [6]b  这个位启用和禁用通道1上的调制电路
    ADS1292R_reg[ADS1292R_RESP1] |= ADS1292R_resp1.RESP_DemodEN << 7;        //          [7]a  这个位启用和禁用通道1上的解调电路

    ADS1292R_reg[ADS1292R_RESP2] = 0X01;                                     //a000 0bc1 [6:3]必须为0 [0]必须为1
    ADS1292R_reg[ADS1292R_RESP2] |= ADS1292R_resp2.Rldref_Int << 1;          //          [1]c RLDREF信号源外部馈电？
    ADS1292R_reg[ADS1292R_RESP2] |= ADS1292R_resp2.freq << 2;                //          [2]b 呼吸频率设置
    ADS1292R_reg[ADS1292R_RESP2] |= ADS1292R_resp2.Calib << 7;               //          [7]a 启动通道偏移校正？

    ADS1292R_reg[ADS1292R_GPIO] = 0X0C;                                      //GPIO set input [7:4] must be 0  [3:2]==11 GPIO is input [1:0] when set input，return GPIO status; when set output , set the output value
    //GPIO设为输入  [7:4] 必须为0  [3:2]==11 GPIO为输入(默认)  [1:0] 当读取寄存器时，返回的数据与GPIO外部引脚的状态相对应，无论它们被编程为输入还是输出。作为输出，对GPIOD的写入设置输出值。作为输入，对GPIOD的写入没有任何影响。
}

/**
  * @name   ADS1292R_SPI_TransmitReceive(uint8_t tx_data)
  *
  * @brief  Using SPI to transmit data and receive data. 用SPI2传输
  *
  * @note
  *
  * @param  tx_data: the data to transmit with SPI
  *
  * @retval rx_data
  */
u8 ADS1292R_SPI_TransmitReceive(u8 tx_data)
{
    return SPI2_ReadWriteByte(tx_data);
}

/**
  * @name   ADS1292R_SendCmd(uint8_t cmd)
  *
  * @brief  Send command to ADS1292R. 写命令
  *
  * @note
  *
  * @param  cmd: the command to send
  *
  * @retval rx_data
  *
  * @version
  */
uint8_t ADS1292R_SendCmd(uint8_t cmd)
{
    u8 rx_data = 0;
    ADS1292R_CS_LOW;
    delay_us(10);
    rx_data = ADS1292R_SPI_TransmitReceive(cmd);
    delay_us(10);
    ADS1292R_CS_HIGH;
    return rx_data;
}

/**
  * @name   ADS1292R_RwReg(uint8_t cmd,uint8_t data)
  *
  * @brief  Write and read internal reg of ADS1292R. 读写寄存器
  *
  * @note
  *
  * @param  cmd: read or write command
  *            @arg  ADS1292R_RREG
  *            @arg  ADS1292R_WREG
  * @param  data: content to send to register
  *
  * @retval rx_data
  */
uint8_t ADS1292R_RwReg(uint8_t cmd, uint8_t data)
{
    uint8_t rx_data = 0;

    ADS1292R_CS_LOW;
    delay_us(10);
    ADS1292R_SPI_TransmitReceive(cmd);
    delay_us(10);
    ADS1292R_SPI_TransmitReceive(0x00);

    if ((cmd & ADS1292R_RREG) == ADS1292R_RREG)
    {
        rx_data = ADS1292R_SPI_TransmitReceive(0x00);
    }
    else
    {
        rx_data = ADS1292R_SPI_TransmitReceive(data);
    }
    delay_us(10);
    ADS1292R_CS_HIGH;
    return rx_data;
}

/**
  * @name   ADS1292R_WriteMultiRegs(uint8_t reg, uint8_t *data,uint8_t len)
  *
  * @brief  Write some numbers of registers from specified register. 写多个寄存器
  *
  * @note
  *
  * @param  reg: starting reg address
  * @param  data: the pointer to data
  * @param  len: data's length
  *
  * @retval None
  */
void ADS1292R_WriteMultiRegs(uint8_t reg, u8 *data, uint8_t len)
{
    uint8_t i;

    ADS1292R_CS_LOW;
    delay_us(10);

    ADS1292R_SPI_TransmitReceive(ADS1292R_WREG | reg);
    delay_us(10);
    ADS1292R_SPI_TransmitReceive(len - 1);

    for (i = 0; i < len; i++)
    {
        delay_us(10);
        ADS1292R_SPI_TransmitReceive(*data);
        data++;
    }

    delay_us(10);
    ADS1292R_CS_HIGH;
}

/**
  * @name   ADS1292R_ReadMultiRegs(uint8_t reg, uint8_t *data,uint8_t len)
  *
  * @brief  Read some numbers of registers from specified register. 读多个寄存器
  *
  * @note
  *
  * @param  reg: starting reg address
  * @param  data: the pointer to data
  * @param  len: data's length
  *
  * @retval None
  */
void ADS1292R_ReadMultiRegs(uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t i;

    ADS1292R_CS_LOW;
    delay_us(10);
    ADS1292R_SPI_TransmitReceive(ADS1292R_RREG | reg);
    delay_us(10);
    ADS1292R_SPI_TransmitReceive(len - 1);

    for (i = 0; i < len; i++)
    {
        delay_us(10);
        *data = ADS1292R_SPI_TransmitReceive(0X00);
        data++;
    }

    delay_us(10);
    ADS1292R_CS_HIGH;
}

/**
  * @name   ADS1292R_GetValue(void)
  *
  * @brief  读取72位数据 1100+LOFF_STAT[4:0]+GPIO[1:0]+13*0 + 2*CH*24位 ，共9个字节。
	*					24位状态位 + 24位通道位*2 = 72位数据
	*
	*					24位状态位[2:0]: 1100 + LOFF_STAT[4:0] + GPIO[1:0] + 13个0
	*					24位通道位[5:3] [8:6]:  CH1是RESP CH2是ECG MSB模式输出 高位先输出 [5]<<16+[4]<<8[5]
  *
  * @note   1100 LOFF_STAT[4   3   2   1   0 ] //导联脱落相关的信息在LOFF_STAT寄存器里
	*            				 RLD  1N2N 1N2P 1N1N 1N1P
	*     		例 C0 00 00 FF E1 1A FF E1 52
  *
  * @param  无输入参数
  *
  * @retval 无返回值。收到的信息存在ADS1292R_data_buf数组里了
  */
void ADS1292R_GetValue(void)
{
    u8 i;
    ADS1292R_CS_LOW;
    //读9个字节的数据
    for(i=0; i<9; i++) {
        ADS1292R_data_buf[i] = SPI2_ReadWriteByte(0x00);
    }
    ADS1292R_CS_HIGH;
}

/**
  * @name   ADS1292R_Init(void)
  *
  * @brief  Init ADS1292R.
  *
  * @note
  *
  * @param  None
  *
  * @retval None
  */
void ADS1292R_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE, ENABLE);
    SPI2_Init();//初始化SPI引脚
    SPI2_SetSpeed(SPI_BaudRatePrescaler_8);		//设置为 42M/8=5.25M时钟,高速模式
    //250ns 频率4.5M	发送八组时钟需要23 us
    //125ns 频率9M，		发送八组时钟需要14 us
    //55ns 频率18M		发送八组时钟需要9.2us
    //30ns 36M				发送八组时钟需要9.2us
    //手册10页里写的最小时钟周期为50ns

    //DRDY	//待机时高电平，采集时低电平有效
    GPIO_InitStructure.GPIO_Pin = ADS1292R_DRDY_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(ADS1292R_DRDY_GPIO_Port, &GPIO_InitStructure);//初始化GPIO

    // CS
    GPIO_InitStructure.GPIO_Pin = ADS1292R_CS_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(ADS1292R_CS_GPIO_Port, &GPIO_InitStructure);//初始化GPIO

		// START
		GPIO_InitStructure.GPIO_Pin = ADS1292R_START_Pin;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(ADS1292R_START_GPIO_Port, &GPIO_InitStructure);
		ADS1292R_START_LOW;
		
		// RESET
		GPIO_InitStructure.GPIO_Pin = ADS1292R_RES_Pin;
		GPIO_Init(ADS1292R_RES_GPIO_Port, &GPIO_InitStructure);
    ADS1292R_RES_HIGH;
    
    //DRDY中断初始
    EXTIX_Init();       //初始化外部中断输入
	
    uint8_t device_id = 0;

    ADS1292R_CS_HIGH;
    ADS1292R_SendCmd(ADS1292R_SDATAC);   //停止连续读取数据模式
    delay_ms(100);
    ADS1292R_SendCmd(ADS1292R_ADSRESET);  //复位
    delay_ms(1000);
    ADS1292R_SendCmd(ADS1292R_SDATAC);   //停止连续读取数据模式
    delay_ms(100);

    while (device_id != DEVICE_ID_ADS1292R)
    {
        device_id = ADS1292R_RwReg(ADS1292R_RREG | ADS1292R_ID, 0x00);
        delay_ms(500);
    }
		
		
		
		#ifdef ADS1292R_DEBUG
				printf("ADS1292R detected\r\n");
				u8 i;
				u8 REG_Cache[12];
				
				ADS1292R_ReadMultiRegs(ADS1292R_RREG | ADS1292R_ID, REG_Cache, 12);
				printf("read default REG:\r\n");
				for(i=0; i<12; i++	)	//读默认寄存器
					printf("%d: %02X\r\n",i,REG_Cache[i]);
				
		#endif
		
		ADS1292R_SetBuffer(); // 将寄存器的值存入数组
    ADS1292R_WriteMultiRegs(ADS1292R_CONFIG1, ADS1292R_reg+1, 11);
		
		#ifdef ADS1292R_DEBUG
				printf("Write REG Successful!\r\n");
				
				ADS1292R_ReadMultiRegs(ADS1292R_RREG | ADS1292R_ID, REG_Cache, 12);
				printf("writed REG:\r\n");
				for(i=0; i<12; i++	)	//读默认寄存器
					printf("%d: %02X\r\n",i,REG_Cache[i]);
		#endif
		
}

/**
  * @name   ADS1292R_ADCStartNormal(void)
  *
  * @brief  Setup ADS1292R with default mode.
  *
  * @note
  *
  * @param  none
  *
  * @retval None
  *
  * @version
  */
void ADS1292R_ADCStartNormal(void)
{
    ADS1292R_SendCmd(ADS1292R_SDATAC); // 关闭连读数据读取模式
    delay_ms(100);

    ADS1292R_SetBuffer();
    ADS1292R_WriteMultiRegs(ADS1292R_CONFIG1, ADS1292R_reg + 1, 11);
    delay_ms(10); // 将初始化的寄存器配置写入

    ADS1292R_SendCmd(ADS1292R_RDATAC); // 打开数据连续读取模式
    delay_ms(10);
    ADS1292R_SendCmd(ADS1292R_START); // 开始转换
    delay_ms(10);
}

/**
  * @name   ADS1292R_ADCStartTest(void)
  *
  * @brief  Setup ADS1292R with testing mode.
  *
  * @note
  *
  * @param  none
  *
  * @retval None
  */
void ADS1292R_ADCStartTest(void)
{
    ADS1292R_config2.Int_Test = INT_TEST_ON;   //open internal testing signal
    ADS1292R_config2.Test_Freq = TEST_FREQ_AC; //test with 1 Hz square wave

    ADS1292R_ch1set.PD = PD_ON;            //open channel 1,2
    ADS1292R_ch1set.GAIN = GAIN_6;         //set PGA gain is 6
    ADS1292R_ch1set.MUX = MUX_Test_signal; //set testing mode

    ADS1292R_ch2set.PD = PD_ON;
    ADS1292R_ch2set.GAIN = GAIN_6;
    ADS1292R_ch2set.MUX = MUX_Test_signal;

    ADS1292R_resp1.RESP_DemodEN = RESP_DEMOD_OFF; //close demodulation and modulation
    ADS1292R_resp1.RESP_modEN = RESP_MOD_OFF;
    ADS1292R_resp1.RESP_ph = 0X00;                       //the phase is 0 degree
    ADS1292R_resp1.RESP_Ctrl = RESP_CTRL_CLOCK_EXTERNAL; //set internal respiration with external clock

    ADS1292R_SendCmd(ADS1292R_SDATAC); //stop Read Data Continuous mode
    delay_ms(100);
    ADS1292R_WriteMultiRegs(ADS1292R_WREG | ADS1292R_CONFIG1, ADS1292R_reg + 1, 11);
    delay_ms(10);

    ADS1292R_SendCmd(ADS1292R_RDATAC); //enable Read Data Continuous mode
    delay_ms(10);
    ADS1292R_SendCmd(ADS1292R_START); //start conversions
    delay_ms(10);
}
