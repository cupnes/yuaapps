#include <organ.h>
#include <body.h>
#include <lib.h>

void body_run(struct body *body)
{
	/* 各器官で1周期分の生体活動を実施 */
	struct organ *orgn;
	for (orgn = body->orgn_list; orgn != NULL; orgn = orgn->list.next)
		organ_run(orgn);

	/* 次の周期まで待つ */
	sleep(BODY_CYCLE_US);
}
