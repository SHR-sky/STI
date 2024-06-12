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

// // ���ȵ�λΪcm��ʱ�䵥λΪ΢��

// #define c1 0.26 // cm/us

// double c=c1;     // cm/us
// double t3;
// double t4;
 
// double x1, yy1, x2, y2, x3, y3;
// double s1;
// double s2,k1,k2,k3;
// double p1_molecule, p1_denominator, p1, q1;
// double p2_molecule, p2_denominator, p2, q2;
// double a, b;
// double discriminant;
// double r1, r2;

// // ʱ���
// double f_sec1, f_sec2, f_sec3;

// // ���
// double xx,yy;

// void get_time()
// {
//     printf("ʱ��:\n");
//     // ʱ�����룬��λΪ΢��
//     scanf("%lf",&f_sec1);
//     scanf("%lf",&f_sec2);
//     scanf("%lf",&f_sec3);
// }

// void TDOA(void) 
// {
//     t3 = f_sec2-f_sec1;
//     t4 = f_sec3-f_sec1;

//     // ��λΪcm����sbw������Ϊԭ��
//     x1 = -19, yy1 = -19;
//     x2 = 19, y2 = -19;
//     x3 = 19, y3 = 19;
    
//     s1 = c*t3;
//     s2 = c*t4;
//     k1 = x1 * x1 + yy1 * yy1;                                                                                                                                                                                
//     k2 = x2 * x2 + y2 * y2;
//     k3 = x3 * x3 + y3 * y3;
 
//     p1_molecule = (y2 - yy1) * s2 * s2 - (y3 - yy1) * s1 * s1 + (y3 - yy1) * (k2 - k1) - (y2 - yy1) * (k3 - k1);
//     p1_denominator = (x2 - x1) * (y3 - yy1) - (x3 - x1) * (y2 - yy1);
//     p1 = p1_molecule / (p1_denominator * 2);
//     q1 = ((y2 - yy1) * s2 - (y3 - yy1) * s1) / ((x2 - x1) * (y3 - yy1) - (x3 - x1) * (y2 - yy1));
 
//     p2_molecule = (x2 - x1) * s2 * s2 - (x3 - x1) * s1 * s1 + (x3 - x1) * (k2 - k1) - (x2 - x1) * (k3 - k1);
//     p2_denominator = (x3 - x1) * (y2 - yy1) - (x2 - x1) * (y3 - yy1);
//     p2 = p2_molecule / (p2_denominator * 2);
//     q2 = ((x2 - x1) * s2 - (x3 - x1) * s1) / ((x3 - x1) * (y2 - yy1) - (x2 - x1) * (y3 - yy1));
 
//     a = q1 * q1 + q2 * q2 - 1;
//     b = -2 * ((x1 - p1) * q1 + (yy1 - p2) * q2);
//     c = (x1 - p1) * (x1 - p1) + (yy1 - p2) * (yy1 - p2);
 
 
//     discriminant = b * b - 4 * a * c;
//     r1 = (-b + sqrt(discriminant)) / (2 * a);
//     r2 = (-b - sqrt(discriminant)) / (2 * a);
 
 
//     if (r1 > 0) {
//         xx = p1 + q1 * r1;
//         yy = p2 + q2 * r1;
//     }
//     else {
//         xx = p1 + q1 * r2;
//         yy = p2 + q2 * r2;
//     }
// }


// int main()
// {
//     get_time();
//     TDOA();
//     printf("xx:%lf, yy:%lf\n",xx,yy);
// }


typedef struct Point
{
    float x;
    float y;
} Point;


#define V_BOARD 0.1



static Point TDOA(Point receiver1, Point receiver2, Point receiver3, float tdoa1, float tdoa2, float tdoa3)
{

    float distance1 = V_BOARD * tdoa1;
    float distance2 = V_BOARD * tdoa2;
    float distance3 = V_BOARD * tdoa3;
	float a_watch = tdoa2 - tdoa1;
	float b_watch = tdoa3 - tdoa1;

    float R21 = distance2 - distance1;
    float R31 = distance3 - distance1;
    float g = ((R31*receiver2.x)/R21 - receiver3.x)/receiver3.y;
    float h = (receiver3.x*receiver3.x+receiver3.y*receiver3.y-R31*R31+R31*R21*(1-(receiver2.x*receiver2.x/(R21*R21))))/(2*receiver3.y);
    float d = -((1-(receiver2.x/R21)*(receiver2.x/R21))+g*g);
    float e = receiver2.x*(1-(receiver2.x/R21)*(receiver2.x/R21))-2*g*h;
    float f = (R21*R21/4)*(1-(receiver2.x/R21)*(receiver2.x/R21))*(1-(receiver2.x/R21)*(receiver2.x/R21))-h*h;
    float x1 = (-e-sqrtf(e*e-4*d*f))/(2*d);
	float watch = e*e-4*d*f;
    float y1 = g*x1+h;
    float x2 = (-e+sqrtf(e*e-4*d*f))/(2*d);
    float y2 = g*x2+h;
    Point source;
    float t1 = (R21*R21-receiver2.x*receiver2.x+2*receiver2.x*x1+2*R21*sqrtf(x1*x1+y1*y1));
    float t2 = (R21*R21-receiver2.x*receiver2.x+2*receiver2.x*x2+2*R21*sqrtf(x2*x2+y2*y2));
    if(t1 < 0) t1 = -t1;
    if(t2 < 0) t2 = -t2;
    if(t1 < t2){
        source.x = x1;
        source.y = y1;
    }
    else{
        source.x = x2;
        source.y = y2;
    }
    return source;
}



int main()
{
    float t1,t2,t3;
    Point z1,z2,z3;
    z1.x = -21;
    z1.y = 21;

    z2.x = 21;
    z2.y = 21;

    z3.x = -21;
    z3.y = -21;


    Point xxyy = TDOA(z1,z2,z3,t1,t2,t3);

    printf("xx:%lf, yy:%lf\n",xxyy.x,xxyy.y);
}