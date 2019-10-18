#include <common.h>

void cell_create(struct cell *c, nucleotide_t *dna, size_t dna_len)
{
	size_t i;
	for (i = 0; i < dna_len; i++)
		c->dna[i] = dna[i];

	
}
