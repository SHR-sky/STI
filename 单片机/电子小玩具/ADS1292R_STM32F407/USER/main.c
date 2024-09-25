#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "ads1292r.h"
#include "dma.h"

extern uint8_t ADS1292R_data_buf[9]; //ADS1292 data buffer
extern uint8_t ADS1292R_receive_flag;
extern u16 counter;

uint32_t channel[2] = {0}; //ADS1292R dual channel data

s32 get_volt(u32 num);

int main(void)
{
    u8 i, sum;
    u8 data_to_send[60];//���ڷ��ͻ���
    u32 channel[2]; // ��CH1 CH2����
    s32	p_Temp[2];	//���ݻ���

    data_to_send[0]=0xAA;
    data_to_send[1]=0xAA;
    data_to_send[2]=0xF1;
    data_to_send[3]=0x08;

    SystemInit();
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
    delay_init(168);     //��ʼ����ʱ����
    delay_ms(100);
    uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
    MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)data_to_send,60);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��Ϊdata_to_send,����Ϊ:SEND_BUF_SIZE.
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����
    LED_Init();					//��ʼ��LED
    KEY_Init(); 				//������ʼ��

    ADS1292R_Init();
    ADS1292R_ADCStartNormal();
    // printf("ADS1292R READY!\r\n");

    EXTI->IMR |= EXTI_Line8;//��DRDY�ж�
    while(1)
    {
        if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ�DMA2_Steam7�������
        {
            DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//���DMA2_Steam7������ɱ�־
        }
        if(ADS1292R_receive_flag) {
            // printf("ok");
            channel[0] = ADS1292R_data_buf[3]<<16 | ADS1292R_data_buf[4]<<8 | ADS1292R_data_buf[5];	// RESP����
            channel[1] = ADS1292R_data_buf[3]<<16 | ADS1292R_data_buf[7]<<8 | ADS1292R_data_buf[8];	// ECG����

            p_Temp[0] = get_volt(channel[0]);	//�Ѳɵ���3���ֽ�ת���з���32λ��
            p_Temp[1] = get_volt(channel[1]);	//�Ѳɵ���3���ֽ�ת���з���32λ��

            //�з�����Ϊ��תΪ�޷��ţ��޷�����Ϊ�߼�����
            channel[0] = p_Temp[0];
            channel[1] = p_Temp[1];
            data_to_send[4]=channel[0]>>24;		//25-32λ
            data_to_send[5]=channel[0]>>16;  	//17-24
            data_to_send[6]=channel[0]>>8;		//9-16
            data_to_send[7]=channel[0]; 			//1-8

            data_to_send[8]=channel[1]>>24;		//25-32λ
            data_to_send[9]=channel[1]>>16;  	//17-24
            data_to_send[10]=channel[1]>>8;		//9-16
            data_to_send[11]=channel[1];			 //1-8

            for(i=0; i<12; i++)
                sum += data_to_send[i];
            data_to_send[12] = sum;
            MYDMA_Enable(DMA2_Stream7,13);     //��ʼһ��DMA����

            ADS1292R_receive_flag = 0;
            sum=0;

            //printf("%08X %08X, ", channel[0], channel[1]);
            //printf("%d %d, ", resp, ecg);
            //printf("%d %d\r\n", channel[0], channel[1]);
        }
//		key=KEY_Scan(0);
//		if(key==KEY0_PRES)//KEY0����
//		{
// 			printf("0:%d, 1:%d", channel[0], channel[1]);
//		}
        /*i++;
        delay_ms(10);
        if(i==20)
        {
        	LED0=!LED0;//��ʾϵͳ��������
        	i=0;
        }		  */
    }
}

s32 get_volt(u32 num)
{
    s32 temp;
    temp = num;
    temp <<= 8;
    temp >>= 8;
    return temp;
}
