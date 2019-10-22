#pragma once

#include <lib.h>

#define MAX_POOL_PROTEINS	100

struct protein {
	/* Head */
	struct singly_list list;

	/* Compounds */
	struct compound *opcode;
	struct compound *operand_list;

	/* Attributes */
	bool_t is_destroyed;
};

extern struct protein protein_pool[MAX_POOL_PROTEINS];

void protein_pool_init(void);
struct protein *protein_create(void);
struct protein *protein_create_with_compounds(
	struct compound *opcode, struct compound *operand_list);
unsigned int protein_bond_compounds(struct protein *prot, unsigned char *buf);
