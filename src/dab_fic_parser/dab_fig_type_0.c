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

struct BasicSubchannelOrganization *  appendSubchannel(struct BasicSubchannelOrganization *lst,
						       uint8_t * fig,uint8_t SubChId){

  struct BasicSubchannelOrganization *temp1;
  temp1 = lst;
  while(temp1->next!=NULL) {
    if (temp1->SubChId == SubChId)
      return lst;
    temp1 = temp1->next;
  }

  
  struct BasicSubchannelOrganization *temp;
  temp = malloc(sizeof(struct BasicSubchannelOrganization));
  temp->SubChId = SubChId;
  temp->startAddr = ((uint16_t)(fig[0] & 0x02)<<10) + (uint16_t)fig[1] ;
  temp->shortlong = fig[2] >> 7;
  if (temp->shortlong) {
    temp->option = (fig[2] >> 4) & 0x03;
    temp->protectionLevel = (fig[2] >> 2) & 0x02;
    temp->subchannelSize = ((uint16_t)(fig[2] & 0x02)<<8) + (uint16_t)fig[3]; 
    temp->tableSwitch = 0;
    temp->tableIndex = 0;
  } else {
    temp->option = 0;
    temp->protectionLevel = 0;
    temp->subchannelSize = 0;
    temp->tableSwitch = (fig[2] >> 6) & 0x01;
    temp->tableIndex = fig[2] & 0x3F;
  }  
  temp->next = lst;
  return temp;
 }


struct ServiceList * appendService(struct ServiceList *sl,uint8_t *fig,uint32_t sr,uint8_t pd){
  struct ServiceList * list;
  list = sl;
  while (list->next != NULL) {
    if (list->ServiceReference == sr)
      return sl;   
    list = list->next;     

  }

  struct ServiceList *new;
  new = malloc(sizeof(struct ServiceList));
  new->scp = malloc(sizeof(struct ServiceComponents));
  new->ServiceReference = sr;
  int idx;
  if (pd) {
    new->ECC = fig[0];
    new->CountryId = fig[1] >> 4;
    new->localFlag = fig[4] >> 7;
    new->CAId = (fig[4] >> 4) & 0x07;
    new->NumberOfSCs = fig[4] & 0x0F; 
    idx = 5;
      } else {
    new->ECC = 0;
    new->CountryId = fig[0] >> 4;
    new->localFlag = fig[2] >> 7;
    new->CAId = (fig[2] >> 4) & 0x07;
    new->NumberOfSCs = fig[2] & 0x0F;
    idx = 3;
  }
  new->next = sl;
  if (new->NumberOfSCs > 1)
    fprintf(stderr,"%s: TODO: implement multiple SCs per service!",__FUNCTION__);
  new->scp->TMId = fig[idx] >> 6;
  if (new->scp->TMId == 0){
    new->scp->ASCTy = fig[idx] & 0x3F;
    new->scp->SubChId = fig[idx+1] >> 2;
    new->scp->PS = (fig[idx+1] >> 1) & 0x01;
    new->scp->CAFlag = fig[idx+1] & 0x01;
    new->scp->DSCTy = 0;
    new->scp->SCId = 0;
  }
  if (new->scp->TMId == 1){
    new->scp->DSCTy = fig[idx] & 0x3F;
    new->scp->SubChId = fig[idx+1] >> 2;
    new->scp->PS = (fig[idx+1] >> 1) & 0x01;
    new->scp->CAFlag = fig[idx+1] & 0x01;
    new->scp->ASCTy = 0;
    new->scp->SCId = 0;
  }
  if (new->scp->TMId == 2){
    new->scp->DSCTy = fig[idx] & 0x3F;
    new->scp->FIDCId = fig[idx+1] >> 2;
    new->scp->PS = (fig[idx+1] >> 1) & 0x01;
    new->scp->CAFlag = fig[idx+1] & 0x01;
    new->scp->ASCTy = 0;
    new->scp->SCId = 0;
  }
  if (new->scp->TMId == 3){
    new->scp->SCId = ((uint16_t)(fig[idx] & 0x3F) << 6) + (fig[idx+1] >> 2);
    new->scp->PS = (fig[idx+1] >> 1) & 0x01;
    new->scp->CAFlag = fig[idx+1] & 0x01;
    new->scp->DSCTy = 0;
    new->scp->ASCTy = 0;
    new->scp->SubChId = 0;
    
  }
  /* debug print */
  if (0) {
  struct ServiceList *r;
  r = sl;
  fprintf(stderr,"ServiceList:              \n");
  while (r->next != NULL) {
    fprintf(stderr,"sr: %5u ecc: %2X cid: %2u lf: %1u caid:%2u nscs: %2u \n",
	    r->ServiceReference,
	    r->ECC,r->CountryId,
	    r->localFlag,r->CAId,r->NumberOfSCs);
    fprintf(stderr,"SCPs: ASCTy:%u,DSCTy:%u,SubChId:%u,SCId:%u\n",
	    r->scp->ASCTy,r->scp->DSCTy,r->scp->SubChId,r->scp->SCId);
    r = r->next;
  }
  }
  
  return new;

  
  

}

uint8_t dab_fig_type_0(uint8_t * fig,Ensemble * ens, uint32_t length){
  uint8_t extension = fig[0] & 0x1F;
  //fprintf(stderr,"ext: %u\n",extension);
  /* Basic sub-channel organization */
  uint32_t idx=1;
  if (extension == 0){
  }
  if (extension == 1){
    while (idx<(length)){
      ens->sco = appendSubchannel(ens->sco,&fig[idx+1],fig[idx+1]>>2);	
      if (fig[idx+3] >> 7)
	idx += 4;
      else
	idx += 3;
      //fprintf(stderr,"%u \n",fig[idx+1]>>2);
    }
  }
  if (extension == 2) {
    //fprintf(stderr,"P/D %u \n",(fig[0] >> 5) & 0x01);
    uint32_t sr=0;
    while (idx<length) {
      if (!((fig[0] >> 5) & 0x01)){
	sr = ((uint16_t)fig[idx] & 0x0F) + (uint8_t)fig[idx+1];
	ens->sl = appendService(ens->sl,&fig[idx],sr,0);
	idx = idx + (3+(fig[idx+2] & 0x0F)*2);
      } else {
	sr = (((uint32_t)fig[idx+1] & 0x0F) << 16) 
	  + ((uint32_t)fig[idx+2] << 8)  
	  + (uint32_t)fig[idx+3];
	ens->sl = appendService(ens->sl,&fig[idx],sr,1);
	idx = idx + (5+(fig[idx+4] & 0x0F)*2);
      }
    }
  }
  /* Service Component in Packet Mode */
  if (extension == 3) {

  }
  /* Service Linking Info */
  if (extension == 6) {

  }
  /* User Application Information */
  if (extension == 13) {
  
  }
  /* Program Type PTy */
  if (extension == 17) {

  }

  /* Anouncment */
  if (extension == 18) {


  }
}
