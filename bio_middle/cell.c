#include <protein.h>
#include <cell.h>

#define MAX_POOL_CODONS	100
#define MAX_POOL_CELLS	100

static struct cell cell_pool[MAX_POOL_CELLS];
static unsigned int is_cell_creation;
static struct codon codn_pool[MAX_POOL_CODONS];
static unsigned int is_codn_creation;

static void init_cell(struct cell *cell)
{
	/* ヘッダ */
	cell->list.next = NULL;

	/* タンパク質 */
	cell->prot_head.next = NULL;

	/* DNA */
	cell->codn_head.next = NULL;

	/* 属性情報 */
	cell->life_duration = DEFAULT_LIFE_DURATION;
	cell->life_left = DEFAULT_LIFE_DURATION;
}

void cell_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_CODONS; i++)
		codn_pool[i].in_use = FALSE;
	for (i = 0; i < MAX_POOL_CELLS; i++)
		cell_pool[i].in_use = FALSE;
}

struct cell *cell_create(void)
{
	spin_lock(&is_cell_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_CELLS; i++) {
		if (cell_pool[i].in_use == FALSE) {
			cell_pool[i].in_use = TRUE;
			spin_unlock(&is_cell_creation);
			init_cell(&cell_pool[i]);
			return &cell_pool[i];
		}
	}

	spin_unlock(&is_cell_creation);
	return NULL;
}

struct codon *codon_create(void)
{
	spin_lock(&is_codn_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_CODONS; i++) {
		if (codn_pool[i].in_use == FALSE) {
			codn_pool[i].in_use = TRUE;
			spin_unlock(&is_codn_creation);
			codn_pool[i].list.next = NULL;
			codn_pool[i].comp_data = 0;
			codn_pool[i].buf = NULL;
			return &codn_pool[i];
		}
	}

	spin_unlock(&is_codn_creation);
	return NULL;
}

struct codon *codon_create_with_data(bio_data_t data)
{
	struct codon *codn = codon_create();
	if (codn == NULL)
		return NULL;

	codn->comp_data = data;
	return codn;
}

void cell_dump_entry(struct cell *cell)
{
	putchar('(');
	struct singly_list *entry;
	for (entry = cell->prot_head.next; entry != NULL; entry = entry->next) {
		protein_dump_entry((struct protein *)entry);
		if (entry->next != NULL)
			putchar(',');
	}
	putchar(')');
}

void cell_dump_list(struct singly_list *list_head)
{
	struct singly_list *entry;
	for (entry = list_head->next; entry != NULL; entry = entry->next) {
		cell_dump_entry((struct cell *)entry);
		if (entry->next != NULL)
			putchar(',');
	}
}
