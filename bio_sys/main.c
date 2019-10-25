#include <compound.h>
#include <protein.h>
#include <cell.h>
#include <tissue.h>
#include <organ.h>
#include <body.h>
#include <lives/incrementer.h>

int main(void)
{
	/* 各種初期化処理を実施 */
	clear_screen();
	compound_pool_init();
	protein_pool_init();
	cell_pool_init();
	tissue_pool_init();
	organ_pool_init();
	body_pool_init();

	/* インクリメンタの生体を生成 */
	struct body *incr = incrementer_create_body();

	/* 生体活動を開始 */
	body_run(incr);

	return 0;
}
