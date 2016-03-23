#!/bin/octave
% Evan Widloski - 2015-03-16
% adapted from https://sites.google.com/site/wayneholder/attiny-4-5-9-10-assembly-ide-and-programmer/bell-202-1200-baud-demodulator-in-an-attiny10
% testing digital filters

function out = gen(data)

    cof1 = cof2 = cof3 = cof4 = int8(0);

    src = int16(data(1));
    src = bitshift(src,6);
    cof1 = src;
    cof2 = 0;
    cof3 = src;
    cof4 = 0;

    src = int16(data(2));
    cof1 += src;
    cof2 += src;
    cof4 += src;
    src
    cof1
    cof2
    cof3
    cof4
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


    src = int16(data(3));
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

    src = int16(data(4));
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

    src = int16(data(5));
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

    src = int16(data(6));
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

    src = int16(data(7));
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

    src = int16(data(8));
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
    if cof1 <= 0
      cof1 *= -1;
    endif
    if cof2 <= 0
      cof1 *= -1;
    endif
    if cof3 <= 0
      cof1 *= -1;
    endif
    if cof4 <= 0
      cof1 *= -1;
    endif

    out = int16(cof3) + int16(cof4) - int16(cof1) - int16(cof2);
    ## out = bitshift(cof3, -8) * bitshift(cof3, -8) + bitshift(cof4, -8) * bitshift(cof4, -8) - bitshift(cof1, -8) * bitshift(cof1, -8) - bitshift(cof2, -8) * bitshift(cof2, -8);
endfunction

src_rate = 9600;
t = linspace(0,8/src_rate,8)
tau = linspace(0,8/src_rate,8)

% Plot result against frequency


fd = fopen('/tmp/out.mat','wt');
freqs = 1200:100:5200;
## freqs = [2200];
res = [];
for s = tau
  s
  ## x = round(32*sin(2*pi*freq*t) .+ 5*randn(1,8));
  x = round(32*sin(2*pi*2200*(t - s)) + 127)
  fprintf(fd,'%d,',x)
  fprintf(fd,'\n')
  res = [res gen(x(:,1:8))];
endfor
res
fclose(fd)
plot(tau,res)
xlabel('freq')
pause

% Plot result against phase

## freq = 2200;
## phases = 0:2*pi/10:2*pi;
## for phase = phases
## x = round(32*sin(2*pi*freq*t + phase));
## res = [res gen(x(:,1:8))];
## endfor
## plot(phases,res)
xlabel('phase')
## pause

## for rand = 1:8
## x = 32*randn(1,8)
## res = [res gen(x(:,1:8))];
## endfor
## plot(res)
## pause
