#ifndef Filter_H_
#define Filter_H_ 

#define Sliding_Mean_Filter_Buffer_Size 50

typedef struct
{
	double Filter_Buffer[Sliding_Mean_Filter_Buffer_Size];
	uint16_t id;
	double sum;
	double average;
}Sliding_Mean_Filter;

void Sliding_Mean_Filter_Calc(Sliding_Mean_Filter *filter, double Data);

extern Sliding_Mean_Filter Example_Filter;

#endif