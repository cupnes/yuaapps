#include <cell.h>
#include <organ.h>
#include <lib.h>

struct organ organ_pool[MAX_POOL_ORGANS];
unsigned int is_organ_creation;

void organ_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_ORGANS; i++)
		organ_pool[i].is_destroyed = TRUE;
}

struct organ *organ_create(void)
{
	spin_lock(&is_organ_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_ORGANS; i++) {
		if (organ_pool[i].is_destroyed == TRUE) {
			organ_pool[i].is_destroyed = FALSE;
			spin_unlock(&is_organ_creation);
			organ_pool[i].list.next = NULL;
			organ_pool[i].cell_list = NULL;
			organ_pool[i].vessel = NULL;
			return &organ_pool[i];
		}
	}

	spin_unlock(&is_organ_creation);
	return NULL;
}

struct organ *organ_create_with_cell(struct cell *cell_list)
{
	struct organ *orgn = organ_create();
	if (orgn == NULL)
		return NULL;

	orgn->cell_list = cell_list;
	return orgn;
}

void organ_run(struct organ *orgn)
{
	struct cell *cell;
	for (cell = orgn->cell_list; cell != NULL; cell = cell->list.next)
		cell_run(cell, orgn->vessel);

	/* TODO: DNAを実装できたら、組織の層を追加して
	 *       組織の層から細胞へ細胞分裂を指示するようにする
	 *       ∵ 細胞分裂のタイミングは組織で定義される */
}