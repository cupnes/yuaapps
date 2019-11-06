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
			body_pool[i].periodic_func_hook = NULL;
			return &body_pool[i];
		}
	}

	spin_unlock(&is_body_creation);
	return NULL;
}

struct body *body_create_with_organ(struct singly_list *orgn_1st_entry)
{
	struct body *body = body_create();
	if (body == NULL)
		return NULL;

	body->orgn_head.next = orgn_1st_entry;
	return body;
}

void body_run(struct body *body)
{
	unsigned int cycle_num = 0;

	clear_screen();
	puts("\r\n    ");
	putd(cycle_num++, 2);
	puts("th cycle\r\n\r\n");
	struct organ *orgn = (struct organ *)body->orgn_head.next;
	organ_dump_vessel(orgn);

	unsigned long long _wait = BODY_CYCLE_US * 300;
	while (_wait--);

	while (TRUE) {
		clear_screen();
		puts("\r\n    ");
		putd(cycle_num++, 2);
		puts("th cycle\r\n\r\n");

		/* struct organ *orgn = (struct organ *)body->orgn_head.next; */
		/* organ_dump_vessel(orgn); */

		/* 各器官で1周期分の生体活動を実施 */
		for (orgn = (struct organ *)body->orgn_head.next; orgn != NULL;
		     orgn = (struct organ *)orgn->list.next) {
			organ_run(orgn);

			/* デバッグ用に管の情報をダンプ */
			organ_dump_vessel(orgn);
		}

		if (body->periodic_func_hook != NULL)
			body->periodic_func_hook(body);

		/* 次の周期まで待つ */
		/* sleep(BODY_CYCLE_US); */
		unsigned long long _wait = BODY_CYCLE_US * 300;
		while (_wait--);
	}
}
