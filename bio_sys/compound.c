#include <compound.h>
#include <common.h>

struct compound compound_pool[MAX_POOL_COMPOUNDS];
unsigned char compound_pool_is_used[MAX_POOL_COMPOUNDS];
unsigned int is_compound_allocation;

void compound_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_COMPOUNDS; i++)
		compound_pool_is_used[i] = FALSE;
}

struct compound *compound_alloc(void)
{
	spin_lock(&is_compound_allocation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_COMPOUNDS; i++) {
		if (compound_pool_is_used[i] == FALSE) {
			compound_pool_is_used[i] = TRUE;
			spin_unlock(&is_compound_allocation);
			return &compound_pool[i];
		}
	}

	spin_unlock(&is_compound_allocation);
	return NULL;
}
