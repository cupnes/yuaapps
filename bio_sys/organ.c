#include <compound.h>
#include <tissue.h>
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
			organ_pool[i].tiss_head.next = NULL;
			organ_pool[i].vessel_head.next = NULL;
			return &organ_pool[i];
		}
	}

	spin_unlock(&is_organ_creation);
	return NULL;
}

struct organ *organ_create_with_tissue(struct tissue *tiss_1st_entry)
{
	struct organ *orgn = organ_create();
	if (orgn == NULL)
		return NULL;

	orgn->tiss_head.next = &tiss_1st_entry->list;
	return orgn;
}

void organ_run(struct organ *orgn)
{
	struct singly_list *tiss;
	for (tiss = orgn->tiss_head.next; tiss != NULL; tiss = tiss->next)
		tissue_run((struct tissue *)tiss, &orgn->vessel_head);

	/* TODO: DNAを実装できたら、組織の層を追加して
	 *       組織の層から細胞へ細胞分裂を指示するようにする
	 *       ∵ 細胞分裂のタイミングは組織で定義される */
}

void organ_dump_vessel(struct organ *orgn)
{
	struct singly_list *comp;
	for (comp = orgn->vessel_head.next; comp != NULL; comp = comp->next) {
		struct compound *c = (struct compound *)comp;
		compound_dump_elements(c);
		if (comp->next != NULL)
			puts(", ");
	}
	puts("\r\n");
}
