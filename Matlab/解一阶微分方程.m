%四阶Runge-Kutta法解常微分方程
%待求解方程 y'= y-2x/y (0<x<1), y(0) = 1; 设步长 h = 0.1;
clc,clear
set(0,'defaultfigurecolor','w')
x0 = 0; %自变量初值
xn = 1; %自变量终值
y0 = 1; %因变量初值
h = 0.1; %步长
[x,y] = RungeKutta4(x0, xn, y0, h); %自定义4阶龙格库塔法
[x2,y2] = ode45(@fun, [0,1], y0);   %matlab内部函数
plot(x,y,'r.')
hold on
plot(x2,y2,'b')



function [x,y] = RungeKutta4(x0, xn, y0, h)
n = (xn-x0)/h;
x = zeros(n+1,1);
y = zeros(n+1,1);
x(1) = x0;
y(1) = y0;
for i = 1:n
    x(i+1) = x(i)+h;
    K1 = fun(x(i), y(i));
    K2 = fun(x(i)+h/2, y(i)+K1*h/2);
    K3 = fun(x(i)+h/2, y(i)+K2*h/2);
    K4 = fun(x(i)+h, y(i)+K3*h);
    y(i+1) = y(i)+h/6*(K1+2*K2+2*K3+K4);
end
end

function f = fun(x,y)
f = y-2*x/y;
end
