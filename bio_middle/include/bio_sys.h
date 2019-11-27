#pragma once

#include <compound.h>

struct compound *biosys_pop_compound(void);
void biosys_push_compound(struct compound *comp);
void biosys_push_cell(struct cell *cell);
void biosys_remove_cell(struct cell *cell);
