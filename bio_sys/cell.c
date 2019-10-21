#include <compound.h>
#include <protein.h>
#include <cell.h>
#include <lib.h>

#define BOND_BUF_SIZE	100

struct cell cell_pool[MAX_POOL_CELLS];
unsigned int is_cell_creation;
unsigned char bond_buf[BOND_BUF_SIZE];

void cell_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_CELLS; i++)
		cell_pool[i].is_destroyed = TRUE;
}

struct cell *cell_create(void)
{
	spin_lock(&is_cell_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_CELLS; i++) {
		if (cell_pool[i].is_destroyed == TRUE) {
			cell_pool[i].is_destroyed = FALSE;
			spin_unlock(&is_cell_creation);
			cell_pool[i].list.next = NULL;
			cell_pool[i].life_duration = DEFAULT_LIFE_DURATION;
			cell_pool[i].prot_list = NULL;
			cell_pool[i].prot_store_list = NULL;
			cell_pool[i].num_args = 0;
			return &cell_pool[i];
		}
	}

	spin_unlock(&is_cell_creation);
	return NULL;
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
