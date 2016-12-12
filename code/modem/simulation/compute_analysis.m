#!/bin/octave
## Evan Widloski - 2015-03-16

%---------- Plot result against frequency and phase ---------------

t = linspace(0,8/sample_rate,8);
## phases = [0:90:360];
## phases = linspace(0,180,4);
phases = linspace(0,180,20);

freqs = linspace(500,3000,300);
results = [];
hold on
for phase = 2*pi*phases/360
    freq_result = [];
  for freq = freqs
      x = 64*sin(2*pi*freq*[0:7]/sample_rate - phase) + 0*randn(1,8);  %add some noise
      if sample_rate == 9600
        freq_result = [freq_result dtft(x(:,1:8))];
    endif
    if sample_rate = 19200
        freq_result = [freq_result dtft(x(:,1:8))];
    endif
  endfor
  results = [results;freq_result];

endfor
surf(freqs,phases,results)
xlabel('frequency')
ylabel('phase')
view(-90,180)

print -dpng freq_phase.png
