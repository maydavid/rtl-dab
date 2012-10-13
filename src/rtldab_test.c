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

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "dab_demod.h"
#include "dab_fic_parser.h"




int corr_counter;

//ServiceInformation sinfo;
Ensemble sinfo;

int main(void){

  dab_state dab;
  int frequency = 222055000;
  
  FILE *fh;
  fh = fopen("222055_dump.dump","r");
  dab_demod_init(&dab);
  dab_fic_parser_init(&sinfo);
  fread(dab.input_buffer,1,16*16384,fh);
dab.input_buffer_len = 16*16384;
 int i;

 for (i=0;i<20;i++) {
   fread(dab.input_buffer,1,16*16384,fh);
   dab.input_buffer_len = 16*16384;
   dab_demod(&dab);
   dab_fic_parser(dab.fib,&sinfo);
   if (abs(dab.fine_freq_shift) > 20 || abs(dab.coarse_freq_shift) > 1) {
     corr_counter++;
     corr_counter = 0 ;
     frequency = frequency - dab.fine_freq_shift;// + dab2->coarse_freq_shift*1000;
     fprintf(stderr,"cfs : %i\n",dab.coarse_freq_shift);
     fprintf(stderr,"ffs : %f\n",dab.fine_freq_shift);
   }
 } 
 
 
 return 1;
}
