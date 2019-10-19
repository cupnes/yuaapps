#include <common.h>

#ifdef IS_SIM
#include <string.h>
#endif

#define BOND_BUF_SIZE	100

unsigned char bond_buf[BOND_BUF_SIZE];

void cell_create(struct cell *c, nucleotide_t *dna, size_t dna_len)
{
	size_t i;
	for (i = 0; i < dna_len; i++)
		c->dna[i] = dna[i];

	
}

struct compound *reaction(struct cell *cell)
{
	struct compound *product = compound_alloc();
	if (product == NULL)
		return NULL;

	unsigned int idx = 0;
	struct protein *t_prot;
	for (t_prot = cell->prot_list; t_prot != NULL;
	     t_prot = t_prot->list.next)
		idx += bond_compounds(t_prot, &bond_buf[idx]);

	data_t *t;

	data_t (*func)(data_t, data_t, data_t, data_t);
	func = (void *)bond_buf;
	data_t args[MAX_CELL_ARGS];
	unsigned int i;
	for (i = 0; i < MAX_CELL_ARGS; i++) {
		t = (data_t *)cell->args[i]->elements;
		args[i] = *t;
	}
	data_t prod_val = (*func)(args[1], args[2], args[3], args[4]);

	t = (data_t *)product->elements;
	*t = prod_val;
	product->len = 8;

	return product;
}
