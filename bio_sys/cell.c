#include <compound.h>
#include <protein.h>
#include <cell.h>
#include <lib.h>

#define BOND_BUF_SIZE	100

struct cell cell_pool[MAX_POOL_CELLS];
unsigned int is_cell_creation;
unsigned char bond_buf[BOND_BUF_SIZE];

static struct compound *react(struct cell *cell)
{
	struct compound *product = compound_create();
	if (product == NULL)
		return NULL;

	unsigned int idx = 0;
	struct singly_list *prot;
	for (prot = cell->prot_head.next; prot != NULL; prot = prot->next)
		idx += protein_bond_compounds((struct protein *)prot, &bond_buf[idx]);

	bio_data_t *t;

	bio_data_t (*func)(bio_data_t, bio_data_t, bio_data_t, bio_data_t);
	func = (void *)bond_buf;
	bio_data_t args[MAX_CELL_ARGS];
	unsigned int i;
	for (i = 0; i < MAX_CELL_ARGS; i++) {
		t = (bio_data_t *)cell->args[i]->elements;
		args[i] = *t;
	}
	bio_data_t prod_val = (*func)(args[1], args[2], args[3], args[4]);

	t = (bio_data_t *)product->elements;
	*t = prod_val;
	product->len = 8;

	return product;
}

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
			cell_pool[i].prot_head.next = NULL;
			cell_pool[i].prot_store_head.next = NULL;
			cell_pool[i].num_args = 0;
			cell_pool[i].is_can_react = FALSE;
			cell_pool[i].add_to_args_if_need = NULL;
			return &cell_pool[i];
		}
	}

	spin_unlock(&is_cell_creation);
	return NULL;
}

void cell_run(struct cell *cell, struct singly_list *vessel_head)
{
	/* 器官の管(vessel)に必要とする化合物があれば取得 */
	/* TODO: まずは引数となる値の取得のみ実装する */
	if ((cell->is_can_react == FALSE)
	    && (cell->add_to_args_if_need != NULL)) {
		struct singly_list *comp;
		for (comp = vessel_head->next; comp != NULL;
		     comp = comp->next) {
			cell->add_to_args_if_need(
				cell, (struct compound *)comp, vessel_head);
			if (cell->is_can_react == TRUE)
				break;
		}
	}
	/* TODO: T.B.D: 成長と分裂のための化合物取得(要DNA実装) */

	/* タンパク質の反応に必要な化合物が揃っているか？ */
	if (cell->is_can_react == TRUE) {
		/* 反応を起こす */
		struct compound *product = react(cell);

		/* 生成された化合物を管へ追加 */
		slist_prepend((struct singly_list *)product,
			      (struct singly_list *)vessel_head);
	}
}
