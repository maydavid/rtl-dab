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


#include "dab_demod.h"

int8_t dab_demod(dab_state *dab){
  uint32_t i,j;
  
  /* write input data into fifo */
  for (i=0;i<dab->input_buffer_len;i++) {
    cbWrite(&(dab->fifo),&dab->input_buffer[i]);
  }
  
  /* Check for data in fifo */
  if (dab->fifo.count < 196608*3) {
    return 0;
  }
  
  /* read fifo */
  dab_read_fifo(&dab->fifo,196608*2,dab->coarse_timeshift+dab->fine_timeshift,dab->input_buffer);
  
  /* resetting coarse timeshift */
  dab->coarse_timeshift = 0;

  /* complex data conversion */
  for (j=0;j<196608*2;j+=2){
    dab->real[j/2]=dab->input_buffer[j]-127;
    dab->imag[j/2]=dab->input_buffer[j+1]-127;
  }
  
  /* coarse time sync */
  dab->coarse_timeshift = dab_coarse_time_sync(dab->real,dab->input_buffer,dab->filt);

  /* create complex frame */
  for (j=0;j<196608;j++){
    dab->dab_frame[j][0] = dab->real[j];
    dab->dab_frame[j][1] = dab->imag[j];
  }

return 1;

}


void dab_demod_init(dab_state * dab){
  cbInit(&(dab->fifo),(196608*2*4)); // 4 frames
  dab->coarse_timeshift = 0;
  dab->fine_timeshift=0;
  dab->dab_frame = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * 196608 );
}
