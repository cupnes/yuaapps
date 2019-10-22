#pragma once

#include <protein.h>
#include <common.h>

#define MAX_POOL_CELLS	100
#define MAX_DNA_LEN	64
#define MAX_CELL_ARGS	4
#define DEFAULT_LIFE_DURATION	3600

typedef unsigned char nucleotide_t

struct cell {
	/* Head */
	struct singly_list list;
	unsigned long long life_duration;

	/* Protein */
	struct protein *prot_list;
	struct protein *prot_store_list;
	struct compound *args[MAX_CELL_ARGS];
	unsigned char num_args;
	bool_t is_can_react;
	void (*add_to_args_if_need)(
		struct compound *comp, struct compound *vessel);

	/* DNA */

	/* Attributes */
	bool_t is_destroyed;
};

void cell_pool_init(void);
void cell_create(struct cell *cell, nucleotide_t *dna, size_t dna_len);
void cell_run(struct cell *cell, struct compound *vessel);
void cell_update_status(struct cell *cell);
