#pragma once

#include <protein.h>
#include <common.h>

#define MAX_DNA_LEN	64

typedef unsigned char nucleotide_t

struct cell {
	struct singly_list list;

	/* Protein */
	struct func_protein *fp_own;
	struct func_protein *fp_for_div;

	/* DNA */
	nucleotide_t dna[MAX_DNA_LEN];

	/* Nerve */

	/* Other */
	/* life_duration, ... etc */
};

void cell_create(struct cell *c, nucleotide_t *dna, size_t dna_len);
