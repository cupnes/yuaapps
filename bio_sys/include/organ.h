#pragma once

#include <compound.h>
#include <cell.h>

#define MAX_POOL_ORGANS	100

struct organ {
	/* Cells (Cell List) */
	struct cell *cell_list;

	/* Vessel (Compound List) */
	struct compound *vessel;
};

void organ_pool_init(void);
void organ_run(struct organ *orgn);
