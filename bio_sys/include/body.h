#pragma once

#include <organ.h>
#include <lib.h>

#define MAX_POOL_BODIES	10
#define BODY_CYCLE_US	1000000	/* 1,000,000us (1s) */

struct body {
	/* Organ */
	struct singly_list orgn_head;

	/* Attributes */
	bool_t is_destroyed;
	void (*init_func_hook)(struct body *body);
	void (*periodic_func_hook)(struct body *body);
};

void body_pool_init(void);
struct body *body_create(void);
struct body *body_create_with_organ(struct singly_list *orgn_1st_entry);
void body_init(struct body *body);
void body_run(struct body *body);
