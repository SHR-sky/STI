#include<stdio.h>
#include<math.h>

#define x1 0
#define y1 0
#define x2 0
#define y2 40
#define x3 40
#define y3 0

float s1,s2;    //input
float xx,yy;    //output


void get_time(float s1,float s2)    //get the signal difference of time
{
    scanf("%f",&s1);
    scanf("%f",&s2);
}

void TODA(float s1,float s2)
{
    float p1,q1,p1_m,p1_d,p2,q2,p2_m,p2_d;
    float k1,k2,k3;
    float a,b,c,d;
    float r1,r2;

    k1=pow(x1,2)+pow(y1,2);
    k2=pow(x2,2)+pow(y2,2);
    
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
}

int main()
{
    get_time(s1,s2);
    TODA(s1,s2);
    return 0;
}
