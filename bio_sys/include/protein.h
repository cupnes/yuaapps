#pragma once

#include <lib.h>

#define MAX_POOL_PROTEINS	100

struct protein {
	/* Head */
	struct singly_list list;

	/* Compounds */
	struct compound *opcode;
	struct singly_list operand_head;

	/* Attributes */
	bool_t is_destroyed;
	bool_t is_completed;
};

extern struct protein protein_pool[MAX_POOL_PROTEINS];

void protein_pool_init(void);
struct protein *protein_create(void);
struct protein *protein_create_with_compounds(
	struct compound *opcode, struct singly_list *operand_1st_entry);
unsigned int protein_bond_compounds(struct protein *prot, unsigned char *buf);
void prot_decompose(struct protein *prot, struct singly_list *vessel_head);
void protein_dump(struct protein *prot);
void protein_dump_list(struct singly_list *prot_head);
