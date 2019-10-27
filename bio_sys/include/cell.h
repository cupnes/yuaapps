#pragma once

#include <element.h>
#include <compound.h>
#include <protein.h>
#include <lib.h>

#define MAX_POOL_CELLS	100
/* #define MAX_DNA_LEN	64 */
#define MAX_CELL_ARGS	4
#define DEFAULT_LIFE_DURATION	3600

struct codon {
	/* Head */
	struct singly_list list;

	/* Data */
	bio_data_t comp_data;

	/* Attributes */
	bool_t is_stored;
};

struct cell {
	/* Head */
	struct singly_list list;
	unsigned long long life_duration;

	/* Protein */
	struct singly_list prot_head;
	struct singly_list prot_store_head;
	struct singly_list comp_store_head;
	struct compound *args[MAX_CELL_ARGS];
	unsigned char num_args;
	bool_t is_can_react;
	void (*add_to_args_if_need)(struct cell *cell, struct compound *comp,
				    struct singly_list *vessel_head);

	/* DNA */
	struct singly_list codon_head;
	bool_t is_store_saturated;    /* 化合物の蓄えが十分になった(分裂可能) */

	/* Attributes */
	bool_t is_destroyed;
};

void cell_pool_init(void);
struct cell *cell_create(void);
void cell_run(struct cell *cell, struct singly_list *vessel_head);
void cell_decompose(struct cell *cell, struct singly_list *vessel_head);
bool_t cell_is_needed_compound(struct cell *cell, struct compound *comp);
void cell_update_prot_stores(struct cell *cell);
