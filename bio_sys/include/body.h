#pragma once

#include <organ.h>
#include <lib.h>

#define MAX_POOL_BODIES	10
#define BODY_CYCLE_US	1000000	/* 1,000,000us (1s) */

/* この生体の細胞が扱うデータの単位
 * (細胞として実装された関数の引数と戻り値の型) */
typedef unsigned long long bio_data_t;

struct body {
	/* Organ */
	struct singly_list orgn_head;

	/* Attributes */
	bool_t is_destroyed;
};

void body_pool_init(void);
struct body *body_create(void);
struct body *body_create_with_organ(struct singly_list *orgn_1st_entry);
void body_run(struct body *body);
