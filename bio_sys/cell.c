#include <bio_type.h>
#include <compound.h>
#include <protein.h>
#include <cell.h>
#include <lib.h>

#define BOND_BUF_SIZE	100
#define MAX_POOL_CODONS	100

struct cell cell_pool[MAX_POOL_CELLS];
unsigned int is_cell_creation;
struct codon codon_pool[MAX_POOL_CODONS];
unsigned int is_codon_creation;
unsigned char bond_buf[BOND_BUF_SIZE];

static struct compound *react(struct cell *cell)
{
	struct compound *product = compound_create();
	if (product == NULL)
		return NULL;

	unsigned int idx = 0;
	struct singly_list *prot;
	for (prot = cell->prot_head.next; prot != NULL; prot = prot->next)
		idx += protein_bond_compounds((struct protein *)prot, &bond_buf[idx]);

	bio_data_t (*func)(bio_data_t, bio_data_t, bio_data_t, bio_data_t);
	func = (void *)bond_buf;
	bio_data_t args[MAX_CELL_ARGS];
	unsigned int i;
	for (i = 0; i < MAX_CELL_ARGS; i++)
		args[i] = cell->args[i]->elements.data;
	bio_data_t prod_val = (*func)(args[0], args[1], args[2], args[3]);

	for (i = 0; i < MAX_CELL_ARGS; i++)
		cell->args[i]->is_destroyed = TRUE;
	cell->is_can_react = FALSE;

	product->elements.data = prod_val;
	product->len = 8;

	return product;
}

static void init_cell(struct cell *cell)
{
	/* Head */
	cell->list.next = NULL;
	cell->life_duration = DEFAULT_LIFE_DURATION;
	/* cell->life_duration_def = DEFAULT_LIFE_DURATION; */

	/* Protein */
	cell->prot_head.next = NULL;
	cell->prot_store_head.next = NULL;
	cell->comp_store_head.next = NULL;
	unsigned char i;
	for (i = 0; i < MAX_CELL_ARGS; i++)
		cell->args[i] = NULL;
	cell->num_args = 0;
	cell->is_can_react = FALSE;
	cell->add_to_args_if_need = NULL;

	/* DNA */
	cell->codon_head.next = NULL;
	cell->is_store_saturated = FALSE;
}

/* 取り込めるものがあったら一つだけ取り込む
 * (1周期で取り込める化合物は1つ)		*/
static bool_t store_compound_if_need(
	struct cell *cell, struct compound *comp,
	struct singly_list *vessel_head)
{
	struct singly_list *codn;
	for (codn = cell->codon_head.next; codn != NULL; codn = codn->next) {
		struct codon *c = (struct codon *)codn;
		if (c->is_stored == FALSE) {
			if (comp->elements.data == c->comp_data) {
				struct singly_list *entry =
					slist_remove(&comp->list, vessel_head);
				if (entry == NULL)
					die("can't remove a compound.");
				slist_prepend(entry, &cell->comp_store_head);

				c->is_stored = TRUE;

				return TRUE;
			}
		}
	}

	return FALSE;
}

static struct codon *codon_create(void)
{
	spin_lock(&is_codon_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_CODONS; i++) {
		if (codon_pool[i].is_destroyed == TRUE) {
			codon_pool[i].is_destroyed = FALSE;
			spin_unlock(&is_codon_creation);

			codon_pool[i].list.next = NULL;
			codon_pool[i].comp_data = 0;
			codon_pool[i].is_stored = FALSE;

			return &codon_pool[i];
		}
	}

	spin_unlock(&is_codon_creation);
	return NULL;
}

static struct singly_list *copy_codon_list(struct singly_list *codon_head)
{
	struct singly_list *new_entry_1st = NULL;
	struct codon *new_codon = NULL;
	struct singly_list *new_entry_prev = NULL;

	struct singly_list *orig_entry;
	struct codon *orig_codon;

	for (orig_entry = codon_head->next; orig_entry != NULL;
	     orig_entry = orig_entry->next) {
		new_codon = codon_create();
		if (new_codon == NULL)
			die("copy_codon_list: can't create codon.");

		orig_codon = (struct codon *)orig_entry;
		new_codon->comp_data = orig_codon->comp_data;

		if (new_entry_1st == NULL)
			new_entry_1st = &new_codon->list;
		else
			new_entry_prev->next = &new_codon->list;

		new_entry_prev = &new_codon->list;
	}

	return new_entry_1st;
}

void cell_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_CELLS; i++)
		cell_pool[i].is_destroyed = TRUE;
	for (i = 0; i < MAX_POOL_CODONS; i++)
		codon_pool[i].is_destroyed = TRUE;
}

struct cell *cell_create(void)
{
	spin_lock(&is_cell_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_CELLS; i++) {
		if (cell_pool[i].is_destroyed == TRUE) {
			cell_pool[i].is_destroyed = FALSE;
			spin_unlock(&is_cell_creation);
			init_cell(&cell_pool[i]);
			return &cell_pool[i];
		}
	}

	spin_unlock(&is_cell_creation);
	return NULL;
}

/* static void dump_vessel(char *pref, struct singly_list *vessel_head) */
/* { */
/* 	puts(pref); */
/* 	putchar(' '); */
/* 	struct singly_list *comp; */
/* 	for (comp = vessel_head->next; comp != NULL; comp = comp->next) { */
/* 		struct compound *c = (struct compound *)comp; */
/* 		compound_dump_elements(c); */
/* 		if (comp->next != NULL) */
/* 			puts(", "); */
/* 	} */
/* 	puts("\r\n"); */
/* } */

void cell_run(struct cell *cell, struct singly_list *vessel_head)
{
	struct singly_list *comp;

	/* dump_vessel("A", vessel_head); */

	/* 器官の管(vessel)に必要とする化合物があれば取得 */
	/* TODO: まずは引数となる値の取得のみ実装する */
	if ((cell->is_can_react == FALSE)
	    && (cell->add_to_args_if_need != NULL)) {
		for (comp = vessel_head->next; comp != NULL;
		     comp = comp->next) {
			cell->add_to_args_if_need(
				cell, (struct compound *)comp, vessel_head);
			if (cell->is_can_react == TRUE)
				break;
		}
	}

	/* dump_vessel("B", vessel_head); */

	/* タンパク質の反応に必要な化合物が揃っていれば反応を起こす */
	if (cell->is_can_react == TRUE) {
		/* 反応を起こす */
		struct compound *product = react(cell);

		/* 生成された化合物を管へ追加 */
		slist_prepend((struct singly_list *)product,
			      (struct singly_list *)vessel_head);
	}

	/* dump_vessel("C", vessel_head); */

	/* 成長する(自身を構成する化合物が管にあれば取り込む)
	 * (1周期につき1つ)					*/
	struct singly_list *next;
	for (comp = vessel_head->next; comp != NULL; comp = next) {
		next = comp->next;

		bool_t stored = store_compound_if_need(
			cell, (struct compound *)comp, vessel_head);
		if (stored == TRUE)
			break;
	}
	/* puts("- "); */
	/* compound_dump(&cell->comp_store_head); */
	cell_update_prot_stores(cell);
	/* if (cell->is_store_saturated == TRUE) { */
	/* 	puts("cell->is_store_saturated == TRUE\r\n"); */
	/* } */

	/* dump_vessel("D", vessel_head); */

	/* 寿命を減らす */
	cell->life_duration--;
}

void cell_decompose(struct cell *cell, struct singly_list *vessel_head)
{
	struct singly_list *prot;
	for (prot = cell->prot_head.next; prot != NULL; prot = prot->next)
		prot_decompose((struct protein *)prot, vessel_head);
	for (prot = cell->prot_store_head.next; prot != NULL; prot = prot->next)
		prot_decompose((struct protein *)prot, vessel_head);

	struct singly_list *comp, *next;
	for (comp = cell->comp_store_head.next; comp != NULL; comp = next) {
		next = comp->next;
		slist_prepend(comp, vessel_head);
	}
	cell->comp_store_head.next = NULL;
}

void cell_update_prot_stores(struct cell *cell)
{
	if ((cell->is_store_saturated == TRUE)
	    || (cell->comp_store_head.next == NULL))
		return;

	struct singly_list *codn;
	for (codn = cell->codon_head.next; codn != NULL; codn = codn->next) {
		struct codon *c = (struct codon *)codn;
		if (c->is_stored == FALSE)
			return;
	}

	/* comp_store_head で prot_store_head を作る */
	struct protein *prot_1st_entry = NULL;
	struct protein *prot_prev = NULL;
	struct protein *prot_product;
	struct singly_list *prot;
	for (prot = cell->prot_head.next; prot != NULL; prot = prot->next) {
		struct singly_list *_t;

		struct protein *p = (struct protein *)prot;

		/* protein_dump(p); */
		/* putchar('-'); */

		/* opcodeをcomp_store_headのリストから取得 */
		struct compound *comp_opcode = compound_find_in(
			p->opcode, &cell->comp_store_head);
		if (comp_opcode == NULL)
			die("cell_update_prot_stores: opcode not found.");
		_t = slist_remove(&comp_opcode->list, &cell->comp_store_head);
		if (_t == NULL)
			die("cell_update_prot_stores: opcode can't remove.");
		_t->next = NULL;

		/* operandのリストをcomp_store_headのリストから作成 */
		struct singly_list *comp;
		struct compound *comp_operand_1st_entry = NULL;
		struct singly_list *_t_prev = NULL;
		struct singly_list *next;
		for (comp = p->operand_head.next; comp != NULL; comp = next) {
			next = comp->next;

			struct compound *comp_operand = compound_find_in(
				(struct compound *)comp,
				&cell->comp_store_head);
			if (comp_operand == NULL)
				die("cell_update_prot_sto: operand not found.");
			_t = slist_remove(&comp_operand->list,
					  &cell->comp_store_head);
			if (_t == NULL)
				die("cell_update_prot_: operand can't remove.");

			if (_t_prev != NULL)
				_t_prev->next = _t;
			else
				comp_operand_1st_entry = comp_operand;

			_t_prev = _t;
		}
		_t->next = NULL;

		/* opcodeとoperandのリストでproteinを生成 */
		prot_product = protein_create_with_compounds(
			comp_opcode, &comp_operand_1st_entry->list);
		/* protein_dump(prot_product); */
		/* putchar('-'); */

		if (prot_prev != NULL)
			prot_prev->list.next = &prot_product->list;
		else
			prot_1st_entry = prot_product;

		prot_prev = prot_product;

		/* struct singly_list dump_list_head; */
		/* dump_list_head.next = &prot_1st_entry->list; */
		/* protein_dump_list(&dump_list_head); */
	}
	prot_product->list.next = NULL;

	cell->prot_store_head.next = &prot_1st_entry->list;
	cell->is_store_saturated = TRUE;
}

struct cell *cell_division(struct cell *cell)
{
	if ((cell == NULL) || (cell->is_store_saturated != TRUE))
		return NULL;

	struct cell *cell_new = cell_create();
	if (cell_new == NULL)
		return NULL;

	/* cell_new->life_duration_def = cell->life_duration_def; */

	cell_new->prot_head.next = cell->prot_store_head.next;
	cell->prot_store_head.next = NULL;
	struct singly_list *codn;
	for (codn = cell->codon_head.next; codn != NULL; codn = codn->next) {
		struct codon *c = (struct codon *)codn;
		c->is_stored = FALSE;
	}
	cell->is_store_saturated = FALSE;

	cell_new->num_args = cell->num_args;
	cell_new->add_to_args_if_need = cell->add_to_args_if_need;

	cell_new->codon_head.next = copy_codon_list(&cell->codon_head);
	if (cell_new->codon_head.next == NULL)
		die("cell_division: codon copy failure.");

	/* protein_dump_list(&cell_new->prot_head); */

	return cell_new;
}
