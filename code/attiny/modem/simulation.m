#!/bin/octave
% Evan Widloski - 2015-03-16
% adapted from https://sites.google.com/site/wayneholder/attiny-4-5-9-10-assembly-ide-and-programmer/bell-202-1200-baud-demodulator-in-an-attiny10

% testing digital filter against various frequencies and phases

function out = gen2(data)
    avg = bitshift(data(1) + data(2) + data(3) + data(4) + data(5) + data(6) + data(7) + data(8),-3);
    cof1 = cof2 = cof3 = cof4 = int8(0);

    src = int16(data(1)) - avg;
    src = bitshift(src,6);
    cof1 = src;
    cof2 = 0;
    cof3 = src;
    cof4 = 0;

    src = int16(data(2)) - avg;
    cof1 += src;
    cof2 += src;
    cof4 += src;
    src = bitshift(src,1);
    cof4 += src;
    src = bitshift(src,1);
    cof1 += src;
    cof2 += src;
    cof4 += src;
    src = bitshift(src,1);
    cof1 += src;
    cof2 += src;
    cof3 += src;
    cof4 += src;
    src = bitshift(src,1);
    cof4 += src;
    src = bitshift(src,1);
    cof1 += src;
    cof2 += src;
    cof4 += src;


    src = int16(data(3)) - avg;
    cof4 += src;
    src = bitshift(src,1);
    cof3 -= src;
    src = bitshift(src,1);
    cof3 -= src;
    src = bitshift(src,1);
    cof3 -= src;
    src = bitshift(src,1);
    cof3 -= src;
    cof4 += src;
    src = bitshift(src,1);
    cof3 -= src;
    src = bitshift(src,1);
    cof2 += src;

    src = int16(data(4)) - avg;
    cof1 -= src;
    cof2 += src;
    cof4 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof4 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 -= src;
    cof2 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 -= src;
    cof2 += src;
    cof3 -= src;
    cof4 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 -= src;
    cof4 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 -= src;
    cof2 += src;
    cof4 -= src;

    src = int16(data(5)) - avg;
    cof3 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 += src;
    ## src <<= 1;
    ## src <<= 1;
    src = bitshift(src,1);
    src = bitshift(src,1);
    cof3 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 += src;
    cof4 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 -= src;

    src = int16(data(6)) - avg;
    cof1 -= src;
    cof2 -= src;
    cof3 += src;
    cof4 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 += src;
    cof4 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 -= src;
    cof2 -= src;
    cof3 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 -= src;
    cof2 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof4 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 -= src;
    cof2 -= src;
    cof3 += src;
    cof4 += src;

    src = int16(data(7)) - avg;
    cof3 -= src;
    cof4 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 -= src;
    cof4 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 -= src;
    cof4 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 -= src;
    cof4 += src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof2 -= src;

    src = int16(data(8)) - avg;
    cof1 += src;
    cof2 -= src;
    cof3 -= src;
    cof4 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 -= src;
    cof4 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 += src;
    cof2 -= src;
    cof4 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 += src;
    cof2 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof3 -= src;
    ## src <<= 1;
    src = bitshift(src,1);
    cof1 += src;
    cof2 -= src;
    cof3 -= src;
    cof4 -= src;

    cof1 = bitshift(cof1,-8);
    cof2 = bitshift(cof2,-8);
    cof3 = bitshift(cof3,-8);
    cof4 = bitshift(cof4,-8);

    if cof1 <= 0
      cof1 *= -1;
    endif
    if cof2 <= 0
      cof2 *= -1;
    endif
    if cof3 <= 0
      cof3 *= -1;
    endif
    if cof4 <= 0
      cof4 *= -1;
    endif

    out = cof3 + cof4 - cof1 - cof2;
endfunction

% sample rate
sample_rate = 9600;
% input amplitude
amplitude = 16;
shift = 127;


%---------- Plot result against frequency and phase ---------------
## t = linspace(0,8/sample_rate,8);
## phases = [0:30:360];
## tau = phases./(2*pi);

## freqs = 500:100:3000;
## reses = [];
## for freq = freqs

##   res = [];
##   for s = tau
##     x = round(amplitude*sin(2*pi*freq*(t - s))) + shift; 
##     ## x = round(amplitude*sin(2*pi*freq*(t - s)) + 0*randn(1,8) + 40);  %add some noise
##     res = [res gen2(x(:,1:8))];
##   endfor
##   reses = [reses; res];

## endfor
## mesh(phases,freqs,double(reses))
## xlabel('phase')
## ylabel('frequency')
## view(-90,180)

%---------- Plot result against time ---------------

samples = 192*2;
time = linspace(0,samples/sample_rate,samples);
n = 1:samples;
fsk_rate = 600;
mark = round(amplitude*sin(2*pi*2200*time)) + shift;
space = round(amplitude*sin(2*pi*1200*time)) + shift;

samples_per_bit = sample_rate/fsk_rate;
signal = (mod(n,2*samples_per_bit) >= samples_per_bit) .* mark ...
         + (mod(n,2*samples_per_bit) < samples_per_bit) .* space;

result = [];
for i = 1:samples - 7
    result = [result gen2(signal(:,i:i + 7))];
endfor

for i = 4:length(result)
  result(i) = bitshift(result(i) + result(i-1) + result(i-2) + result(i-3),-2);
endfor


hold on

subplot(2,1,1)
plot(time,signal)
xlabel('time')
axis([time(1) time(end)])

subplot(2,1,2)
plot(time(1:samples - 7),result)
xlabel('time')
axis([time(1) time(end)])
