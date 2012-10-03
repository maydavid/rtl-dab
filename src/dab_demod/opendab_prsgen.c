/*
  prsgen.c

  Copyright (C) 2005,2006,2007  David Crawley

  This file is part of OpenDAB.

  OpenDAB is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenDAB is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenDAB.  If not, see <http://www.gnu.org/licenses/>.
*/
/* Build Phase Reference Signal as in 
   sect. 14.3.2 of ETSI EN300 401 V1.3.3 (2001-05) */

/* Mode 1 only, for now */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>

#define N 2048

fftw_complex *fft_prs(fftw_complex*);
void ifft_prs(fftw_complex*,fftw_complex*);
void cplx_dump(char*, fftw_complex*, int);

/* From ETSI EN 300 401 V1.3.3 Sect.14.3.2 Table 48 */
int h[4][32] = {
	{ 0,2,0,0,0,0,1,1,2,0,0,0,2,2,1,1,
	  0,2,0,0,0,0,1,1,2,0,0,0,2,2,1,1 },

	{ 0,3,2,3,0,1,3,0,2,1,2,3,2,3,3,0,
	  0,3,2,3,0,1,3,0,2,1,2,3,2,3,3,0 },

	{ 0,0,0,2,0,2,1,3,2,2,0,2,2,0,1,3,
	  0,0,0,2,0,2,1,3,2,2,0,2,2,0,1,3 },

	{ 0,1,2,1,0,3,3,2,2,3,2,1,2,1,3,2,
	  0,1,2,1,0,3,3,2,2,3,2,1,2,1,3,2 }
};

/* From ETSI EN 300 401 V1.3.3 Sect.14.3.2 Table 44 */
int i[] = { 0,1,2,3,0,1,2,3,0,1,2,3,
	    0,1,2,3,0,1,2,3,0,1,2,3,
	    0,3,2,1,0,3,2,1,0,3,2,1,
	    0,3,2,1,0,3,2,1,0,3,2,1 };

int n[] = { 1,2,0,1,3,2,2,3,2,1,2,3,
	    1,2,3,3,2,2,2,1,1,3,1,2,
	    3,1,1,1,2,2,1,0,2,2,3,3,
	    0,2,1,3,3,3,3,0,3,0,1,1 };

int cospi2[] = { 1, 0, -1, 0 };  /* cos(0), cos(pi/2), cos(pi), cos(3*pi/2) */ 
int sinpi2[] = { 0, 1, 0, -1 };  /* sin(0), sin(pi/2), sin(pi), sin(3*pi/2) */

fftw_complex prs_syms[N];
fftw_complex prs_conj[N];
//fftw_complex *prs_syms_ifft;
//fftw_complex *prs_conj_ifft;

int prsgen(fftw_complex * prs_syms_ifft,fftw_complex * prs_conj_ifft,fftw_complex * prs_syms_ret)
{
	int k, kp, kpl, re, im;
	int j = 0;

	kpl = -768;
	for (k = -768; k < 769; k++) {
		if (k < 0)
			kp = (k % 32) ? k - 32 - k % 32 : k;
		else
			kp = (k % 32) ? k - k % 32 + 1 : k - 31;

		/* check */
		/* *(prs_syms + k + 768 + 255) = cexp(I*M_PI/2*(h[i[j]][k-kp]+n[j])); */

		re = cospi2[(h[i[j]][k-kp]+n[j]) % 4];
		im = sinpi2[(h[i[j]][k-kp]+n[j]) % 4];

		*(prs_syms + k + 768 + 255) = re + I*im;
		*(prs_conj + k + 768 + 255) = conj(*(prs_syms + k + 768 + 255));

		if (kp != kpl && k != 0) {
			kpl = kp;
			j++;
		}
		/* For debugging... */
		/*    printf("k = %d,  k' = %d, j = %d, i[j] = %d, n[j] = %d, h[i[j]][k-kp] = %d, h[i[j]][k-kp]+n[j] = %d = %d\n",k,kp,j,i[j],n[j],h[i[j]][k-kp],h[i[j]][k-kp]+n[j],(h[i[j]][k-kp]+n[j])%4); */

	}
	*(prs_syms + 768 + 255) = 0 + 0j;  /* explicitly set value for k = 0 */
	*(prs_conj + 768 + 255) = 0 + 0j;

	ifft_prs(prs_syms,prs_syms_ifft);
	ifft_prs(prs_conj,prs_conj_ifft);

	for (k=0;k<N;k++)
	  prs_syms_ret[k] = prs_syms[k];
	//for (k=0;k<2048;k++)
	//  fprintf(stderr,"%f ",&prs_syms[0][1]);

	//cplx_dump("prs_syms.gplot",prs_syms,N);
	//cplx_dump("prs_conj.gplot",prs_conj,N);
	//cplx_dump("prs_syms_ifft.gplot",prs_syms_ifft,N);
	//cplx_dump("prs_conj_ifft.gplot",prs_conj_ifft,N);

	//exit(EXIT_SUCCESS);
	return 1;
}

fftw_complex *fft_prs(fftw_complex *in)
{
	fftw_complex *out;
	fftw_plan p;

	out = fftw_malloc(sizeof(fftw_complex) * (N + 32));
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);

	return(out);
}

void ifft_prs(fftw_complex *in,fftw_complex *out)
{
  //fftw_complex *out;
	fftw_plan p;

	//out = fftw_malloc(sizeof(fftw_complex) * (N + 32));
	p = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);

	//return(out);
}

/* Dump vector of fttw_complex to file suitable for gnuplot */

void cplx_dump(char* fname, fftw_complex* vect, int pts)
{
	FILE *dfp;
	int j;
	double re, im;

	if ((dfp = fopen(fname, "w")) == NULL) {
		fprintf(stderr,"cplx_dump: Couldn't open output file\n");
		exit(EXIT_FAILURE);
	}

	fputs("# Real part\n",dfp);
	for (j=0; j < pts; j++) {
		re = creal(*(vect+j));
		fprintf(dfp,"%.17e\n",re);
	}

	fputs("\n\n# Imaginary part\n",dfp);
	for (j=0; j < pts; j++) {
		im = cimag(*(vect+j));
		fprintf(dfp,"%.17e\n",im);
	}

	fclose(dfp);
}

