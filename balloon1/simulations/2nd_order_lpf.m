#!/bin/octave
% Calculating transfer function for second order lowpass

L = 10e-8;
C = 1e-9
w = 433e6;
s = i*w;

x = [-1/(s*L) (2/(s*L) + C*s) -1/(s*L); (C*s + 1/50) -1/(s*L) 0];
-rref(x)(2,3)
abs(1/((1-ans)/(s*L)))
