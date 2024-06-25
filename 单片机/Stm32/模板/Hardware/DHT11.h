#ifndef _DHT11__H_
#define _DHT11__H_
#include "sys.h"                // Device header


void Data_Out_Init(void);
void Data_In_Init(void);
void tDHT11_start(void);
uint8_t tDHT11_rec_byte(void);
void tDHT11_rec_data(uint8_t *temp,uint8_t *dec) ;//温度，湿度

#endif
