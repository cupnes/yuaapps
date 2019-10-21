#pragma once

#include <organ.h>

#define MAX_POOL_BODIES	100
#define BODY_CYCLE_US	1000000	/* 1,000,000us (1s) */

/* この生体の細胞が扱うデータの単位
 * (細胞として実装された関数の引数と戻り値の型) */
typedef unsigned long long bio_data_t;

struct body {
	struct organ *orgn_list;
};

void body_pool_init(void);
void body_run(struct body *body);
