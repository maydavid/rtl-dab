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


typedef struct {

  struct ProgramServiceLabel psl;



} ServiceInformation;



uint8_t dab_fic_parser(uint8_t fibs[12][256]);
