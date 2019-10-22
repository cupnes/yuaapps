#pragma once

#include <compound.h>
#include <cell.h>
#include <lib.h>

#define MAX_POOL_ORGANS	100

struct organ {
	/* Head */
	struct singly_list list;

	/* Cells (Cell List) */
	struct cell *cell_list;

	/* Vessel (Compound List) */
	struct compound *vessel;

	/* Attributes */
	bool_t is_destroyed;
};

void organ_pool_init(void);
struct organ *organ_create(void);
struct organ *organ_create_with_cell(struct cell *cell_list);
void organ_run(struct organ *orgn);
