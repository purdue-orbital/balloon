#!/bin/octave
% filter for bell202 modem


%1sec sample
samples = 10000;
total_time = 1;
t = linspace(0,total_time,samples);
delta_t = total_time/samples
sample_rate = 1/delta_t

filter = sin(2*pi*1000.*t)./(pi.*t);

sum = sin(2*pi*1e3*t) + sin(2*pi*1.2e3*t);

subplot(4,1,1)
## plot(([1:samples]*sample_rate)./samples,fft(sum));
plot(t,sum)
axis([0 .02])

subplot(4,1,2)
f = linspace(-sample_rate,sample_rate,samples);
fft_sum = abs(fft(sum));
fft_sum = fft_sum./max(fft_sum);
plot(f,fft_sum');
axis([0 2e3])

subplot(4,1,3)
fftplot(filter)
## axis([0 2e3])

subplot(4,1,4)
sum_lpf = fftconv(sum,filter);
sum_lpf = sum_lpf(:,ceil((length(sum_lpf) - sample_rate*total_time)/2):floor((length(sum_lpf) + sample_rate*total_time)/2));
plot(t,sum_lpf)
axis([0 0.02])
pause
