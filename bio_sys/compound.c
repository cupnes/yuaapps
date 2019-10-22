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

	memcpy(comp->elements, elem_arry, elem_len);
	comp->len = elem_len;
	return comp;
}

struct compound *compound_create_with_data(bio_data_t data)
{
	struct compound *comp = compound_create();
	if (comp == NULL)
		return NULL;

	bio_data_t *p = (bio_data_t *)comp->elements;
	*p = data;
	comp->len = sizeof(bio_data_t);
	return comp;
}

bool_t compound_is_data(struct compound *comp)
{
	if (comp->len == sizeof(bio_data_t))
		return TRUE;
	return FALSE;
}
