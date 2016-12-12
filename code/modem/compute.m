#!/bin/octave
% Evan Widloski - 2015-03-16
% adapted from https://sites.google.com/site/wayneholder/attiny-4-5-9-10-assembly-ide-and-programmer/bell-202-1200-baud-demodulator-in-an-attiny10

% testing digital filter against various frequencies and phases

function out = compute(data)
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

