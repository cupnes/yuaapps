#include <cell.h>
#include <organ.h>
#include <lib.h>

void organ_run(struct organ *orgn)
{
	struct cell *cell;
	for (cell = orgn->cell_list; cell != NULL; cell = cell->list.next)
		cell_run(cell, orgn->vessel);

	/* TODO: DNAを実装できたら、組織の層を追加して
	 *       組織の層から細胞へ細胞分裂を指示するようにする
	 *       ∵ 細胞分裂のタイミングは組織で定義される */
}
