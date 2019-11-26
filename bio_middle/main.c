#include <protein.h>
#include <cell.h>
#include <lives/incrementer.h>

#define MAX_BIO_FILES	100
#define MAX_CYCLES	100
#define SLEEP_CYCLE_US	1000000	/* 1,000,000us (1s) */

#define BG_R	0
#define BG_G	0
#define BG_B	0

#define FG_R	255
#define FG_G	255
#define FG_B	255

#ifdef FULL_DEMO
#define PUT_INCR_COMP_CYCLE	10
#define VIRUS_INFECTION_CYCLE	30
#define FIRST_CELL_LIFE_DURATION	40
#else
#define PUT_INCR_COMP_CYCLE	50
#define VIRUS_INFECTION_CYCLE	60
#define FIRST_CELL_LIFE_DURATION	70
#endif

#define INITIAL_DATA	0

struct bio_env {
	struct singly_list cell_head;
	struct singly_list comp_head;
};

struct file *bio_files[MAX_BIO_FILES];

static void init_bio_env(struct bio_env *be);
static void dump_bio_status(struct bio_env *be, unsigned int cycle_num);
static void run_bio_cycle(struct bio_env *be);
static void run_bio_cycle_hook(struct bio_env *be, unsigned int cycle_num);

int main(void)
{
	/* 生体環境を初期化 */
	struct bio_env be;
	init_bio_env(&be);

	/* 背景色・描画色を設定 */
	set_bg(BG_R, BG_G, BG_B);
	set_fg(FG_R, FG_G, FG_B);

	unsigned int cycle_num;
	for (cycle_num = 0; cycle_num < MAX_CYCLES; cycle_num++) {
		/* 画面クリア */
		clear_screen();

		/* 現在の状態をダンプ */
		dump_bio_status(&be, cycle_num);

		/* 次の周期を待つ */
		sleep(SLEEP_CYCLE_US);

		/* 生体環境の1周期を実施 */
		run_bio_cycle(&be);

		/* 周期毎追加処理を呼び出す */
		run_bio_cycle_hook(&be, cycle_num);
	}

	return 0;
}

static void init_bio_env(struct bio_env *be)
{
	/* プール領域の初期化 */
	compound_pool_init();
	protein_pool_init();
	cell_pool_init();

	struct singly_list *entry;

	/* 環境に配置する生物を生成しリスト化 */
	entry = (struct singly_list *)incrementer_create();
	be->cell_head.next = entry;
	entry->next = NULL;

	/* 環境に配置する化合物を生成しリスト化 */
	entry = (struct singly_list *)compound_create_with_data(INITIAL_DATA);
	be->comp_head.next = entry;
	entry->next = NULL;
}

static void dump_bio_status(struct bio_env *be, unsigned int cycle_num)
{
	puts("    ### ");
	putd(cycle_num, 2);
	puts("th cycle ###\r\n");

	puts("\r\n");

	puts("Cells:\r\n");
	cell_dump_list(&be->cell_head);
	puts("\r\n");
	puts("\r\n");

	puts("Data Compounds:\r\n");
	compound_dump_list(&be->comp_head, COMP_FILTER_DATA);
	puts("\r\n");
	puts("\r\n");

	puts("Code Compounds:\r\n");
	compound_dump_list(&be->comp_head, COMP_FILTER_CODE);
}

static void run_bio_cycle(struct bio_env *be)
{
	/* unsigned int i; */

	/* /\* 生物ファイルを全て取得 *\/ */
	/* unsigned long long num_bio_files = */
	/* 	get_files_with_prefix(bio_files, 'b'); */

	/* /\* 化合物と実行ファイルをそれぞれリスト化 *\/ */
	/* struct singly_list comp_head, exec_head; */
	/* comp_head.next = exec_head.next = NULL; */
	/* for (i = 0; i < num_bio_files; i++) { */
	/* 	struct singly_list *entry; */
	/* 	if (bio_files[i]->name[1] == 'c') { */
	/* 		entry = (struct singly_list *) */
	/* 			bio_files[i]->data; */
	/* 		slist_prepend(entry, &comp_head); */
	/* 	} else if (bio_files[i]->name[1] == 'e') { */
	/* 		entry = (struct singly_list *) */
	/* 			bio_files[i]->data; */
	/* 		slist_prepend(entry, &exec_head); */
	/* 	} */
	/* } */

	/* /\* 生体実行ファイルを全て実行 *\/ */
}

static void run_bio_cycle_hook(struct bio_env *be, unsigned int cycle_num)
{
	/* switch (cycle_num) { */
	/* case PUT_INCR_COMP_CYCLE: */
	/* 	incrementer_create_essential_compounds(); */
	/* 	break; */

	/* case VIRUS_INFECTION_CYCLE: */
	/* 	break; */
	/* } */
}
