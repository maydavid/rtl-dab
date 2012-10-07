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


void appendServiceLabel(struct ProgramServiceLabel *lst,uint16_t SId ,
			uint8_t * label, uint16_t charFlag){

  struct ProgramServiceLabel *temp1;
  temp1 = malloc(sizeof(struct ProgramServiceLabel));
  temp1 = lst;
  while(temp1->next!=NULL)
    temp1 = temp1->next;
  
  struct ProgramServiceLabel *temp;
  temp = malloc(sizeof(struct ProgramServiceLabel));
  temp->SId = SId;
  memcpy(temp->label,label,16);
  temp->charFlag = charFlag;
  temp->next = NULL;
  temp1->next = temp;
  
 }

uint8_t isServiceLabel(struct ProgramServiceLabel *lst,uint16_t SId){
  
  struct ProgramServiceLabel *list;
  list = lst;
  while (list->next != NULL) {
    list = list->next; 
    if (list->SId == SId)
      return 1;   
  }
  list = lst;
  system("clear");
  fprintf (stderr,"\nProgram list:--------------------------------\n");
  fprintf(stderr,"Service ID --- Label\n");
  while (list->next != NULL) {
    list = list->next; 
    
    fprintf(stderr,"%10u  ---  %s\n",list->SId, list->label);
  }
  return 0;
  
}


uint8_t dab_fig_type_1(uint8_t * fig,ServiceInformation * sinfo){
  uint8_t extension = fig[0] & 0x03;
  //fprintf(stderr,"%u\n",extension);
  
  if (extension == 0) {
  }
  if (extension == 1) {
    //fprintf(stderr,"hh\n");
    if (sinfo->psl != NULL) {
      //fprintf(stderr,"aa\n");
      if (isServiceLabel(sinfo->psl,((uint16_t)fig[1]<<8) + fig[2])==0){
	appendServiceLabel(sinfo->psl,((uint16_t)fig[1]<<8) + fig[2],&fig[3],
			   ((uint16_t)fig[4]<<8) + fig[5]);
      }
    }	else {
      //fprintf(stderr,"ll\n");
      appendServiceLabel(sinfo->psl,((uint16_t)fig[1]<<8) + fig[2],&fig[3],
      	 ((uint16_t)fig[4]<<8) + fig[5]);
    }
  }
  
}
