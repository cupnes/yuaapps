#pragma once

#include <compound.h>
#include <lib.h>

struct protein {
	/* Head */
	struct singly_list list;
	bool_t in_use;

	/* Compounds */
	struct compound *opcode;
	struct singly_list operand_head;
};

void protein_pool_init(void);
void protein_dump_entry(struct protein *prot);
void protein_dump_list(struct singly_list *prot_head);
