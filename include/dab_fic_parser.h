#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "dab_helper_functions.h"

typedef struct{
  
  uint8_t locked;
  struct ServiceList *sl;
  struct BasicSubchannelOrganization *sco;
  

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

/* FIG 0/1 */
struct BasicSubchannelOrganization {
  uint8_t SubChId;
  uint16_t startAddr;
  uint8_t shortlong;
  uint8_t tableSwitch;
  uint8_t tableIndex;
  uint8_t option;
  uint8_t protectionLevel;
  uint16_t subchannelSize;
  struct BasicSubchannelOrganization * next;
};




uint8_t dab_fic_parser(uint8_t fibs[12][256],Ensemble * ens);
void dab_fic_parser_init(Ensemble *ens);

//uint8_t dab_fig_type_1(uint8_t * fig,ServiceInformation * sinfo);
uint8_t dab_fig_type_0(uint8_t * fig,Ensemble * ens,uint32_t length);
