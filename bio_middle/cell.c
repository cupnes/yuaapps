#include <protein.h>
#include <cell.h>

#define MAX_POOL_CODONS	100
#define MAX_POOL_CELLS	100

static struct cell cell_pool[MAX_POOL_CELLS];
static unsigned int is_cell_creation;
static struct codon codn_pool[MAX_POOL_CODONS];
static unsigned int is_codn_creation;

static void init_cell(struct cell *cell)
{
	/* ヘッダ */
	cell->list.next = NULL;

	/* タンパク質 */
	cell->prot_head.next = NULL;

	/* DNA */
	cell->codn_head.next = NULL;

	/* 属性情報 */
	cell->life_duration = DEFAULT_LIFE_DURATION;
	cell->life_left = DEFAULT_LIFE_DURATION;
}

void cell_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_CODONS; i++)
		codn_pool[i].in_use = FALSE;
	for (i = 0; i < MAX_POOL_CELLS; i++)
		cell_pool[i].in_use = FALSE;
}

struct cell *cell_create(void)
{
	spin_lock(&is_cell_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_CELLS; i++) {
		if (cell_pool[i].in_use == FALSE) {
			cell_pool[i].in_use = TRUE;
			spin_unlock(&is_cell_creation);
			init_cell(&cell_pool[i]);
			return &cell_pool[i];
		}
	}

	spin_unlock(&is_cell_creation);
	return NULL;
}

struct codon *codon_create(void)
{
	spin_lock(&is_codn_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_CODONS; i++) {
		if (codn_pool[i].in_use == FALSE) {
			codn_pool[i].in_use = TRUE;
			spin_unlock(&is_codn_creation);
			codn_pool[i].list.next = NULL;
			codn_pool[i].comp_data = 0;
			codn_pool[i].buf = NULL;
			return &codn_pool[i];
		}
	}

	spin_unlock(&is_codn_creation);
	return NULL;
}

struct codon *codon_create_with_data(bio_data_t data)
{
	struct codon *codn = codon_create();
	if (codn == NULL)
		return NULL;

	codn->comp_data = data;
	return codn;
}

bool_t cell_run(struct cell *cell)
{
	/* 化合物リストから化合物を一つ取得 */

/*
if (何か化合物を取得できた?) then (yes)
  :代謝/運動|
  if (エラーが返された?) then (yes)
    :取得した化合物を化合物リストへ戻す;
  else (no)
    :代謝の生成物を化合物リストへ追加;
  endif
  :成長|
  if (エラーが返された?) then (yes)
    :取得した化合物を化合物リストへ戻す;
  else (no)

  endif
      1. (分裂可能が返された場合)
	 1. *増殖* を実施
      2. (成功した場合)
	 1. 特になし
      3. (エラーが返された場合)
	 1. 取得した化合物を環境へ放出する
3. 寿命を1周期減らす
   1. (寿命が0になった場合のみ実施)
      1. *死* を実施
	 - 自身を構成するタンパク質・化合物を化合物レベルまで分解して
           化合物リストへ追加する

*/
	return TRUE;
}

void cell_dump_entry(struct cell *cell)
{
	putchar('(');
	struct singly_list *entry;
	for (entry = cell->prot_head.next; entry != NULL; entry = entry->next) {
		protein_dump_entry((struct protein *)entry);
		if (entry->next != NULL)
			putchar(',');
	}
	putchar(')');
}

void cell_dump_list(struct singly_list *list_head)
{
	struct singly_list *entry;
	for (entry = list_head->next; entry != NULL; entry = entry->next) {
		cell_dump_entry((struct cell *)entry);
		if (entry->next != NULL)
			putchar(',');
	}
}
