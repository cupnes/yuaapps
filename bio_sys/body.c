#include <organ.h>
#include <body.h>
#include <lib.h>

#define BUSY_LOOP_CYCLES	(BODY_CYCLE_US * 100)

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
			body_pool[i].init_func_hook = NULL;
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

void body_dump_status(struct body *body)
{
	clear_screen();

	static unsigned int cycle_num = 0;
	puts("    ### ");
	putd(cycle_num++, 2);
	puts("th cycle ###\r\n\r\n");

	struct organ *orgn = (struct organ *)body->orgn_head.next;
	struct tissue *tiss = (struct tissue *)orgn->tiss_head.next;

	puts("Cells:\r\n");
	struct singly_list *entry;
	for (entry = tiss->cell_head.next; entry != NULL; entry = entry->next) {
		cell_dump_entry((struct cell *)entry);
		puts("\r\n");
	}
	puts("\r\n");

	puts("Data Compounds:\r\n");
	compound_dump_list(&orgn->vessel_head, COMP_FILTER_DATA);
	puts("\r\n\r\n");

	puts("Code Compounds:\r\n");
	compound_dump_list(&orgn->vessel_head, COMP_FILTER_CODE);
	puts("\r\n");
}

void body_init(struct body *body)
{
	if (body->init_func_hook != NULL)
		body->init_func_hook(body);

	/* body_dump_status(body); */

	unsigned long long _wait = BUSY_LOOP_CYCLES;
	while (_wait--);
}

void body_run(struct body *body)
{
	while (TRUE) {
		struct organ *orgn = (struct organ *)body->orgn_head.next;

		/* 各器官で1周期分の生体活動を実施 */
		for (orgn = (struct organ *)body->orgn_head.next; orgn != NULL;
		     orgn = (struct organ *)orgn->list.next)
			organ_run(orgn);

		/* 生体側でフック関数が登録されていれば実行 */
		if (body->periodic_func_hook != NULL)
			body->periodic_func_hook(body);

		/* body_dump_status(body); */

		/* 次の周期まで待つ */
		/* sleep(BODY_CYCLE_US); */
		unsigned long long _wait = BUSY_LOOP_CYCLES;
		while (_wait--);
	}
}
