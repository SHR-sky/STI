clc
clear
close all

% Step 3: 使用四阶Runge-Kutta方法进行迭代求解
t0 = 0; % 初始时间
y10 = 1; % 初始条件 for y1
y20 = 0; % 初始条件 for y2
h = 0.000001; % 步长
t_final = 0.25; % 最终时间

% 存储结果的数组
time = t0:h:t_final;
y1_result = zeros(size(time));
y2_result = zeros(size(time));

i = 1;

while t0 < t_final
    % 使用自定义的四阶Runge-Kutta函数进行计算
    [k1, k2, k3, k4] = runge_kutta_4(t0, y10, y20, h);

    % 更新未知函数的值
    y1_new = y10 + (1/6) * (k1(1) + 2*k2(1) + 2*k3(1) + k4(1));
    y2_new = y20 + (1/6) * (k1(2) + 2*k2(2) + 2*k3(2) + k4(2));

    % 更新y1和y2的值，进行下一次迭代
    y10 = y1_new;
    y20 = y2_new;

    % 存储结果
    y1_result(i) = y10;
    y2_result(i) = y20;

    % 更新时间
    t0 = t0 + h;
    i = i + 1;
end

% 最终结果
final_y1 = y10;
final_y2 = y20;

% 绘制结果图像
figure;
plot(time, y1_result, 'b', 'LineWidth', 1.5);
hold on;
plot(time, y2_result, 'r', 'LineWidth', 1.5);
xlabel('时间');
ylabel('函数值');
legend('y1', 'y2');
title('二阶常微分方程解的图像');
grid on;
hold off;


% Step 1: 定义四阶Runge-Kutta方法的函数

function [k1, k2, k3, k4] = runge_kutta_4(t, y1, y2, h)
    % 计算k1
    k1(1) = h * y2;
    k1(2) = h * my_function(t, y1, y2);

    % 计算k2
    k2(1) = h * (y2 + 0.5 * k1(2));
    k2(2) = h * my_function(t + 0.5 * h, y1 + 0.5 * k1(1), y2 + 0.5 * k1(2));

    % 计算k3
    k3(1) = h * (y2 + 0.5 * k2(2));
    k3(2) = h * my_function(t + 0.5 * h, y1 + 0.5 * k2(1), y2 + 0.5 * k2(2));

    % 计算k4
    k4(1) = h * (y2 + k3(2));
    k4(2) = h * my_function(t + h, y1 + k3(1), y2 + k3(2));
end


% Step 2: 定义二阶常微分方程
function dydt = my_function(t, y1, y2)
    % 定义二阶常微分方程
    dydt = (30000*sin(600*3.14*t) - 40000*y1) / 2;
end

