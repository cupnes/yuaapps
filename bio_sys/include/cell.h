#pragma once

#include <protein.h>
#include <lib.h>

#define MAX_POOL_CELLS	100
#define MAX_DNA_LEN	64
#define MAX_CELL_ARGS	4
#define DEFAULT_LIFE_DURATION	3600

typedef unsigned char nucleotide_t;

struct cell {
	/* Head */
	struct singly_list list;
	unsigned long long life_duration;

	/* Protein */
	struct singly_list prot_head;
	struct singly_list prot_store_head;
	struct compound *args[MAX_CELL_ARGS];
	unsigned char num_args;
	bool_t is_can_react;
	void (*add_to_args_if_need)(struct cell *cell, struct compound *comp,
				    struct singly_list *vessel_head);

	/* DNA */

	/* Attributes */
	bool_t is_destroyed;
};

void cell_pool_init(void);
struct cell *cell_create(void);
void cell_run(struct cell *cell, struct singly_list *vessel_head);
void cell_update_status(struct cell *cell);
