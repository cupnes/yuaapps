#pragma once

#include <compound.h>
#include <cell.h>

struct organ {
	/* Cells (Cell List) */
	struct cell *cell_list;

	/* Vessel (Compound List) */
	struct compound *vessel;
};

void organ_run(struct organ *orgn);
