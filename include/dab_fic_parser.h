#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "dab_helper_functions.h"

struct ProgramServiceLabel {
  uint8_t charset;
  uint8_t oe;
  uint8_t extension;
  uint16_t SId;
  uint8_t label[17];
  uint16_t charFlag;
  struct ProgramServiceLabel * next;
};

struct EnsembleLabel {
  uint8_t charset;
  uint8_t oe;
  uint8_t extension;
  uint16_t EId;
  uint8_t label[17];
  uint16_t charFlag;
};

/* FIG 0/1 */
struct SubchannelOrganization {
  uint8_t SubChId;
  uint16_t startAddr;
  uint8_t shortlong;
  uint8_t tableSwitch;
  uint8_t tableIndex;
  uint8_t option;
  uint8_t protectionLevel;
  uint16_t subchannelSize;
  struct SubchannelOrganization * next;
};

/* FIG 0/2 */
struct ServiceOrganization {
  
  
  
};



typedef struct{

  struct ProgramServiceLabel * psl;
  struct EnsembleLabel * esl;

}ServiceInformation;



uint8_t dab_fic_parser(uint8_t fibs[12][256],ServiceInformation * sinfo);
void dab_fic_parser_init(ServiceInformation *sinfo);

uint8_t dab_fig_type_1(uint8_t * fig,ServiceInformation * sinfo);
