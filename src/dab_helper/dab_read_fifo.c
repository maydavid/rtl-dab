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

#include "dab_read_fifo.h"

int32_t dab_read_fifo(CircularBuffer * fifo,uint32_t bytes,int32_t shift,uint8_t * buffer)
{
  int32_t i=0;
  uint32_t j=0;
  if (shift>0)
    {
      for (i=0;i<shift;i++)
	if(!cbIsEmpty(fifo))
	  cbRead(fifo,&buffer[i]);
      for (j=0;j<bytes;j++)
	if(!cbIsEmpty(fifo))
	  cbRead(fifo,&buffer[j]);
    }
  else {
    for (j=0;j<bytes+shift;j++)
      cbRead(fifo,&buffer[j]);
  }
  return 1;
}
