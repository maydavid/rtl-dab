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
#include "prs.h"

float mag_squared(fftw_complex sample) {
    float x = sample[0];
    float y = sample[1];
    return x * x + y *y;
}

uint32_t dab_coarse_time_sync(int8_t * real,uint8_t * buffer,float * filt) {

  int32_t tnull = 2656; // was 2662? why?
  int32_t j,k;

  // check for energy in fist tnull samples
  float e=0;
  float threshold=5000;
  for (k=0;k<tnull;k+=10)
    e = e +(float) abs(real[k]);
  fprintf(stderr,"Energy over nullsymbol: %f\n",e);
  if (e<threshold)
    return 0;

  // energy was to high so we assume we are not in sync
  // subsampled filter to detect where the null symbol is
  for (j=0;j<(196608-tnull)/10;j++)
    filt[j] = 0;
  for (j=0;j<196608-tnull;j+=10)
    for (k=0;k<tnull;k+=10)
      filt[j/10] = filt[j/10] +(float) abs(real[j+k]);

  // finding the minimum in filtered data gives position of null symbol
  float minVal=9999999;
  uint32_t minPos;
  for (j=0;j<(196608-tnull)/10;j++){
    if (filt[j]<minVal) {
      minVal = filt[j];
      minPos = j*10;
    }
  }
  //fprintf(stderr,"calculated position of nullsymbol: %f",minPos*2);
  return minPos*2;
}


int32_t dab_fine_time_sync(fftw_complex * frame,fftw_complex * prs){

  /* correlation in frequency domain 
     e.g. J.Cho "PC-based receiver for Eureka-147" 2001
     e.g. K.Taura "A DAB receiver" 1996
  */
#define dbg 1
  int k;
#if dbg
  FILE *fh0;
  fh0 = fopen("prs_received.dat","w+");
  for(k=0;k<2048;k++) {
    fprintf(fh0,"%f\n",(frame[2656+504+k][0]));
    fprintf(fh0,"%f\n",(frame[2654+504+k][1]));
  }
  fclose(fh0);
#endif



  /* first we have to transfer the receive prs symbol in frequency domain */
  fftw_complex prs_received_fft[2048];
  fftw_plan p;
  p = fftw_plan_dft_1d(2048, &frame[2656+504], &prs_received_fft[0], FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(p);
#if dbg
  FILE *fh1;
  fh1 = fopen("prs_received_fft.dat","w+");
  for(k=0;k<2048;k++) {
    fprintf(fh1,"%f\n",(prs_received_fft[k][0]));
    fprintf(fh1,"%f\n",(prs_received_fft[k][1]));
  }
  fclose(fh1);
#endif
  /* now we build the complex conjugate of the known prs */
  // 1536 as only the carries are used
  fftw_complex prs_star[1536];
  int i;
  for (i=0;i<1536;i++) {
    prs_star[i][0] = prs_static[i][0];
    prs_star[i][1] = -1 *  prs_static[i][1];
  }
#if dbg
  FILE *fh2;
  fh2 = fopen("prs_star.dat","w+");
  for(k=0;k<1536;k++) {
    fprintf(fh2,"%f\n",(prs_star[k][0]));
    fprintf(fh2,"%f\n",(prs_star[k][1]));
  }
  fclose(fh2);
#endif


  /* fftshift the received prs
     at this point we have to be coarse frequency sync 
     however we can simply shift the bins */
  fftw_complex prs_rec_shift[1536];
  // TODO allow for coarse frequency shift !=0 
  int32_t cf_shift = 0;
  // matlab notation (!!!-1)
  // 769:1536+s
  //  2:769+s why 2? I dont remember, but peak is very strong
  for (i=0;i<1536;i++) {
    if (i<768) {
      prs_rec_shift[i][0] = prs_received_fft[i+1280][0];
      prs_rec_shift[i][1] = prs_received_fft[i+1280][1];
    }
    if (i>=768) {
      prs_rec_shift[i][0] = prs_received_fft[i-765][0];
      prs_rec_shift[i][1] = prs_received_fft[i-765][1];

    }
  }
#if dbg
  FILE *fh3;
  fh3 = fopen("prs_rec_shift.dat","w+");
  for(k=0;k<1536;k++) {
    fprintf(fh3,"%f\n",(prs_rec_shift[k][0]));
    fprintf(fh3,"%f\n",(prs_rec_shift[k][1]));
  }
  fclose(fh3);
#endif


  
  /* now we convolute both symbols */
  fftw_complex convoluted_prs[1536];
  int s;
  for (s=0;s<1536;s++) {
    convoluted_prs[s][0] = prs_rec_shift[s][0]*prs_star[s][0]-prs_rec_shift[s][1]*prs_star[s][1];
    convoluted_prs[s][1] = prs_rec_shift[s][0]*prs_star[s][1]+prs_rec_shift[s][1]*prs_star[s][0];
  }

  /* and finally we transfer the convolution back into time domain */
  fftw_complex convoluted_prs_time[1536]; 
  fftw_plan px;
  px = fftw_plan_dft_1d(1536, &convoluted_prs[0], &convoluted_prs_time[0], FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(px);
#if dbg
  FILE *fh4;
  fh4 = fopen("convoluted_prs_time.dat","w+");
  for(k=0;k<1536;k++) {
    fprintf(fh4,"%f\n",(convoluted_prs_time[k][0]));
    fprintf(fh4,"%f\n",(convoluted_prs_time[k][1]));
  }
  fclose(fh4);
#endif

  uint32_t maxPos=0;
  float tempVal = 0;
  float maxVal=-99999;
  for (i=0;i<1536;i++) {
    tempVal = sqrt((convoluted_prs_time[i][0]*convoluted_prs_time[i][0])+(convoluted_prs_time[i][1]*convoluted_prs_time[i][1]));
    if (tempVal>maxVal) {
      maxPos = i;
      maxVal = tempVal;
    }
  }
  


#if dbg
  fprintf(stderr,"Fine time shift: %d\n",maxPos);
#endif
  if (maxPos<1536/2) {
    return maxPos*2+16;
  } else {
    return ((maxPos-(1536))*2);
  }
  //return 0;
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


double dab_fine_freq_corr(fftw_complex * dab_frame,int32_t fine_timeshift){
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
