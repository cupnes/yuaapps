#include <lives/incrementer.h>
#include <body.h>

int main(void)
{
	/* インクリメンタの生体を生成 */
	struct body *incr = incrementer_create_body();

	/* 生体活動を開始 */
	body_run(incr);

	return 0;
}
