#pragma once

#include <protein.h>
#include <common.h>

#define MAX_DNA_LEN	64
#define MAX_CELL_ARGS	4

typedef unsigned char nucleotide_t

struct cell {
	/* Head */
	struct singly_list list;
	unsigned long long life_duration;

	/* Protein */
	struct protein *prot_list;
	struct protein *prot_store_list;
	struct compound *args[MAX_CELL_ARGS];

	/* DNA */
};

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
