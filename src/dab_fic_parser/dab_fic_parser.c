#include "dab_fic_parser.h"


uint8_t dab_fic_parser(uint8_t fibs[12][256]){
  int32_t i,j;
  uint8_t fib_c[12][32];
  uint8_t type=0,length=0,shift=0;

  
  for (i=0;i<12;i++){
    /* CRC check */
    j = dab_crc16(fibs[i], 256);
    if (j)
      fprintf(stderr,".");
    dab_bit_to_byte(fibs[i],fib_c[i],256);
    if (j==0) {
      
      //for(j=0;j<32;j++)
      //fprintf(stderr,"%c",fib_c[i][j]);
      type = 0;
      length = 0;
      shift = 0;
      while (shift<30) {
	type = fib_c[i][shift]>>5;
	length = fib_c[i][shift] & 0x1f;
	
	//fprintf(stderr,"type: %u \n",type);
	//fprintf(stderr,"leng: %u \n",length);
	//fprintf(stderr,"shif: %u \n",shift);
	switch(type)
	  {
	  case 0:
	    break;
	  case 1:
	    fprintf(stderr,"type1\n");
	    uint8_t charset = fib_c[i][shift+1]>>4;
	    uint8_t oe = (fib_c[i][shift+1] >> 3) & 0x01;
	    uint8_t extension = fib_c[i][shift+1] & 0x03;
	    //fprintf(stderr,"ext.: %u \n",extension);
	    if (extension == 1) {
	      uint8_t str[16];
	      memcpy(str,&fib_c[i][shift+4],16);
	      fprintf(stderr,"%s\n",str);
	    }
	    break;
	  case 2:
	    //fprintf(stderr,"type2\n"); 
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
