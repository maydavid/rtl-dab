#include "dab_analyzer.h"

void dab_analyzer_init(Analyzer * ana) {
  ana->received_fibs = 0;
  ana->faulty_fibs = 0;
  ana->ber = 0;
}


int dab_analyzer_calculate_error_rates(Analyzer *ana,dab_state *dab) {

  //conv enc
  dab_bit_to_byte(&dab->FIC_dep_dec[0],&dab->FIC_dep_dec_byte[0],(768*4));
  // just one atm
  encode(&dab->FIC_dep_enc[0*3096],&dab->FIC_dep_dec_byte[0*768/8],768/8,0,0);
  encode(&dab->FIC_dep_enc[1*3096],&dab->FIC_dep_dec_byte[1*768/8],768/8,0,0);
  encode(&dab->FIC_dep_enc[2*3096],&dab->FIC_dep_dec_byte[2*768/8],768/8,0,0);
  encode(&dab->FIC_dep_enc[3*3096],&dab->FIC_dep_dec_byte[3*768/8],768/8,0,0);


  int i,j;
  int s=0;
  int idx =0;
  int t;

  for (j=0;j<4;j++) {
    idx = 2304*j;
    for (i=0;i<3096;i++) {
      if (dab->FIC_dep[(3096*j)+i] != 8) {
	dab->FIC_enc_pun[idx++] = dab->FIC_dep_enc[(3096*j)+i];
      }
    }
  }
  
  
  for (i=0;i<2304*2;i++) {
    t = (dab->FIC[i]-dab->FIC_enc_pun[i]);
    if (t!=0) {
      s++;
    }
  }
  //printf("%i\n",s);
  // channel ber
  ana->ber = (float)s/(float)(1536*2*3);
  return 1;
}


