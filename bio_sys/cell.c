#include <common.h>

#ifdef IS_SIM
#include <string.h>
#endif

#define BOND_BUF_SIZE	100

unsigned char bond_buf[BOND_BUF_SIZE];

void cell_create(struct cell *cell, nucleotide_t *dna, size_t dna_len)
{
	size_t i;
	for (i = 0; i < dna_len; i++)
		c->dna[i] = dna[i];
}

void cell_run(struct cell *cell, struct compound *vessel)
{
	/* 器官の管(vessel)に必要とする化合物があれば取得 */
	/* TODO: まずは引数となる値の取得のみ実装する */
	/* TODO: T.B.D: 成長と分裂のための化合物取得(要DNA実装) */

	/* タンパク質の反応に必要な化合物が揃っていれば化学反応を起こす */

	/* 化学反応を起こした場合、生成物を管(vessel)へ送出する */
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
