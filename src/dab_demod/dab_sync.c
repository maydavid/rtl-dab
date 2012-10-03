#include "dab_sync.h"

uint32_t dab_coarse_time_sync(int8_t * real,uint8_t * buffer,float * filt) {

  int j,k;
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
