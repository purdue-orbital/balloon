#!/bin/octave
## Evan Widloski - 2015-03-16
## adapted from https://sites.google.com/site/wayneholder/attiny-4-5-9-10-assembly-ide-and-programmer/bell-202-1200-baud-demodulator-in-an-attiny10

## testing digital filter against various frequencies and phases

sample_rate = 9600;
amplitude = 127;
shift = 0;

%---------- Synthesize FSK signal -------------------
fsk_rate = 600;
samples_per_bit = sample_rate/(fsk_rate*2);
## plot 10 periods
samples = 20*samples_per_bit;

t = linspace(0,samples/sample_rate,samples);
n = 1:samples;
unmodulated_signal = (mod(n,2*samples_per_bit) >= samples_per_bit) * 1 ...
                     + (mod(n,2*samples_per_bit) < samples_per_bit) * 0;

subplot(5,1,1)
plot(t,unmodulated_signal,'r')
title('Input data')
xlabel('time')
axis([t(1) t(end) -.2 1.2])

## incoherent synthesis
## mark = amplitude*sin(2*pi*1200*t) + shift;
## space = amplitude*sin(2*pi*2200*t) + shift;
## signal = (mod(n,2*samples_per_bit) >= samples_per_bit) .* mark ...
##          + (mod(n,2*samples_per_bit) < samples_per_bit) .* space;

## coherent synthesis
mark = ones(1,samples_per_bit)*(1200/sample_rate);
space = ones(1,samples_per_bit)*(2200/sample_rate);
signal = amplitude * sin(2*pi*cumsum([space mark space mark space mark space mark space mark space mark space mark space mark space mark space mark]));

## subplot(5,1,2)
subplot(2,1,1)
plot(t,signal)

## add noise
noise = .20 * amplitude * 2*(rand(1,length(signal)) - .5);
signal = signal + noise;

hold on
plot(t,noise,'r')
title('FSK Modulation')
xlabel('time')
axis([t(1) t(end)])


## %---------- Plot result against time ---------------
result = [];
for n = 1:length(signal)-7
    result = [result compute_cpp(signal(n:n+7),sample_rate)];
endfor
## result = filter([1 .75],[1], result);
result = filter([1 -1/4 * ones(1,4)],1,result);


## subplot(5,1,3)
subplot(2,1,1)
plot(t(1:samples - 7),result)
title('Demodulated')
xlabel('time')
axis([t(1) t(end)])

## %--------- test effects of lowpass (n sample rolling average) ---------------
## for i = 4:length(result)
##   result(i) = bitshift(result(i) + result(i-1) + result(i-2) + result(i-3),-2);
## endfor
pkg load signal
b=(fir1(8,[900 2500]/4800,"pass"));
b = (sign(b).*2.^round(log(abs(b))./log(2)));

## subplot(5,1,4)
## plot(t(1:samples - 7),result)
## title('Demodulated with filtering')
## xlabel('time')
## axis([t(1) t(end)])

## subplot(5,1,5)
subplot(2,1,2)
plot(t(1:samples - 7),(result > 1000),'g')
title('Thresholded output')
xlabel('time')
axis([t(1) t(end) -.2 1.2])


## ----- Spectrogram Plots of FSK signal -----
## window=8;
## [xx yy] = meshgrid(1:length(signal) - window + 1,linspace(-sample_rate/2,sample_rate/2,512));

## ## no filter
## result = abs(fftshift(fft(signal(hankel(1:window,window:length(signal))),512),1));
## subplot(1,1,1)
## imagesc(xx,yy,result)
## title('Spectrogram')



