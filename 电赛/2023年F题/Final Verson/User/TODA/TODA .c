#include<stdio.h>
#include<math.h>
#include "Serial.h"

#define x1 21
#define y1 21
#define x2 -21
#define y2 21
#define x3 -21
#define y3 -21

//float s1,s2;    //input
//float x_AB,y_12;    //output
//int x_AB[2]; // AB~KL
//int y_12[2]; // 12~1112


void TODA(float s1,float s2, int x_AB[], int y_12[])
{
    float p1,q1,p1_m,p1_d,p2,q2,p2_m,p2_d;
    float k1,k2,k3;
    float a,b,c,d;
    float r1,r2;
	float xx,yy;
    k1 = pow(x1, 2) + pow(y1, 2);
    k2 = pow(x2, 2) + pow(y2, 2);
	k3 = pow(x3, 2) + pow(y3, 2);
    
    p1_m=(y2-y1)*s2*s2-(y3-y1)*s1*s1+(y3-y1)*(k2-k1)-(y2-y1)*(k3-k1);
    p1_d=(x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
    p1=p1_m/(p1_d*2);
    q1=((y2-y1)*s2-(y3-y1)*s1)/((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1));

    p2_m=(x2-x1)*s2*s2-(x3-x1)*s1*s1+(x3-x1)*(k2-k1)-(x2-x1)*(k3-k1);
    p2_d=(x3-x1)*(y2-y1)-(x2-x1)*(y3-y1);
    p2=p2_m/(p2_d*2);
    q2=((x2-x1)*s2-(x3-x1)*s1)/((x3-x1)*(y2-y1)-(x2-x1)*(y3-y1));

    a=q1*q1+q2*q2-1;
    b=-2*((x1-p1)*q1+(y1-p2)*q2);
    c=(x1-p1)*(x1-p1)+(y1-p2)*(y1-p2);

    d=b*b-4*a*c;
    r1=(-b+sqrt(d))/(2*a);
    r2=(-b-sqrt(d))/(2*a);

    if(r1>0)
    {
        xx=p1+q1*r1;
        yy=p2+q2*r1;
    }
    else
    {
        xx=p1+q1*r2;
        yy=p2+q2*r2;
    }
	xx*=100;
	yy*=100;
		
		//映射
		int x;//计数
    int y;
    // 初值设定
    float x_low = -15;
    float x_high = -10;
    float y_low = 10;
    float y_high = 15;
    x_AB[0] = 'A';
    x_AB[1] = 'B';
    y_12[0] = 1;
    y_12[1] = 2;
    // 边界区域
    float x_min = -15;
    float x_max = 15;
    float y_min = -15;
    float y_max = 15;
    // 定位x
    for (x = 0; x < 6; x++) {
        if (xx < x_min || xx >= x_max) {
            x = -1;
            break;
        }
        else if (xx < x_high && xx >= x_low)
            break;
        else {
            x_low += 5;
            x_high += 5;
        }
    }
    // 定位y
    for (y = 0; y < 6; y++) {
        if (yy < y_min || yy >= y_max) {
            y = -1;
            break;
        }
        else if (yy < y_high && yy >= y_low)
            break;
        else {
            y_low -= 5;
            y_high -= 5;
        }
    }
    // 转换板子位置
    if (x == -1 || y == -1) {//超出坐标范围，则为0
        x_AB[0] = '*';
        x_AB[1] = '*';
        y_12[0] = '*';
        y_12[1] = '*';
    } else {
        x_AB[0] += 2 * x;
        x_AB[1] += 2 * x;
        y_12[0] += 2 * y;
        y_12[1] += 2 * y;
    }
	//Serial_Printf("----   %f %f    ----",xx,yy);	
	Serial_Printf("va0.val=%d",((x_AB[0]-'A'+2)/2)*30);
	Serial_End();
	Serial_Printf("va1.val=%d",((y_12[0]+1)/2)*30);
	Serial_End();
	Serial_Printf("va2.val=1");
	Serial_End();
}




