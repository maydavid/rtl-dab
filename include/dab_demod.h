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


#include <stdint.h>
#include "dab_fifo.h"


#define DEFAULT_BUF_LENGTH (16 * 16384)

typedef struct{
  uint32_t frequency;
  uint8_t input_buffer[DEFAULT_BUF_LENGTH];
  uint16_t input_buffer_len;
  CircularBuffer fifo;
}dab_state;


int8_t dab_demod(dab_state *dab);
