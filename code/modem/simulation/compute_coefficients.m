#!/bin/octave
## Evan Widloski - 2016-12-12
## Calculate DTFT coefficients and convert to Q number

## sample length
N = 8

## generate DFT coefficients
Xd1 = e.^(-i*2*pi*(1200/9600)*[0:N-1]);
Xd2 = e.^(-i*2*pi*(2200/9600)*[0:N-1]);

## scale down coefficients by 1 bit since `1` can't be expressed as binary fraction
## e.g. `Xd1 = Xd1 * 127/128` for N = 8
Xd1 = Xd1*((2^(N-1) - 1)/2^(N-1));
Xd2 = Xd2*((2^(N-1) - 1)/2^(N-1));

## express coefficients as signed N-bit binary fractions
arrayfun(@(x) dec2binfrac(x,8),real(Xd1))
arrayfun(@(x) dec2binfrac(x,8),imag(Xd1))
arrayfun(@(x) dec2binfrac(x,8),real(Xd2))
arrayfun(@(x) dec2binfrac(x,8),imag(Xd2))

