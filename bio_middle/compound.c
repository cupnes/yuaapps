#include <compound.h>

#define MAX_POOL_COMPOUNDS	100

static struct compound compound_pool[MAX_POOL_COMPOUNDS];

void compound_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_COMPOUNDS; i++)
		compound_pool[i].in_use = FALSE;
}

void compound_dump_entry(struct compound *comp)
{
	char bracket_open, bracket_close;
	if (compound_is_data(comp) == TRUE) {
		bracket_open  = '[';
		bracket_close = ']';
	} else {
		bracket_open  = '<';
		bracket_close = '>';
	}

	putchar(bracket_open);
	unsigned long long i;
	for (i = 0; i < comp->len; i++) {
		puth(comp->elements.bytes[i], 2);
		if (i < (comp->len - 1))
			putchar(' ');
	}
	putchar(bracket_close);
}

bool_t compound_is_data(struct compound *comp)
{
	if (comp->len == sizeof(bio_data_t))
		return TRUE;
	return FALSE;
}

void compound_dump_list(struct singly_list *list_head, enum comp_filter filter)
{
	struct singly_list *entry;
	unsigned char is_first = TRUE;
	for (entry = list_head->next; entry != NULL; entry = entry->next) {
		struct compound *comp = (struct compound *)entry;

		if ((filter == COMP_FILTER_CODE)
		    && (compound_is_data(comp) == TRUE))
			continue;

		if ((filter == COMP_FILTER_DATA)
		    && (compound_is_data(comp) == FALSE))
			continue;

		if (is_first == TRUE)
			is_first = FALSE;
		else
			putchar(',');

		compound_dump_entry(comp);
	}
}
