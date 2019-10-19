#include <element.h>
#include <compound.h>
#include <protein.h>
#include <common.h>

#ifdef IS_SIM
#include <string.h>
#endif

#define MAX_COMBINED_ELEMENTS	100

struct protein protein_pool[MAX_POOL_PROTEINS];
unsigned char protein_pool_is_used[MAX_POOL_PROTEINS];
unsigned int is_protein_allocation;

void protein_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_PROTEINS; i++)
		protein_pool_is_used[i] = FALSE;
}

struct protein *protein_alloc(void)
{
	spin_lock(&is_protein_allocation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_PROTEINS; i++) {
		if (protein_pool_is_used[i] == FALSE) {
			protein_pool_is_used[i] = TRUE;
			spin_unlock(&is_protein_allocation);
			return &protein_pool[i];
		}
	}

	spin_unlock(&is_protein_allocation);
	return NULL;
}

unsigned int bond_compounds(struct protein *prot, unsigned char *buf)
{
	unsigned int len = 0;

	memcpy(buf, prot->opcode->elements, prot->opcode->len);
	len = prot->opcode->len;
	buf += len;

	struct compound *comp;
	for (comp = prot->operand_list; comp != NULL;
	     comp = comp->list.next) {
		memcpy(buf, comp->elements, comp->len);
		len += comp->len;
		buf += comp->len;
	}

	return len;
}
