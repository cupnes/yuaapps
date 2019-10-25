#pragma once

#include <cell.h>
#include <lib.h>

#define MAX_POOL_TISSUES	100

struct tissue {
	/* Head */
	struct singly_list list;

	/* Cells */
	struct singly_list cell_head;

	/* Attributes */
	bool_t is_destroyed;
};

void tissue_pool_init(void);
struct tissue *tissue_create_with_cell(struct cell *cell);
void tissue_run(struct tissue *tiss, struct singly_list *vessel_head);
