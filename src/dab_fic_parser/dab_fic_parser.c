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

#include "dab_fic_parser.h"




//struct ProgramServiceLabel * psl = NULL;
//struct EnsembleLabel * enl = NULL;
//struct ServiceInformation * sin = NULL;

void dab_fic_parser_init(ServiceInformation *sinfo) {
  sinfo->psl = malloc(sizeof(struct ProgramServiceLabel));
  sinfo->psl->next = NULL;
  sinfo->esl = NULL;
}

uint8_t dab_fic_parser(uint8_t fibs[12][256],ServiceInformation *sinfo){
  int32_t i,j;
  uint8_t fib_c[12][32];
  uint8_t type=0,length=0,shift=0;
  //uint8_t extension;
  uint8_t charEnc;

  for (i=0;i<12;i++){
    /* CRC check */
    j = dab_crc16(fibs[i], 256);
    dab_bit_to_byte(fibs[i],fib_c[i],256);
    if (j==0) {
      type = 0;
      length = 0;
      shift = 0;
      while (shift<30) {
	type = fib_c[i][shift]>>5;
	length = fib_c[i][shift] & 0x1f;
	
	switch(type)
	  {
	  case 0:
	    break;
	  case 1:
	    dab_fig_type_1(&fib_c[i][shift+1],sinfo);
	    break;
	  case 2:
	    break;
	  default:
	    break;
	  }
	shift = shift+length+1;
      }
    }
  }
  

  return 1;

}
