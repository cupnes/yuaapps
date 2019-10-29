#pragma once

#include <bio_type.h>
#include <element.h>
#include <lib.h>

#define MAX_POOL_COMPOUNDS	100
#define MAX_COMPOUND_ELEMENTS	8

struct compound {
	/* Head */
	struct singly_list list;

	/* Elements */
	union {
		element_t bytes[MAX_COMPOUND_ELEMENTS];
		bio_data_t data;
	} elements;
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
void compound_dump_elements(struct compound *comp);
void compound_dump(struct singly_list *list_head);
bool_t compound_are_differ(struct compound *a, struct compound *b);
struct compound *compound_find_in(
	struct compound *comp, struct singly_list *list_head);
