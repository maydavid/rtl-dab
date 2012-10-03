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
