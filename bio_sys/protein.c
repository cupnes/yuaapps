#include <element.h>
#include <compound.h>
#include <protein.h>
#include <lib.h>

#define MAX_COMBINED_ELEMENTS	100

struct protein protein_pool[MAX_POOL_PROTEINS];
unsigned int is_protein_creation;

void protein_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_PROTEINS; i++)
		protein_pool[i].is_destroyed = TRUE;
}

struct protein *protein_create(void)
{
	spin_lock(&is_protein_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_PROTEINS; i++) {
		if (protein_pool[i].is_destroyed == TRUE) {
			protein_pool[i].is_destroyed = FALSE;
			spin_unlock(&is_protein_creation);
			protein_pool[i].list.next = NULL;
			protein_pool[i].opcode = NULL;
			protein_pool[i].operand_list = NULL;
			return &protein_pool[i];
		}
	}

	spin_unlock(&is_protein_creation);
	return NULL;
}

struct protein *protein_create_with_compounds(
	struct compound *opcode, struct compound *operand_list)
{
	struct protein *prot = protein_create();
	if (prot == NULL)
		return NULL;

	prot->opcode = opcode;
	prot->operand_list = operand_list;
	return prot;
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
