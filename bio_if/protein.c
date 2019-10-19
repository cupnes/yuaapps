#include <element.h>
#include <compound.h>
#include <protein.h>
#include <common.h>

#ifdef IS_SIM
#include <string.h>
#endif

#define MAX_COMBINED_ELEMENTS	100

struct protein protein_pool[MAX_POOL_PROTEINS];

void protein_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_PROTEINS; i++)
		protein_pool[i].list.next = NULL;
}
