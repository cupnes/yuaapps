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
			protein_pool[i].operand_head.next = NULL;
			return &protein_pool[i];
		}
	}

	spin_unlock(&is_protein_creation);
	return NULL;
}

struct protein *protein_create_with_compounds(
	struct compound *opcode, struct singly_list *operand_1st_entry)
{
	struct protein *prot = protein_create();
	if (prot == NULL)
		return NULL;

	prot->opcode = opcode;
	prot->operand_head.next = operand_1st_entry;
	return prot;
}

unsigned int protein_bond_compounds(struct protein *prot, unsigned char *buf)
{
	unsigned int len = 0;

	memcpy(buf, prot->opcode->elements, prot->opcode->len);
	len = prot->opcode->len;
	buf += len;

	struct singly_list *comp;
	for (comp = prot->operand_head.next; comp != NULL; comp = comp->next) {
		struct compound *c = (struct compound *)comp;
		memcpy(buf, c->elements, c->len);
		len += c->len;
		buf += c->len;
	}

	return len;
}
