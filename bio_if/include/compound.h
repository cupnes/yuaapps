#pragma once

#include <element.h>
#include <common.h>

#define MAX_POOL_COMPOUNDS	100
#define MAX_COMPOUND_ELEMENTS	16

struct compound {
	struct singly_list list;
	element_t elements[MAX_COMPOUND_ELEMENTS];
	unsigned long long len;
};

extern struct compound compound_pool[MAX_POOL_COMPOUNDS];

void compound_pool_init(void);
struct compound *compound_alloc(void);
