#include <bio_type.h>
#include <element.h>
#include <compound.h>
#include <protein.h>
#include <cell.h>
#include <tissue.h>
#include <organ.h>
#include <body.h>
#include <lib.h>

#define INCREMENTER_MAX_COMPOUNDS	5
#ifdef FULL_DEMO
#define PUT_INCR_COMP_TH	20
#define VIRUS_INFECTION_TH	40
#define FIRST_CELL_LIFE_DURATION	50
#else
#define PUT_INCR_COMP_TH	50
#define VIRUS_INFECTION_TH	60
#define FIRST_CELL_LIFE_DURATION	70
#endif
#define INITIAL_DATA	0

/* インクリメンタ細胞の機械語コード
protein1: opcode=0x48 0x89, operand=0xf8	mov %rdi,%rax
protein2: opcode=0x48 0xff, operand=0xc0	inc %rax
protein3: opcode=0xc3     , operand=NULL	ret
 */

struct codon codon_data[INCREMENTER_MAX_COMPOUNDS];
struct compound *data_compound;
unsigned int incr_cycle_num;

static void put_incrementer_compounds(struct body *body)
{
	element_t elem_opcode[2], elem_operand[1];
	struct compound *comp_opcode, *comp_operand;
	struct singly_list incr_comp_head;

	/* opcode1=0x48 0x89 */
	elem_opcode[0] = 0x48; elem_opcode[1] = 0x89;
	comp_opcode = compound_create_with_elements(elem_opcode, 2);
	if (comp_opcode == NULL)
		die("put_incrementer_compounds: can't create opcode1.");
	incr_comp_head.next = &comp_opcode->list;

	/* operand1=0xf8 */
	elem_operand[0] = 0xf8;
	comp_operand = compound_create_with_elements(elem_operand, 1);
	if (comp_operand == NULL)
		die("put_incrementer_compounds: can't create operand1.");
	comp_opcode->list.next = &comp_operand->list;

	/* opcode2=0x48 0xff */
	elem_opcode[0] = 0x48; elem_opcode[1] = 0xff;
	comp_opcode = compound_create_with_elements(elem_opcode, 2);
	if (comp_opcode == NULL)
		die("put_incrementer_compounds: can't create opcode2.");
	comp_operand->list.next = &comp_opcode->list;

	/* operand2=0xc0 */
	elem_operand[0] = 0xc0;
	comp_operand = compound_create_with_elements(elem_operand, 1);
	if (comp_operand == NULL)
		die("put_incrementer_compounds: can't create operand2.");
	comp_opcode->list.next = &comp_operand->list;

	/* opcode=0xc3 */
	elem_opcode[0] = 0xc3;
	comp_opcode = compound_create_with_elements(elem_opcode, 1);
	if (comp_opcode == NULL)
		die("incrementer_create_body: can't create opcode2.");
	comp_operand->list.next = &comp_opcode->list;

	/* terminate */
	comp_opcode->list.next = NULL;

	/* append incr comp list to vessel list */
	struct organ *orgn = (struct organ *)body->orgn_head.next;
	struct singly_list *entry;
	for (entry = orgn->vessel_head.next; entry->next != NULL;
	     entry = entry->next);
	entry->next = incr_comp_head.next;
}

static void virus_infection(struct body *body)
{
	struct organ *orgn = (struct organ *)body->orgn_head.next;
	struct tissue *tiss = (struct tissue *)orgn->tiss_head.next;

	struct singly_list *entry;

	struct cell *cell;
	unsigned long long min_life_duration = 0xffffffff;
	struct cell *cell_min_ld;
	for (entry = tiss->cell_head.next; entry != NULL; entry = entry->next) {
		cell = (struct cell *)entry;
		if (min_life_duration > cell->life_duration) {
			cell_min_ld = cell;
			min_life_duration = cell->life_duration;
		}
	}

	struct protein *prot;
	for (entry = cell_min_ld->prot_head.next; entry != NULL;
	     entry = entry->next) {
		prot = (struct protein *)entry;
		struct compound *opcode = prot->opcode;
		if ((opcode->elements.bytes[0] == 0x48)
		    && (opcode->elements.bytes[1] == 0xff)) {
			struct compound *operand =
				(struct compound *)prot->operand_head.next;
			operand->elements.bytes[0] = 0xc8;
		}
	}
}

void incrementer_add_to_args_if_need(struct cell *cell, struct compound *comp,
				     struct singly_list *vessel_head)
{
	if (comp->len == sizeof(bio_data_t)) {
		struct singly_list *removed = slist_remove(
			(struct singly_list *)comp, vessel_head);
		if (removed == NULL)
			die("incrementer_add_to_args_if_need: remove entry not found.");
		cell->args[0] = comp;
		cell->is_can_react = TRUE;
	}
}

struct rect th_rect = {
	8 * FONT_WIDTH, 0,
	FONT_WIDTH * 2, FONT_HEIGHT
};

struct rect cells_rect = {
	0, 3 * FONT_HEIGHT,
	FONT_WIDTH * 26, FONT_HEIGHT * 4
};

struct rect data_rect = {
	15 * FONT_WIDTH, 8 * FONT_HEIGHT,
	FONT_WIDTH * 2, FONT_HEIGHT
};

struct rect data_rect2 = {
	FONT_WIDTH * 22, 8 * FONT_HEIGHT,
	FONT_WIDTH * 2, FONT_HEIGHT
};

struct rect code_rect = {
	0, 11 * FONT_HEIGHT,
	FONT_WIDTH * 26, FONT_HEIGHT * 2
};

struct pixelformat bg_color = {
	0, 0, 0, 255
};

static void draw_template(struct body *body)
{
	struct organ *orgn = (struct organ *)body->orgn_head.next;
	struct tissue *tiss = (struct tissue *)orgn->tiss_head.next;
	struct singly_list *entry;

	clear_screen();

	move_cursor_text(4, 0);
	puts("### ");
	putd(0, 2);
	puts("th cycle ###");

	move_cursor_text(0, 2);
	puts("Cells:\r\n");
	entry = tiss->cell_head.next;
	cell_dump_entry((struct cell *)entry);

	move_cursor_text(0, 7);
	puts("Data Compounds:\r\n");
	puts("[00 00 00 00 00 00 00 00]");

	move_cursor_text(0, 10);
	puts("Code Compounds:");
}

static void update_screen(struct body *body)
{
	struct organ *orgn = (struct organ *)body->orgn_head.next;
	struct tissue *tiss = (struct tissue *)orgn->tiss_head.next;
	struct singly_list *entry;
	struct compound *comp;

	fill_rect(&th_rect, &bg_color);
	move_cursor_text(8, 0);
	putd(incr_cycle_num, 2);

	fill_rect(&cells_rect, &bg_color);
	move_cursor_text(0, 3);
	entry = tiss->cell_head.next;
	cell_dump_entry((struct cell *)entry);

	if (entry->next != NULL) {
		move_cursor_text(0, 5);
		cell_dump_entry((struct cell *)entry->next);
	}

	/* fill_rect(&data_rect, &bg_color); */
	/* move_cursor_text(15, 8); */
	/* puth(data_compound->elements.data, 2); */

	fill_rect(&data_rect2, &bg_color);
	move_cursor_text(22, 8);
	for (entry = orgn->vessel_head.next; entry != NULL;
	     entry = entry->next) {
		comp = (struct compound *)entry;
		if (compound_is_data(comp) == TRUE)
			puth(comp->elements.bytes[0], 2);
	}

	fill_rect(&code_rect, &bg_color);
	move_cursor_text(0, 11);
	compound_dump_list(&orgn->vessel_head, COMP_FILTER_CODE);
}

void incrementer_init_func(struct body *body)
{
	draw_template(body);
}

void incrementer_periodic_func(struct body *body)
{
	switch (incr_cycle_num) {
	case PUT_INCR_COMP_TH:
		put_incrementer_compounds(body);
		break;

	case VIRUS_INFECTION_TH:
		virus_infection(body);
		break;
	}

	update_screen(body);

	incr_cycle_num++;
}

struct body *incrementer_create_body(void)
{
	/* 空の細胞を生成 */
	struct cell *cell = cell_create();
	if (cell == NULL)
		die("incrementer_create_body: can't create cell.");

	/* 寿命を短く設定 */
	cell->life_duration = FIRST_CELL_LIFE_DURATION;

	/* タンパク質のリスト(関数)を生成し細胞へ配置 */
	element_t elem_opcode[2], elem_operand[1];
	struct compound *comp_opcode, *comp_operand;
	struct protein *prot[3];

	unsigned int i;
	for (i = 0; i < INCREMENTER_MAX_COMPOUNDS; i++) {
		if (i > 0)
			codon_data[i - 1].list.next = &codon_data[i].list;
		codon_data[i].is_stored = FALSE;
	}
	codon_data[i].list.next = NULL;

	unsigned char codon_idx = 0;

	/* opcode=0x48 0x89 */
	elem_opcode[0] = 0x48; elem_opcode[1] = 0x89;
	comp_opcode = compound_create_with_elements(elem_opcode, 2);
	if (comp_opcode == NULL)
		die("incrementer_create_body: can't create opcode0.");
	codon_data[codon_idx++].comp_data = comp_opcode->elements.data;

	/* operand=0xf8 */
	elem_operand[0] = 0xf8;
	comp_operand = compound_create_with_elements(elem_operand, 1);
	if (comp_operand == NULL)
		die("incrementer_create_body: can't create operand0.");
	codon_data[codon_idx++].comp_data = comp_operand->elements.data;

	/* protein1 */
	prot[0] = protein_create_with_compounds(comp_opcode,
						&comp_operand->list);
	if (prot[0] == NULL)
		die("incrementer_create_body: can't create prot0.");

	/* opcode=0x48 0xff */
	elem_opcode[0] = 0x48; elem_opcode[1] = 0xff;
	comp_opcode = compound_create_with_elements(elem_opcode, 2);
	if (comp_opcode == NULL)
		die("incrementer_create_body: can't create opcode1.");
	codon_data[codon_idx++].comp_data = comp_opcode->elements.data;

	/* operand=0xc0 */
	elem_operand[0] = 0xc0;
	comp_operand = compound_create_with_elements(elem_operand, 1);
	if (comp_operand == NULL)
		die("incrementer_create_body: can't create operand1.");
	codon_data[codon_idx++].comp_data = comp_operand->elements.data;

	/* protein2 */
	prot[1] = protein_create_with_compounds(comp_opcode,
						&comp_operand->list);
	if (prot[1] == NULL)
		die("incrementer_create_body: can't create prot1.");
	prot[0]->list.next = &prot[1]->list;

	/* opcode=0xc3 */
	elem_opcode[0] = 0xc3;
	comp_opcode = compound_create_with_elements(elem_opcode, 1);
	if (comp_opcode == NULL)
		die("incrementer_create_body: can't create opcode2.");
	codon_data[codon_idx++].comp_data = comp_opcode->elements.data;

	/* protein3 */
	prot[2] = protein_create_with_compounds(comp_opcode, NULL);
	if (prot[2] == NULL)
		die("incrementer_create_body: can't create prot2.");
	prot[1]->list.next = &prot[2]->list;
	prot[2]->list.next = NULL;

	cell->prot_head.next = &prot[0]->list;
	cell->codon_head.next = &codon_data[0].list;

	/* 結合する化合物(引数)に関する設定 */
	cell->num_args = 1;
	cell->add_to_args_if_need = incrementer_add_to_args_if_need;

	/* 細胞にその他の設定を行う */
	cell->list.next = NULL;

	/* 組織を形成 */
	struct tissue *tiss = tissue_create_with_cell(cell);
	if (tiss == NULL)
		die("incrementer_create_body: can't create tissue.");
	tiss->list.next = NULL;

	/* 器官を形成 */
	struct organ *orgn = organ_create_with_tissue(tiss);
	if (orgn == NULL)
		die("incrementer_create_body: can't create organ.");
	orgn->list.next = NULL;

	/* 器官の管に初期値となる化合物を配置 */
	/* データ1 */
	data_compound = compound_create_with_data(INITIAL_DATA);
	if (data_compound == NULL)
		die("incrementer_create_body: can't create data compound.");
	data_compound->list.next = NULL;
	orgn->vessel_head.next = &data_compound->list;

	/* 生体へ器官を配置 */
	struct body *body = body_create_with_organ(&orgn->list);
	if (body == NULL)
		die("incrementer_create_body: can't create body.");
	body->init_func_hook = incrementer_init_func;
	body->periodic_func_hook = incrementer_periodic_func;

	/* protein_dump_list(&cell->prot_head); */

	incr_cycle_num = 1;

	return body;
}
