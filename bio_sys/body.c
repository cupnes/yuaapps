#include <organ.h>
#include <body.h>
#include <lib.h>

struct body body_pool[MAX_POOL_BODIES];
unsigned int is_body_creation;

void body_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_BODIES; i++)
		body_pool[i].is_destroyed = TRUE;
}

struct body *body_create(void)
{
	spin_lock(&is_body_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_BODIES; i++) {
		if (body_pool[i].is_destroyed == TRUE) {
			body_pool[i].is_destroyed = FALSE;
			spin_unlock(&is_body_creation);
			body_pool[i].orgn_head.next = NULL;
			return &body_pool[i];
		}
	}

	spin_unlock(&is_body_creation);
	return NULL;
}

struct body *body_create_with_organ(struct organ *orgn_1st_entry)
{
	struct body *body = body_create();
	if (body == NULL)
		return NULL;

	body->orgn_head.next = &orgn_1st_entry->list;
	return body;
}

void body_run(struct body *body)
{
	while (TRUE) {
		/* 各器官で1周期分の生体活動を実施 */
		struct organ *orgn;
		for (orgn = (struct organ *)body->orgn_head.next; orgn != NULL;
		     orgn = (struct organ *)orgn->list.next)
			organ_run(orgn);

		/* 次の周期まで待つ */
		sleep(BODY_CYCLE_US);
	}
}
