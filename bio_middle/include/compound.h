#pragma once

#include <lib.h>

#define MAX_COMPOUND_ELEMENTS	8

enum comp_filter {
	COMP_FILTER_NONE,
	COMP_FILTER_CODE,
	COMP_FILTER_DATA
};

struct compound {
	/* Head */
	struct singly_list list;
	bool_t in_use;

	/* Elements */
	union {
		element_t bytes[MAX_COMPOUND_ELEMENTS];
		bio_data_t data;
	} elements;
	unsigned long long len;
};

void compound_pool_init(void);
bool_t compound_is_data(struct compound *comp);
void compound_dump_entry(struct compound *comp);
void compound_dump_list(struct singly_list *list_head, enum comp_filter filter);
