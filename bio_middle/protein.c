#include <protein.h>

#define MAX_POOL_PROTEINS	100

struct protein protein_pool[MAX_POOL_PROTEINS];

void protein_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_PROTEINS; i++)
		protein_pool[i].in_use = FALSE;
}

void protein_dump_entry(struct protein *prot)
{
	if (prot->comp_head.next == NULL)
		return;

	struct singly_list *entry;
	for (entry = prot->comp_head.next; entry != NULL; entry = entry->next) {
		compound_dump_entry((struct compound *)entry);
		if (entry->next != NULL)
			putchar('-');
	}
}

void protein_dump_list(struct singly_list *prot_head)
{
	struct singly_list *entry;
	for (entry = prot_head->next; entry != NULL; entry = entry->next) {
		protein_dump_entry((struct protein *)entry);
		if (entry->next != NULL)
			putchar(',');
	}
}
