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


struct ProgrammeServiceLabel * appendServiceLabel(struct ProgrammeServiceLabel *lst,uint16_t SId ,
			uint8_t * label, uint16_t charFlag){

  struct ProgrammeServiceLabel *temp1;
  temp1 = lst;
  while(temp1->next!=NULL){
    if (temp1->SId == SId)
      return lst;
    temp1 = temp1->next;
  
  }
  struct ProgrammeServiceLabel *temp;
  temp = malloc(sizeof(struct ProgrammeServiceLabel));
  temp->SId = SId;
  memcpy(temp->label,label,16);
  temp->label[16] = '\0';
  temp->chFlag = charFlag;
  temp->next = lst;
  return temp;
  
 }




uint8_t dab_fig_type_1(uint8_t * fig,Ensemble * sinfo){
  uint8_t extension = fig[0] & 0x03;
  //fprintf(stderr,"%u\n",extension);
  
  if (extension == 0) {
    //fprintf(stderr,"FIG 1/0\n");
    sinfo->esl->EId = ((uint16_t)fig[1]<<8) + fig[2];
    memcpy(sinfo->esl->label,&fig[3],16);
    sinfo->esl->label[16] = '\0';
    sinfo->esl->chFlag = ((uint16_t)fig[4]<<8) + fig[5];
  }
  if (extension == 1) {
    sinfo->psl = appendServiceLabel(sinfo->psl,((uint16_t)fig[1]<<8) + fig[2],&fig[3],
		       ((uint16_t)fig[4]<<8) + fig[5]);
    
  }
  /* FIG 1/3 region Label */
  if (extension == 3) {
    //++fig;
    //fprintf(stderr,"%s\n",++fig);
  }
  if (extension == 5) {
    //fprintf(stderr,"FIG 1/5\n");
    //fprintf(stderr,"%u\n",(uint32_t)fig);
  }
  return 0;
}
