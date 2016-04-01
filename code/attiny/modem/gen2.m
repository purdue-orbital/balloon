#!/bin/octave
% Evan Widloski - 2015-03-16
% adapted from https://sites.google.com/site/wayneholder/attiny-4-5-9-10-assembly-ide-and-programmer/bell-202-1200-baud-demodulator-in-an-attiny10

% testing digital filter against various frequencies and phases

function out = gen2(data)
    cof1 = [ 64,  45,   0, -45, -64, -45,   0,  45];
    cof2 = [  0,  45,  64,  45,   0, -45, -64, -45];
    cof3 = [ 64,   8, -62, -24,  55,  39, -45, -51];
    cof4 = [  0,  63,  17, -59, -32,  51,  45, -39];

    out1 = out2 = out3 = out4 = 0;

    for i = 1:8
      out1 += data(i) * cof1(i);
      out2 += data(i) * cof2(i);
      out3 += data(i) * cof3(i);
      out4 += data(i) * cof4(i);
    endfor


    out1 = bitshift(out1,-8);
    out2 = bitshift(out2,-8);
    out3 = bitshift(out3,-8);
    out4 = bitshift(out4,-8);

    if out1 <= 0
      out1 *= -1;
    endif
    if out2 <= 0
      out2 *= -1;
    endif
    if out3 <= 0
      out3 *= -1;
    endif
    if out4 <= 0
      out4 *= -1;
    endif

    out = out3 + out4 - out1 - out2;
endfunction

src_rate = 9600;
t = linspace(0,8/src_rate,8);
phases = [0:30:360];
tau = phases./(2*pi);

% Plot result against frequency and phase

fd = fopen('/tmp/out.mat','wt');
freqs = 200:5:3000;
reses = [];
for freq = freqs

  res = [];
  for s = tau
    x = round(32*sin(2*pi*freq*(t - s))); 
    ## x = round(32*sin(2*pi*freq*(t - s)) + 0*randn(1,8) + 40);  %add some noise
    fprintf(fd,'%d,',x)
    fprintf(fd,'\n')
    res = [res gen2(x(:,1:8))];
  endfor
  reses = [reses; res];

endfor
mesh(phases,freqs,reses)
xlabel('phase')
ylabel('frequency')
view(-90,180)
pause
