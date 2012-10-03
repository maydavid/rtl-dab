#include "dab_fic_depuncture.h"

int32_t fic_depuncture(int32_t *in, int32_t *out)
{
    int32_t i,j,offset;

    offset = 0;
    for (i=0; i<21*128; i+=32)
    {
        for (j=0; j<8; j++)
        {
            out[i+j*4+0] = in[offset+0];
            out[i+j*4+1] = in[offset+1];
            out[i+j*4+2] = in[offset+2];
            out[i+j*4+3] = 255;           
            offset+=3;
        }
    }
    for (i=21*128; i<24*128; i+=32)
    {
        for (j=0; j<7; j++)
        {
            out[i+j*4+0] = in[offset+0];
            out[i+j*4+1] = in[offset+1];
            out[i+j*4+2] = in[offset+2];
            out[i+j*4+3] = 255;
            offset+=3;
        }
        out[i+j*4+0] = in[offset+0];
        out[i+j*4+1] = in[offset+1];
        out[i+j*4+2] = 255;
        out[i+j*4+3] = 255;
        offset+=2;
    }
    for (j=0; j<6; j++)
    {
        out[i+j*4+0] = in[offset+0];
        out[i+j*4+1] = in[offset+1];
        out[i+j*4+2] = 255;
        out[i+j*4+3] = 255;
        offset+=2;
    }

   return 1;
}

