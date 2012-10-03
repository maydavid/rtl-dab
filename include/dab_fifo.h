#include <stdio.h>
#include <stdint.h>
#include <malloc.h>


typedef struct 
{
  uint32_t size;
  uint32_t start;
  uint32_t count;
  uint8_t *elems;
} CircularBuffer;


void cbInit(CircularBuffer *cb, uint32_t size);
void cbFree(CircularBuffer *cb);
int cbIsFull(CircularBuffer *cb);
int cbIsEmpty(CircularBuffer *cb);
void cbWrite(CircularBuffer *cb, uint8_t *elem);
void cbRead(CircularBuffer *cb, uint8_t *elem);
