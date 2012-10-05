#include "dab_fic_depuncture.h"


/* taken from openDAB */
/*sourceforge.net/projects/opendab/*/

int32_t dab_fic_depuncture(uint8_t *in, uint8_t *out)
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
            out[i+j*4+3] = 8;           
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
            out[i+j*4+3] = 8;
            offset+=3;
        }
        out[i+j*4+0] = in[offset+0];
        out[i+j*4+1] = in[offset+1];
        out[i+j*4+2] = 8;
        out[i+j*4+3] = 8;
        offset+=2;
    }
    for (j=0; j<6; j++)
    {
        out[i+j*4+0] = in[offset+0];
        out[i+j*4+1] = in[offset+1];
        out[i+j*4+2] = 8;
        out[i+j*4+3] = 8;
        offset+=2;
    }

   return 1;
}


