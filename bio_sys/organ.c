#include <cell.h>
#include <organ.h>
#include <lib.h>

struct organ organ_pool[MAX_POOL_ORGANS];
unsigned char organ_pool_is_used[MAX_POOL_ORGANS];
unsigned int is_organ_creation;

void organ_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_ORGANS; i++)
		organ_pool_is_used[i] = FALSE;
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
