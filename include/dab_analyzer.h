#pragma once


#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "dab_demod.h"
#include "viterbi.h"
//#include "dab_helper_functions.h"

typedef struct{
  
  uint32_t received_fibs;
  uint32_t faulty_fibs;
  float ber;

}Analyzer;


void dab_analyzer_init(Analyzer * ana);


int dab_analyzer_calculate_error_rates(Analyzer *ana,dab_state *dab);
