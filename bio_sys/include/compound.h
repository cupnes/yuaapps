#pragma once

#include <element.h>
#include <body.h>
#include <lib.h>

#define MAX_POOL_COMPOUNDS	100
#define MAX_COMPOUND_ELEMENTS	16

struct compound {
	/* Head */
	struct singly_list list;

	/* Elements */
	element_t elements[MAX_COMPOUND_ELEMENTS];
	unsigned long long len;

	/* Attributes */
	bool_t is_destroyed;
};

extern struct compound compound_pool[MAX_POOL_COMPOUNDS];

void compound_pool_init(void);
struct compound *compound_create(void);
struct compound *compound_create_with_elements(
	element_t *elem_arry, unsigned int elem_len);
struct compound *compound_create_with_data(bio_data_t data);
bool_t compound_is_data(struct compound *comp);
