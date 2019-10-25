#pragma once

#include <compound.h>
#include <tissue.h>
#include <lib.h>

#define MAX_POOL_ORGANS	100

struct organ {
	/* Head */
	struct singly_list list;

	/* Organs (Organ List) */
	struct singly_list tiss_head;

	/* Vessel (Compound List) */
	struct singly_list vessel_head;

	/* Attributes */
	bool_t is_destroyed;
};

void organ_pool_init(void);
struct organ *organ_create(void);
struct organ *organ_create_with_tissue(struct tissue *tiss_1st_entry);
void organ_run(struct organ *orgn);
void organ_dump_vessel(struct organ *orgn);
