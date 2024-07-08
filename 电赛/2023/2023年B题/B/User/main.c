#include "sys.h"
#include "math.h"

#define CONST_R1 44200
#define CONST_R2 158000
#define CONST_C 3 // pf
#define pi 3.1415926
#define E 2.718281828
#define CONST_LN2 0.69314718
#define MAX_DIFF 1 // 误差值，需更改 0.01/16 ? 存疑
#define RELAY_CTR PDout(1)

uint8_t flag = 0;

// 电缆速度系数
// 可以采用安慰按钮，此处取0.672285，或者修改为更好的值
double k;

// 反馈阻值
const double R0 = 0.001; // 1k

// 单位长度寄生电容（pF）(拟合)
// double Cp = 95.27;

// 单位长度电阻
double Rp = 0;

extern int cnt;
// 测量得到的频率
int cntFre;

// 接的线缆电容
double C_line = 0;

// 多次测量得结果
double length = 0;
double C = 0;
double R = 0;
double preC = 0;
double length_result[100];
double C_result[100];
double R_result[100];
double ne555_length = 0;
// 增益
double Av;

// 相位以及相位差
double phi;
double de_phi;

// 标志是电容还是电阻
uint8_t CorR;

// 用于标定的标准波长80m，对应10m线，对应角度90度
double lambda0;

// 校准函数
void direct(void);

// 波长转相位
int lambdatofre(double);

// 测长度
void Mea_Length(void);

// 测电容
void Mea_C(void);

// ne555测长度
void ne555_measure_L(void);

uint8_t ApproximatelyEqule(double num1, double num2);
int lambdatofre(double lambda);

uint8_t compare(double c1, double c2, double c3);
double average(double *num);
double myabs(double a);

int main(void)
{
    // 频率测量范围1Hz-1M，
    // 100k误差3Hz左右，1M误差10Hz左右,PA0方波输入
    SysTick_Config(SystemCoreClock);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Serial_Init();
    Timer_Init();
    exti_init();
    // TIM_PWMINPUT_Config();
    // Serial_Printf("Hi!");
    //  初始化
    Adc_Init();

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // PD port clock enable

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // Initialize PD0~12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // AD9959_Init();								//初始化控制AD9959需要用到的IO口,及寄存器
    // AD9959_Set_Fre(CH3, 2000000);	//设置通道3频率2M
    // AD9959_Set_Amp(CH3, 1023); 		//设置通道3幅度控制值1023，范围0~1023
    // AD9959_Set_Phase(CH3, 0);	//设置通道3相位控制值，4096(90度)，范围0~16383
    RELAY_CTR = 1;
    // while(1);
    // IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
    /*
    while(1){
        if(USART_RX_STA &0x1 == 0){//长度检测
            C_line = C;
            ne555_measure_L();
            Serial_Printf("长度值：%f\r\n",ne555_length);
        }else if(USART_RX_STA &0x1 == 1){
            Serial_Printf("电容值：%f\r\n",C-C_line;);
        }
    };*/
    Serial_Printf("va0.val=0");
    Serial_End();
    //while (1)
    //{
        //Serial_Printf("x2.val=%d", cntFre);
        //Serial_End();
		//Serial_Printf("%d\r\n",cntFre);
        //delay_ms(500);
    //}
    while (1)
    {
        if (flag == 3)
        {
            direct();
        }

        if (flag == 1) // 测量长度
        {
            RELAY_CTR = 1;
            // Serial_SendByte(0x11); // 使串口屏显示测量中
            // Serial_Printf("L:%f\r\n",ne555_length);
            // Mea_Length();
            Mea_C();
            C_line = C;
            ne555_measure_L();
            ne555_length -= 0.424;
            if (ne555_length < 6)
            {
                ne555_length -= 0.10;
            }
            if (ne555_length > 1)
			{
                ne555_length += 0.2;
			}
				else 
			{
				ne555_length += 0.02;
			}
			
            Serial_Printf("va1.val=0");
            Serial_End();
            Serial_Printf("x2.vvs1=4");
            Serial_End();
            Serial_Printf("x2.val=%d", (int)(ne555_length*(float)10000.0));
            Serial_End();
            Serial_Printf("x1.vvs1=1");
            Serial_End();
            //Serial_Printf("x1.val=%d", cntFre);
            //Serial_End();
			
			/*
            Serial_Printf("Fre:%f\r\n",cntFre);
            Serial_Printf("Length:%f\r\n",ne555_length);
            */
            // Serial_SendByte(0x22); // 使串口屏显示结果
            flag = 0;
        }
        if (flag == 2) // 测量负载
        {
            RELAY_CTR = 1;    // 测量电容
            if (cntFre > 100) // 说明是电容
            {
                Mea_C();
                /*
                if(ApproximatelyEqule(C,C_line)) // 说明空载
                {
                    Serial_Printf("va1.val=0");
                    Serial_End();
                    Serial_Printf("Fre:%f\r\n",cntFre);
                    Serial_Printf("Empty!\r\n");

                }
                */
                // else // 说明电容
                {
					
                    Serial_Printf("x0.val=0");
                    Serial_End();
                    Serial_Printf("va1.val=2");
                    Serial_End();
                    Serial_Printf("x1.vvs1=4");
                    Serial_End();
                    Serial_Printf("x1.val=%d", (int)((C - C_line) * 10000));
                    Serial_End();
					
                    
                    //Serial_Printf("Fre:%f\r\n",cntFre);
                    //Serial_Printf("C:%f\r\n",C-C_line);
                    
                }
            }
            else // 说明是电阻
            {
                // Serial_Printf("Fre:%f\r\n",cntFre);
                RELAY_CTR = 0; // 测量电阻
                delay_ms(100);
                // Av = v2/v1
                uint16_t a1, a2;
                a1 = Get_Adc_Average(ADC1, ADC_Channel_1, 100);
                a2 = Get_Adc_Average(ADC2, ADC_Channel_11, 100);
                
				/*
                Serial_Printf("Fre:%f\r\n",cntFre);
                Serial_Printf("ADC:%d,%d\r\n",a1,a2);
                //R = (float)99906.0*((float)1.0/(a1-a2))-(float)44.735;
                Serial_Printf("R:%f\r\n",R);
                R = (float)99906.0 * ((float)1.0 / (a1 - a2)) - (float)44.735;
				*/
				R = (float)359535.0 * ((float)1.0 / (a1 - a2)) - (float)114.3;
                Serial_Printf("x1.val=0");
                Serial_End();
                Serial_Printf("va1.val=1");
                Serial_End();
                Serial_Printf("x0.vvs1=4");
                Serial_End();
                Serial_Printf("x0.val=%d", (int)(R*10000));
                Serial_End();
				
                delay_ms(100);
            }
            // Serial_SendByte(0x11); // 使串口屏显示测量中

            /*
            Mea_C();
            Serial_Printf("n0.val=%d", CorR);

            if (CorR == 1)
                Serial_Printf("n1.val=%f", C);
            else if (CorR == 2)
                Serial_Printf("n1.val=%f", R);

            Serial_SendByte(0x33); // 使串口屏显示结果
            flag = 0;
            */
            flag = 0;
            RELAY_CTR = 1;
        }
    }
}
// 频率计算
void TIM2_IRQHandler(void)
{
    // DO NOT ADD ANY DELAY CODE!!!
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        if (cnt > 100000)
        {
            cnt = cnt + cnt / 100000 + 1;
        }
        cntFre = cnt;
		//Serial_Printf("%d\r\n",cnt);
        cnt = 0;
		//Serial_Printf("?\r\n");
    }
}

// 中断接收串口数据
void USART3_IRQHandler(void)
{
    uint8_t Res;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        Res = USART_ReceiveData(USART3); // 读取接收到的数据
        if (Res == 0x23)
            flag = 1;
        else if (Res == 0x45)
            flag = 2;
        else
            flag = 0;
    }
}

void direct()
{
    double alpha = GetPhs();
    k = ((double)20 / alpha) * (double)2.625 / ((double)3 * 100);
}

int lambdatofre(double lambda)
{
    int fre = k * 300000000 / lambda;
    return fre;
}

/*
void Mea_Length(void)
{
    // 10m 应该使得 2.625MHZ频率，波长为80m，相位为90度
    // 即是，插入后，测量角度，得到为alpha相位时
    // 不为90是波长不为80m导致的，所以，需要修正波长
    // lambda等于360 * 2 * 10（d）/alpha
    // lambda又等于 光速*系数/f
    // 由此可得系数，用于修正
    // 由此使得，产生的频率确实能够得到标准lambda


    // 给80m波长
    AD9959_Set_Fre(CH3,lambdatofre(80));
    de_phi = GetPhs();
    if(de_phi>90)	// 说明大于10m
    {
        // 给120m波长
        AD9959_Set_Fre(CH3,lambdatofre(120));
        de_phi = GetPhs();
        if(de_phi>90)	// 说明大于15m
        {
            for(int i=1; i<400; i++) // Q:80 + 0.01i最大是50M,没到80M,是不是要i<400 ?
            {
                AD9959_Set_Fre(CH3,lambdatofre(120 + 0.01 * i));
                if(ApproximatelyEqule(de_phi)==1)
                {
                    length = (120 + 0.01*i)/8.0;
                    break;
                }
            }
        }
        else if(de_phi<90)	//说明小于15m
        {
            for(int i=1; i<800; i++)
            {
                AD9959_Set_Fre(CH3,lambdatofre(120 - 0.01 * i));
                if(ApproximatelyEqule(de_phi)==1)
                {
                    length = (120 - 0.01*i)/8.0;
                    break;
                }
            }
        }
        else length = 15 ;
    }
    else if(phi<90)	// 小于10m 对应80M
    {
        // 给160m波长
        AD9959_Set_Fre(CH3,lambdatofre(40));
        de_phi = GetPhs();
        if(de_phi>90)	// 说明大于5m
        {
            for(int i=1; i<400; i++)
            {
                AD9959_Set_Fre(CH3,lambdatofre(40 + 0.01 * i));
                if(ApproximatelyEqule(de_phi)==1)
                {
                    length = (40 + 0.01*i)/8.0;
                    break;
                }
            }
        }
        else if(de_phi<90)	// 说明小于5m
        {
            for(int i=1; i<400; i++)
            {
                AD9959_Set_Fre(CH3,lambdatofre(40 - 0.01 * i));
                if(ApproximatelyEqule(de_phi)==1)
                {
                    length = (40 - 0.01*i)/8.0;
                    break;
                }
            }
        }
        else length = 5;
    }
    else length = 10;

}
*/

void ne555_measure_L()
{
    ne555_length = (double)0.01 * C - 0.193;
}

void Mea_C(void)
{
    // NE555计算
    C = 1000000000000 / (cntFre * (double)CONST_LN2 * (double)(CONST_R1 + (double)2.0 * CONST_R2)) - (double)CONST_C; // 得到pf电容
                                                                                                                      /*1/B36*1000/0.36*1.4427
                                                                                                                      AD9959_Set_Fre(CH3,100000);
                                                                                                                      double C1 = GetPhs();
                                                                                                                  
                                                                                                                      AD9959_Set_Fre(CH3,200000);
                                                                                                                      double C2 = GetPhs();
                                                                                                                  
                                                                                                                      AD9959_Set_Fre(CH3,300000);
                                                                                                                      double C3 = GetPhs();
                                                                                                                  
                                                                                                                      uint8_t flag_c = (C1,C2,C3);
                                                                                                                      if(flag_c == 1)
                                                                                                                      {
                                                                                                                          R =  R0 / GetMag() - Rp;
                                                                                                                      }
                                                                                                                      else
                                                                                                                      {
                                                                                                                          // excel 拟合？或者NE555
                                                                                                                          C = (double)0.0014*pow(E,(double)0.0775*GetPhs()) - length * Cp;
                                                                                                                      }
                                                                                                                      */
}

double myabs(double a)
{
    return (a > 0) ? a : (-a);
}

uint8_t compare(double c1, double c2, double c3)
{
    uint8_t same_flag;
    double de1 = c2 - c1;
    double de2 = c3 - c2;
    if ((myabs(de1) < (double)0.01) && (myabs(de2) < (double)0.01))
    {
        same_flag = 1;
    }
    else
    {
        same_flag = 0;
    }
    return same_flag;
}

double average(double *num)
{
    double sum;
    for (int i = 0; i < 100; i++)
    {
        sum += num[i] * (double)0.01;
    }
    return sum;
}

uint8_t ApproximatelyEqule(double num1, double num2)
{
    if (myabs(num1 - num2) < (double)MAX_DIFF)
        return 1;
    return 0;
}
