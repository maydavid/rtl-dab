#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "rtl-sdr.h"


#define DEFAULT_BUF_LENGTH (16 * 16384)

typedef struct{
  uint32_t frequency;
  uint8_t input_buffer[DEFAULT_BUF_LEN];
}dab_state;
