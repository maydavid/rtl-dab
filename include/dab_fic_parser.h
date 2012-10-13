#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "dab_helper_functions.h"

typedef struct{
  
  struct ServiceList *sl;

}Ensemble;

struct ServiceComponents{
  uint8_t TMId;
  uint8_t ASCTy;
  uint8_t DSCTy;
  uint16_t SCId;
  uint8_t SubChId;
  uint8_t FIDCId;
  uint8_t PS;
  uint8_t CAFlag;
  struct ServiceComponents *next;
};

struct ServiceList {

  uint8_t ECC;
  uint8_t CountryId;
  uint32_t ServiceReference;
  uint8_t localFlag;
  uint8_t CAId;
  uint8_t NumberOfSCs;
  struct ServiceComponents * scp;
  struct ServiceList *next;
};




uint8_t dab_fic_parser(uint8_t fibs[12][256],Ensemble * ens);
void dab_fic_parser_init(Ensemble *ens);

//uint8_t dab_fig_type_1(uint8_t * fig,ServiceInformation * sinfo);
//uint8_t dab_fig_type_0(uint8_t * fig,ServiceInformation * sinfo,uint32_t length);
