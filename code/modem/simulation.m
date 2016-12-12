#!/bin/octave
## Evan Widloski - 2015-03-16
## adapted from https://sites.google.com/site/wayneholder/attiny-4-5-9-10-assembly-ide-and-programmer/bell-202-1200-baud-demodulator-in-an-attiny10

## testing digital filter against various frequencies and phases

## sample rate
sample_rate = 9600;
## input amplitude
amplitude = 16;
shift = 127;


%---------- Plot result against frequency and phase ---------------
t = linspace(0,8/sample_rate,8);
phases = [0:30:360];
tau = phases./(2*pi);

freqs = 500:10:3000;
reses = [];
for freq = freqs

  res = [];
  for s = tau
    x = round(amplitude*sin(2*pi*freq*(t - s))) + shift; 
    ## x = round(amplitude*sin(2*pi*freq*(t - s)) + 0*randn(1,8) + 40);  %add some noise
    res = [res compute(x(:,1:8))];
  endfor
  ## plot([freq],res)
  reses = [reses; res];

endfor

surf(phases,freqs,double(reses))
zlabel('int value')
ylabel('frequency')
view(-90,180)
print -dpng "-S1200,300" freq_phase.png

%---------- Synthesize FSK signal -------------------
samples = 192*2;
t = linspace(0,samples/sample_rate,samples);
n = 1:samples;
fsk_rate = 550;
mark = round(amplitude*sin(2*pi*2200*t)) + shift;
space = round(amplitude*sin(2*pi*1200*t)) + shift;

samples_per_bit = sample_rate/fsk_rate;
signal = (mod(n,2*samples_per_bit) >= samples_per_bit) .* mark ...
         + (mod(n,2*samples_per_bit) < samples_per_bit) .* space;

plot(t,signal)
title('FSK Modulation')
xlabel('time')
ylabel('int value')
axis([t(1) t(end)])
print -dpng "-S1200,300" fsk_modulation.png

%---------- Plot result against time ---------------
result = [];
for i = 1:samples - 7
  result = [result compute(signal(:,i:i + 7))];
endfor

plot(t(1:samples - 7),result)
title('Demodulated')
xlabel('time')
ylabel('int value')
axis([t(1) t(end)])
print -dpng "-S1200,300" demod.png

%--------- test effects of lowpass (n sample rolling average) ---------------
for i = 4:length(result)
  result(i) = bitshift(result(i) + result(i-1) + result(i-2) + result(i-3),-2);
endfor


plot(t(1:samples - 7),result)
title('Demodulated with LPF')
xlabel('time')
ylabel('int value')
axis([t(1) t(end)])
print -dpng "-S1200,300" demod_lpf.png
