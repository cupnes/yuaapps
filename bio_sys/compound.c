#include <bio_type.h>
#include <compound.h>
#include <lib.h>

struct compound compound_pool[MAX_POOL_COMPOUNDS];
unsigned int is_compound_creation;

void compound_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_COMPOUNDS; i++)
		compound_pool[i].is_destroyed = TRUE;
}

struct compound *compound_create(void)
{
	spin_lock(&is_compound_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_COMPOUNDS; i++) {
		if (compound_pool[i].is_destroyed == TRUE) {
			compound_pool[i].is_destroyed = FALSE;
			spin_unlock(&is_compound_creation);
			compound_pool[i].list.next = NULL;
			compound_pool[i].len = 0;
			return &compound_pool[i];
		}
	}

	spin_unlock(&is_compound_creation);
	return NULL;
}

struct compound *compound_create_with_elements(
	element_t *elem_arry, unsigned int elem_len)
{
	struct compound *comp = compound_create();
	if (comp == NULL)
		return NULL;

	memcpy(comp->elements.bytes, elem_arry, elem_len);
	comp->len = elem_len;
	return comp;
}

struct compound *compound_create_with_data(bio_data_t data)
{
	struct compound *comp = compound_create();
	if (comp == NULL)
		return NULL;

	comp->elements.data = data;
	comp->len = sizeof(bio_data_t);
	return comp;
}

bool_t compound_is_data(struct compound *comp)
{
	if (comp->len == sizeof(bio_data_t))
		return TRUE;
	return FALSE;
}

void compound_dump_elements(struct compound *comp)
{
	putchar('[');
	unsigned long long i;
	for (i = 0; i < comp->len; i++) {
		puth(comp->elements.bytes[i], 2);
		if (i < (comp->len - 1))
			putchar(' ');
	}
	putchar(']');
}

void compound_dump(struct singly_list *list_head)
{
	struct singly_list *entry;
	for (entry = list_head->next; entry != NULL; entry = entry->next) {
		compound_dump_elements((struct compound *)entry);
		if (entry->next != NULL)
			putchar(',');
	}
	puts("\r\n");
}

bool_t compound_are_differ(struct compound *a, struct compound *b)
{
	if (a->elements.data != b->elements.data)
		return TRUE;
	return FALSE;
}

struct compound *compound_find_in(
	struct compound *comp, struct singly_list *list_head)
{
	struct singly_list *entry;
	for (entry = list_head->next; entry != NULL; entry = entry->next) {
		struct compound *t = (struct compound *)entry;
		if (compound_are_differ(t, comp) == FALSE)
			return t;
	}
	return NULL;
}
