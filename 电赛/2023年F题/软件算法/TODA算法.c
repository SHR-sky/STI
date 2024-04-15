// #include<stdio.h>
// #include<math.h>

// #define x1 0
// #define y1 0
// #define x2 0
// #define y2 40
// #define x3 40
// #define y3 0

// float s1,s2;    //input
// float xx,yy;    //output


// void get_time(float s1,float s2)    //get the signal difference of time
// {
//     scanf("%f",&s1);
//     scanf("%f",&s2);
// }

// void TODA(float s1,float s2)
// {
//     float p1,q1,p1_m,p1_d,p2,q2,p2_m,p2_d;
//     float k1,k2,k3;
//     float a,b,c,d;
//     float r1,r2;

//     k1=pow(x1,2)+pow(y1,2);
//     k2=pow(x2,2)+pow(y2,2);
    
//     p1_m=(y2-y1)*s2*s2-(y3-y1)*s1*s1+(y3-y1)*(k2-k1)-(y2-y1)*(k3-k1);
//     p1_d=(x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
//     p1=p1_m/(p1_d*2);
//     q1=((y2-y1)*s2-(y3-y1)*s1)/((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1));

//     p2_m=(x2-x1)*s2*s2-(x3-x1)*s1*s1+(x3-x1)*(k2-k1)-(x2-x1)*(k3-k1);
//     p2_d=(x3-x1)*(y2-y1)-(x2-x1)*(y3-y1);
//     p2=p2_m/(p2_d*2);
//     q2=((x2-x1)*s2-(x3-x1)*s1)/((x3-x1)*(y2-y1)-(x2-x1)*(y3-y1));

//     a=q1*q1+q2*q2-1;
//     b=-2*((x1-p1)*q1+(y1-p2)*q2);
//     c=(x1-p1)*(x1-p1)+(y1-p2)*(y1-p2);

//     d=b*b-4*a*c;
//     r1=(-b+sqrt(d))/(2*a);
//     r2=(-b-sqrt(d))/(2*a);

//     if(r1>0)
//     {
//         xx=p1+q1*r1;
//         yy=p2+q2*r1;
//     }
//     else
//     {
//         xx=p1+q1*r2;
//         yy=p2+q2*r2;
//     }
// }

// int main()
// {
//     get_time(s1,s2);
//     TODA(s1,s2);
//     return 0;
// }


#include<stdio.h>
#include<math.h>

// 长度单位为cm，时间单位为微秒

#define c1 0.26 // cm/us

double c=c1;     // cm/us
double t3;
double t4;
 
double x1, yy1, x2, y2, x3, y3;
double s1;
double s2,k1,k2,k3;
double p1_molecule, p1_denominator, p1, q1;
double p2_molecule, p2_denominator, p2, q2;
double a, b;
double discriminant;
double r1, r2;

// 时间点
double f_sec1, f_sec2, f_sec3;

// 结果
double xx,yy;

void get_time()
{
    printf("时间:\n");
    // 时间输入，单位为微秒
    scanf("%lf",&f_sec1);
    scanf("%lf",&f_sec2);
    scanf("%lf",&f_sec3);
}

void TDOA(void) 
{
    t3 = f_sec2-f_sec1;
    t4 = f_sec3-f_sec1;

    // 单位为cm，以sbw的中心为原点
    x1 = -19, yy1 = -19;
    x2 = 19, y2 = -19;
    x3 = 19, y3 = 19;
    
    s1 = c*t3;
    s2 = c*t4;
    k1 = x1 * x1 + yy1 * yy1;                                                                                                                                                                                
    k2 = x2 * x2 + y2 * y2;
    k3 = x3 * x3 + y3 * y3;
 
    p1_molecule = (y2 - yy1) * s2 * s2 - (y3 - yy1) * s1 * s1 + (y3 - yy1) * (k2 - k1) - (y2 - yy1) * (k3 - k1);
    p1_denominator = (x2 - x1) * (y3 - yy1) - (x3 - x1) * (y2 - yy1);
    p1 = p1_molecule / (p1_denominator * 2);
    q1 = ((y2 - yy1) * s2 - (y3 - yy1) * s1) / ((x2 - x1) * (y3 - yy1) - (x3 - x1) * (y2 - yy1));
 
    p2_molecule = (x2 - x1) * s2 * s2 - (x3 - x1) * s1 * s1 + (x3 - x1) * (k2 - k1) - (x2 - x1) * (k3 - k1);
    p2_denominator = (x3 - x1) * (y2 - yy1) - (x2 - x1) * (y3 - yy1);
    p2 = p2_molecule / (p2_denominator * 2);
    q2 = ((x2 - x1) * s2 - (x3 - x1) * s1) / ((x3 - x1) * (y2 - yy1) - (x2 - x1) * (y3 - yy1));
 
    a = q1 * q1 + q2 * q2 - 1;
    b = -2 * ((x1 - p1) * q1 + (yy1 - p2) * q2);
    c = (x1 - p1) * (x1 - p1) + (yy1 - p2) * (yy1 - p2);
 
 
    discriminant = b * b - 4 * a * c;
    r1 = (-b + sqrt(discriminant)) / (2 * a);
    r2 = (-b - sqrt(discriminant)) / (2 * a);
 
 
    if (r1 > 0) {
        xx = p1 + q1 * r1;
        yy = p2 + q2 * r1;
    }
    else {
        xx = p1 + q1 * r2;
        yy = p2 + q2 * r2;
    }
}


int main()
{
    get_time();
    TDOA();
    printf("xx:%lf, yy:%lf\n",xx,yy);
}