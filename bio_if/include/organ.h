#pragma once

#include <protein.h>
#include <cell.h>

struct organ {
	/* Cells (Cell List) */
	struct cell *cells;

	/* Vessel (Protein List) */
	struct protein *vessel;
};
