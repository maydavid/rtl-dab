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

#include "dab_fic_descramble.h"


int16_t dab_fic_descramble(uint8_t *in, uint8_t * out, int32_t len)
{
  int32_t i;
  uint16_t p = 0x01FF;
  int32_t pp;
  for (i=0; i<len; i++)
    {
      p = p << 1;
      pp = ((p>>9)&1) ^ ((p>>5)&1);
      p |= pp;
      out[i] = in[i] ^ pp;
    }
  return 1;
} 
