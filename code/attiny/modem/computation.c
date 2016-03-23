#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){


   
  /* int8_t samples[8] = {127,152,158,141,113,96,102,127}; //1200Hz */
  /* int8_t samples[8] = {127,159,122,96,136,157,113,99}; //2200Hz */

  /* int8_t samples[8] = {0,25,31,14,-14,-31,-25,-0}; //1200Hz */
  /* int8_t samples[8] = {0,32,-5,-31,9,30,-14,-28}; //2200Hz */
    // get some vars ready for the computation

  uint8_t samples[41][8] = {{127,152,158,141,113,96,102,127},
                            {127,153,157,134,105,95,113,143},
                            {127,155,155,127,99,99,127,155},
                            {127,156,152,120,96,107,141,159},
                            {127,157,149,113,95,118,152,155},
                            {127,158,145,107,97,129,158,143},
                            {127,158,141,102,102,141,158,127},
                            {127,159,136,98,109,150,152,111},
                            {127,159,132,96,118,157,141,99},
                            {127,159,127,95,127,159,127,95},
                            {127,159,122,96,136,157,113,99},
                            {127,159,118,98,145,150,102,111},
                            {127,158,113,102,152,141,96,127},
                            {127,158,109,107,157,129,96,143},
                            {127,157,105,113,159,118,102,155},
                            {127,156,102,120,158,107,113,159},
                            {127,155,99,127,155,99,127,155},
                            {127,153,97,134,149,95,141,143},
                            {127,152,96,141,141,96,152,127},
                            {127,150,95,147,132,101,158,111},
                            {127,149,95,152,122,109,158,99},
                            {127,147,96,156,113,120,152,95},
                            {127,145,97,158,105,132,141,99},
                            {127,143,99,159,99,143,127,111},
                            {127,141,102,158,96,152,113,127},
                            {127,139,105,156,95,158,102,143},
                            {127,136,109,152,97,159,96,155},
                            {127,134,113,147,102,156,96,159},
                            {127,132,118,141,109,149,102,155},
                            {127,129,122,134,118,139,113,143},
                            {127,127,127,127,127,127,127,127},
                            {127,125,132,120,136,115,141,111},
                            {127,122,136,113,145,105,152,99},
                            {127,120,141,107,152,98,158,95},
                            {127,118,145,102,157,95,158,99},
                            {127,115,149,98,159,96,152,111},
                            {127,113,152,96,158,102,141,127},
                            {127,111,155,95,155,111,127,143},
                            {127,109,157,96,149,122,113,155},
                            {127,107,158,98,141,134,102,159},
                            {127,105,159,102,132,145,96,155}};


  /* uint8_t samples[41][8] = {{127,152,158,141,113,96,102,127}}; */
  /* uint8_t samples[41][8] = {{127,159,122,96,136,157,113,99}}; */

    // add most recent ADC reading to samples[]
  int16_t cof1 = 0, cof2 = 0, cof3 = 0, cof4 = 0;
  for(int i = 0;i < 41; i++){
    int oldest_sample = 0;

    int16_t data = samples[i][oldest_sample] << 6;

    cof1 = data;
    cof2 = 0;
    cof3 = data;
    cof4 = 0;


    data = samples[i][(oldest_sample + 1)%8] - 127;
    cof1 += data;
    cof2 += data;
    cof4 += data;
    /* printf("data:%d\n",data); */
    /* printf("cof1:%d\n",cof1); */
    /* printf("cof2:%d\n",cof2); */
    /* printf("cof3:%d\n",cof3); */
    /* printf("cof4:%d\n",cof4); */
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


    data = samples[i][(oldest_sample + 2)%8] - 127;
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

    data = samples[i][(oldest_sample + 3)%8] - 127;
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

    data = samples[i][(oldest_sample + 4)%8] - 127;
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

    data = samples[i][(oldest_sample + 5)%8] - 127;
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

    data = samples[i][(oldest_sample + 6)%8] - 127;
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

    data = samples[i][(oldest_sample + 7)%8] - 127;
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
    /* if (cof1 <= 0){ */
    /*   cof1 *= -1; */
    /* } */
    /* if (cof2 <= 0){ */
    /*   cof1 *= -1; */
    /* } */
    /* if (cof3 <= 0){ */
    /*   cof1 *= -1; */
    /* } */
    /* if (cof4 <= 0){ */
    /*   cof1 *= -1; */
    /* } */

    /* printf("%d\n",cof1); */
    /* printf("%d\n",cof2); */
    /* printf("%d\n",cof3); */
    /* printf("%d\n",cof4); */
    printf("%d\n",cof3*cof3 + cof4*cof4 - cof1*cof1 - cof2*cof2);
      
  }

}
