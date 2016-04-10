
cof1 = cof2 = cof3 = cof4 = int16(0)

// add most recent ADC reading to samples[]
samples[oldest_sample] = (int8_t)ADCH - 127;
oldest_sample = (oldest_sample + 1) % 8;

int16_t data = samples[oldest_sample] << 6;

cof1 = data;
cof2 = 0;
cof3 = data;
cof4 = 0;

data = samples[(oldest_sample + 1)%8];
cof1 += data;
cof2 += data;
cof4 += data;
data <<= 1;
cof4 += data;
data <<= 1;
cof1 += data;
cof2 += data;
cof4 += data;
data <<= 1;
cof1 += data;
cof2 += data;
cof3 += data;
cof4 += data;
data <<= 1;
cof4 += data;
data <<= 1;
cof1 += data;
cof2 += data;
cof4 += data;

data = samples[(oldest_sample + 2)%8];
cof4 += data;
data <<= 1;
cof3 -= data;
data <<= 1;
cof3 -= data;
data <<= 1;
cof3 -= data;
data <<= 1;
cof3 -= data;
cof4 += data;
data <<= 1;
cof3 -= data;
data <<= 1;
cof2 += data;

data = samples[(oldest_sample + 3)%8];
cof1 -= data;
cof2 += data;
cof4 -= data;
data <<= 1;
cof4 -= data;
data <<= 1;
cof1 -= data;
cof2 += data;
data <<= 1;
cof1 -= data;
cof2 += data;
cof3 -= data;
cof4 -= data;
data <<= 1;
cof3 -= data;
cof4 -= data;
data <<= 1;
cof1 -= data;
cof2 += data;
cof4 -= data;

data = samples[(oldest_sample + 4)%8];
cof3 += data;
data <<= 1;
cof3 += data;
data <<= 1;
cof3 += data;
data <<= 1;
data <<= 1;
cof3 += data;
data <<= 1;
cof3 += data;
cof4 -= data;
data <<= 1;
cof1 -= data;

data = samples[(oldest_sample + 5)%8];
cof1 -= data;
cof2 -= data;
cof3 += data;
cof4 += data;
data <<= 1;
cof3 += data;
cof4 += data;
data <<= 1;
cof1 -= data;
cof2 -= data;
cof3 += data;
data <<= 1;
cof1 -= data;
cof2 -= data;
data <<= 1;
cof4 += data;
data <<= 1;
cof1 -= data;
cof2 -= data;
cof3 += data;
cof4 += data;

data = samples[(oldest_sample + 6)%8];
cof3 -= data;
cof4 += data;
data <<= 1;
data <<= 1;
cof3 -= data;
cof4 += data;
data <<= 1;
cof3 -= data;
cof4 += data;
data <<= 1;
data <<= 1;
cof3 -= data;
cof4 += data;
data <<= 1;
cof2 -= data;

data = samples[(oldest_sample + 7)%8];
cof1 += data;
cof2 -= data;
cof3 -= data;
cof4 -= data;
data <<= 1;
cof3 -= data;
cof4 -= data;
data <<= 1;
cof1 += data;
cof2 -= data;
cof4 -= data;
data <<= 1;
cof1 += data;
cof2 -= data;
data <<= 1;
cof3 -= data;
data <<= 1;
cof1 += data;
cof2 -= data;
cof3 -= data;
cof4 -= data;

cof1 >>= 8;
cof2 >>= 8;
cof3 >>= 8;
cof4 >>= 8;


if (cof1 < 0){
cof1 *= -1;
}
if (cof2 < 0){
cof2 *= -1;
}
if (cof3 < 0){
cof3 *= -1;
}
if (cof4 < 0){
cof4 *= -1;
}

out = out + (cof3 + cof4 - cof1 - cof2 - out)>>2;

//2200Hz detected, set output high
if (out > 0){
PORTB |= 1<<PB4;
}

// 1200Hz detected, set output low
else if (out < 0){
PORTB &= ~(1<<PB4);
}
