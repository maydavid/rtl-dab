

int viterbi(unsigned char *ibuf, int ilen, unsigned char *obuf);
int encode(unsigned char *symbols,unsigned char *data,unsigned int nbytes,
	   unsigned int startstate,unsigned int endstate);
int init_viterbi();
