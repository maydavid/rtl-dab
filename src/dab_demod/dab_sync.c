/*
This file is part of rtl-dab
trl-dab is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with rtl-dab.  If not, see <http://www.gnu.org/licenses/>.


david may 2012
david.may.muc@googlemail.com

*/

#include "dab_sync.h"

float mag_squared(fftw_complex sample) {
    const float __x = sample[0];
    const float __y = sample[1];
    return __x * __x + __y * __y;
}

uint32_t dab_coarse_time_sync(int8_t * real,uint8_t * buffer,float * filt) {

  int32_t j,k;
  float e=0;
  float threshold=5000;
  for (k=0;k<2662;k+=10)
    e = e +(float) abs(real[k]);
  //fprintf(stderr,"%f \n",e);
  if (e<threshold)
    return 0;

  for (j=0;j<(196608-2662)/10;j++)
    filt[j] = 0;
  for (j=0;j<196608-2662;j+=10)
    for (k=0;k<2662;k+=10)
      filt[j/10] = filt[j/10] +(float) abs(real[j+k]);
  
  float minVal=9999999;
  uint32_t minPos;
  for (j=0;j<(196608-2662)/10;j++){
    if (filt[j]<minVal) {
      minVal = filt[j];
      minPos = j*10;
    }
  }
  return minPos*2;
}


int32_t dab_fine_time_sync(fftw_complex * frame,fftw_complex * prs_ifft){

  fftw_complex mulres[2048];
  float mulres_abs[2048];
  float mulres_sum[41];
  int32_t i,j;
  float minVal=0;
  int32_t minPos;

  for (j=-20;j<21;j++){
    mulres_sum[j+20] = 0;
  }
  for(i=0;i<2048;i++)
      mulres_abs[i] = 0;

  for (j=-20;j<21;j++){
    for (i=0;i<2048;i++){
      mulres[i][0] = (frame[2656+j+i][0]*prs_ifft[i][0]-frame[2656+j+i][1]*prs_ifft[i][1]);
      mulres[i][1] = (frame[2656+j+i][0]*prs_ifft[i][1]+frame[2656+j+i][1]*prs_ifft[i][0]);
    }
    for(i=0;i<2048;i++){
      mulres_abs[i] = sqrt((mulres[i][1]*mulres[i][1])+(mulres[i][0]*mulres[i][0]));
    }
    for(i=0;i<2048;i++)
      mulres_sum[j+20] = mulres_sum[j+20] + mulres_abs[i]; 
  }

  for (j=0;j<41;j++){
    if (mulres_sum[j]>minVal) {
      minVal = mulres_sum[j];
      minPos = j;
    }
  }
  return (minPos-21)*2;

}

/* adapted from gr-dab (c) Andreas Müller*/
int32_t dab_coarse_freq_sync(fftw_complex * symbols){
  uint32_t i, index;
  double sum=0, max=0;
  uint32_t d_num_carriers = 1536;
  uint32_t d_fft_length = 2048;
  

  for (i=0; i<d_num_carriers+1; i++) {
    if (i != d_num_carriers/2)
      sum+=(double)mag_squared(symbols[i]);
  }
  max = sum;
  index = 0;
  for (i=1; i<d_fft_length-d_num_carriers; i++) {
    sum -= (double)mag_squared(symbols[i-1]);
    sum += (double)mag_squared(symbols[i+d_num_carriers/2-1]);
    sum -= (double)mag_squared(symbols[i+d_num_carriers/2]);
    sum += (double)mag_squared(symbols[i+d_num_carriers]);
    if (sum > max) {
      max = sum;
      index = i;
    }
  }

  return (int32_t)(index+(d_num_carriers/2)-(d_fft_length/2));
}


double dab_fine_freq_corr(fftw_complex * dab_frame,uint32_t fine_timeshift){
  fftw_complex *left;
  fftw_complex *right;
  fftw_complex *lr;
  double angle[504];
  double mean;
  double ffs;
  left = fftw_malloc(sizeof(fftw_complex) * 504);
  right = fftw_malloc(sizeof(fftw_complex) * 504);
  lr = fftw_malloc(sizeof(fftw_complex) * 504);
  uint32_t i;

  for (i=0;i<504;i++) {
    left[i][0] = dab_frame[2656+2048+i+fine_timeshift][0];
    left[i][1] = dab_frame[2656+2048+i+fine_timeshift][1];
    right[i][0] = dab_frame[2656+i+fine_timeshift][0];
    right[i][1] = dab_frame[2656+i+fine_timeshift][1];
  }
  for (i=0;i<504;i++){
    lr[i][0] = (left[i][0]*right[i][0]-left[i][1]*-right[i][1]);
    lr[i][1] = (left[i][0]*-right[i][1]+left[i][1]*right[i][0]);
  }
  
  for (i=0;i<504;i++){
   angle[i] = atan2(lr[i][1],lr[i][0]);
  }
  for (i=0;i<504;i++){
    mean = mean + angle[i];
  }
  mean = (mean/504) * -1;
    
  ffs = mean / (2 * M_PI) * 1000;
  
    
 return ffs;
}
