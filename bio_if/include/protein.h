#pragma once

#include <common.h>

#define MAX_POOL_PROTEINS	100

struct protein {
	struct singly_list list;
	struct compound *opcode;
	struct compound *operand_list;
};

extern struct protein protein_pool[MAX_POOL_PROTEINS];

void protein_pool_init(void);
unsigned int bond_compounds(struct protein *prot, unsigned char *buf);
