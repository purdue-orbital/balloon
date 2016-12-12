// Evan Widloski - FSK Computation
// Intended to be called from Octave
#include <octave/oct.h>
#include <stdint.h>
#include <math.h>

// Input: 8 element row-vector containing time samples, 9600 samp/s
// Output: 1 element. DTFT(2200) - DTFT(1200)

DEFUN_DLD(dtft, args, , "FSK"){
  // get input vector from args
  int8NDArray arg = args(0).vector_value();
  int8_t in[8] = {arg(0), arg(1), arg(2), arg(3), arg(4), arg(5), arg(6), arg(7)};
  printf("input:%d,%d,%d,%d,%d,%d,%d,%d\n",in[0],in[1],in[2],in[3],in[4],in[5],in[6],in[7]);

  // define coefficient vectors
  int8_t coeff_1200_real[8] = {127,   90,    0,  -90, -127,  -90,    0,   90};
  int8_t coeff_1200_imag[8] = {  0,  -90, -127,  -90,    0,   90,  127,   90};
  int8_t coeff_2200_real[8] = {127,   16, -123,  -49,  110,   77,  -90, -101};
  int8_t coeff_2200_imag[8] = {  0, -126,  -33,  117,   63, -101,  -90,   77};

  int16_t dtft_1200_real, dtft_1200_imag, dtft_2200_real, dtft_2200_imag;
  dtft_1200_real =  dtft_1200_imag =  dtft_2200_real =  dtft_2200_imag = 0;

  for (int i = 0; i < 8; i++){
    dtft_1200_real += (int8_t)(((int16_t)in[i]*(int16_t)coeff_1200_real[i])>>7);
    dtft_1200_imag += (int8_t)(((int16_t)in[i]*(int16_t)coeff_1200_imag[i])>>7);
    dtft_2200_real += (int8_t)(((int16_t)in[i]*(int16_t)coeff_2200_real[i])>>7);
    dtft_2200_imag += (int8_t)(((int16_t)in[i]*(int16_t)coeff_2200_imag[i])>>7);
  }
  dtft_1200_real = dtft_1200_real>>3;
  dtft_1200_imag = dtft_1200_imag>>3;
  dtft_2200_real = dtft_2200_real>>3;
  dtft_2200_imag = dtft_2200_imag>>3;

  int16_t dtft_1200 = dtft_1200_real*dtft_1200_real +
                      dtft_1200_imag*dtft_1200_imag;
  int16_t dtft_2200 = dtft_2200_real*dtft_2200_real +
                      dtft_2200_imag*dtft_2200_imag;
  // dtft_1200 = sqrt(dtft_1200);
  // dtft_2200 = sqrt(dtft_2200);

  // return octave_value(dtft_1200);
  // return octave_value(100);
  // return octave_value(dtft_2200);
  return octave_value(dtft_2200 - dtft_1200);
}
