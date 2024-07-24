% 生成1 kHz正弦波
frequency = 10000; % 正弦波频率10 kHz
sampling_rate = 1000000; % 采样频率1 MHz
num_samples = 1000; % 采集1000个点

% 生成时间轴
t = (0:num_samples-1) * (1/sampling_rate);

% 生成正弦波信号
amplitude = 1; % 设置正弦波的幅度

% sawtooth(2 * pi * frequency * t,0.5) 为三角波
sin_wave = amplitude * (0.5*sin(2 * pi * frequency * t)+0.5*sin(2 * pi * 25000 * t));

% 绘制正弦波信号
figure;
plot(t, sin_wave);
title('Sinusoidal Wave');
xlabel('Time (s)');
ylabel('Amplitude');

sin_wave = round(sin_wave*2048)+2048; 

grid on;