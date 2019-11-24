#include <protein.h>
#include <cell.h>

#define MAX_POOL_CODONS	100
#define MAX_POOL_CELLS	100

static struct cell cell_pool[MAX_POOL_CELLS];
static struct codon codn_pool[MAX_POOL_CODONS];

void cell_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_CODONS; i++)
		codn_pool[i].is_use = FALSE;
	for (i = 0; i < MAX_POOL_CELLS; i++)
		cell_pool[i].in_use = FALSE;
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
