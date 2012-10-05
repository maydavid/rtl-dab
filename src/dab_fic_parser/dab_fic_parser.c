#include "dab_fic_parser.h"


void append(struct ProgramServiceLabel **lst,uint16_t SId ,uint8_t * label, uint16_t charFlag){

  struct ProgramServiceLabel *new;
  struct ProgramServiceLabel *lst_iter = *lst;
  new = malloc(sizeof(*new));
  new->SId = SId;
  memcpy(new->label,label,16);
  new->label[16] = '\0';
  new->charFlag=charFlag; 
  new->next = NULL;
 
  if ( lst_iter != NULL ) {
    while (lst_iter->next != NULL )
      lst_iter=lst_iter->next;
    lst_iter->next=new;
    }
    else
      *lst=new;
}

uint8_t isinlist(struct ProgramServiceLabel *lst,uint16_t SId){
  struct ProgramServiceLabel *list;
  list = lst;
  while (list->next != NULL) {
    list = list->next; 
    if (list->SId == SId)
      return 1;
    
  }
  list = lst;
  system("clear");
  fprintf (stderr,"\nprogram list:--------------------------------\n");
  fprintf(stderr,"Service ID --- Label\n");
  while (list->next != NULL) {
    list = list->next; 
    
    fprintf(stderr,"%10u  ---  %s\n",list->SId, list->label);
  }
  return 0;
}

struct ProgramServiceLabel * psl = NULL;
struct EnsembleLabel * enl = NULL;

uint8_t dab_fic_parser(uint8_t fibs[12][256]){
  int32_t i,j;
  uint8_t fib_c[12][32];
  uint8_t type=0,length=0,shift=0;
  uint8_t extension;

  for (i=0;i<12;i++){
    /* CRC check */
    j = dab_crc16(fibs[i], 256);
    //if (j)
    //  fprintf(stderr,".");
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
	    extension = fib_c[i][shift+1] & 0x03;
	    if (extension == 0) {
	      //for (j=0;j<16;j++)
	      //fprintf(stderr,"%c", fib_c[i][shift+4+j]);
	    }
	    if (extension == 1) {
	      if (psl != NULL) {
		if (isinlist(psl,((uint16_t)fib_c[i][shift+2]<<8) + fib_c[i][shift+3])==0){
		  append(&psl,((uint16_t)fib_c[i][shift+2]<<8) + fib_c[i][shift+3],
			 &fib_c[i][shift+4],
			 ((uint16_t)fib_c[i][shift+19]<<8) + fib_c[i][shift+20]);
		}
	      }	else {
		  append(&psl,((uint16_t)fib_c[i][shift+2]<<8) + fib_c[i][shift+3],
			 &fib_c[i][shift+4],
			 ((uint16_t)fib_c[i][shift+19]<<8) + fib_c[i][shift+20]);   
	      }
	    }
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
