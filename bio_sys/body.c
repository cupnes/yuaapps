#include <organ.h>
#include <body.h>
#include <lib.h>

struct body body_pool[MAX_POOL_BODIES];
unsigned char body_pool_is_used[MAX_POOL_BODIES];
unsigned int is_body_creation;

void body_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_BODIES; i++)
		body_pool_is_used[i] = FALSE;
}

void body_run(struct body *body)
{
	/* 各器官で1周期分の生体活動を実施 */
	struct organ *orgn;
	for (orgn = body->orgn_list; orgn != NULL; orgn = orgn->list.next)
		organ_run(orgn);

	/* 次の周期まで待つ */
	sleep(BODY_CYCLE_US);
}
