#include <cell.h>
#include <tissue.h>
#include <lib.h>

struct tissue tissue_pool[MAX_POOL_TISSUES];
unsigned int is_tissue_creation;

void tissue_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_TISSUES; i++)
		tissue_pool[i].is_destroyed = TRUE;
}

struct tissue *tissue_create(void)
{
	spin_lock(&is_tissue_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_TISSUES; i++) {
		if (tissue_pool[i].is_destroyed == TRUE) {
			tissue_pool[i].is_destroyed = FALSE;
			spin_unlock(&is_tissue_creation);
			tissue_pool[i].list.next = NULL;
			tissue_pool[i].cell_head.next = NULL;
			tissue_pool[i].check_dividable = NULL;
			return &tissue_pool[i];
		}
	}

	spin_unlock(&is_tissue_creation);
	return NULL;
}

struct tissue *tissue_create_with_cell(struct cell *cell_1st_entry)
{
	struct tissue *tiss = tissue_create();
	if (tiss == NULL)
		return NULL;

	tiss->cell_head.next = &cell_1st_entry->list;
	return tiss;
}

void tissue_run(struct tissue *tiss, struct singly_list *vessel_head)
{
	struct singly_list *cell, *next;
	for (cell = tiss->cell_head.next; cell != NULL; cell = next) {
		next = cell->next;

		struct cell *c = (struct cell *)cell;

		cell_run(c, vessel_head);

		struct cell *c_new = NULL;
		if (c->is_store_saturated == TRUE)
			c_new = cell_division(c);
		if (c_new != NULL)
			slist_prepend((struct singly_list *)c_new,
				      &tiss->cell_head);

		if (c->life_duration == 0) {
			cell_decompose(c, vessel_head);	/* Apotosis */
			slist_remove(cell, &tiss->cell_head);
		}
	}
}
