#include <element.h>
#include <protein.h>
#include <cell.h>
#include <lib.h>

/* インクリメンタ細胞の機械語コード
protein1: 64ビット拡張=0x48, opcode=0x89, operand=0xf8	mov %rdi,%rax
protein2: 64ビット拡張=0x48, opcode=0xff, operand=0xc0	inc %rax
protein3:                    opcode=0xc3, operand=NULL	ret
 */

static void create_comp_codn_with_elem(
	struct compound *comp, struct codon *codn,
	element_t *elem, unsigned int len)
{
	comp = compound_create_with_elements(elem, len);
	if (comp == NULL)
		die("create_comp_codn_with_elem: can't create a compound.");
	codn = codon_create_with_data(comp->elements.data);
	if (codn == NULL)
		die("create_comp_codn_with_elem: can't create a codon.");
}

struct singly_list *incrementer_create(void)
{
	/* 空の細胞を生成 */
	struct cell *cell = cell_create();
	if (cell == NULL)
		die("incrementer_create: can't create cell.");

	/* タンパク質のリスト(関数)を生成し細胞へ配置 */
	element_t elem[2];
	struct compound *comp_opcode, *comp_operand;
	struct codon *codn_opcode, *codn_operand;
	struct protein *prot[3];

	/* opcode=0x48 0x89 */
	elem_opcode[0] = 0x48; elem_opcode[1] = 0x89;
	comp_opcode = compound_create_with_elements(elem_opcode, 2);
	if (comp_opcode == NULL)
		die("incrementer_create_body: can't create opcode0.");
	codn_opcode = codon_create_with_data(comp_opcode->elements.data);
	if (codn_opcode == NULL)
		die("incrementer_create_body: can't create codon opcode0.");

	/* operand=0xf8 */
	elem_operand[0] = 0xf8;
	comp_operand = compound_create_with_elements(elem_operand, 1);
	if (comp_operand == NULL)
		die("incrementer_create_body: can't create operand0.");
	codn_data[codn_idx++].comp_data = comp_operand->elements.data;

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
	codn_data[codn_idx++].comp_data = comp_opcode->elements.data;

	/* operand=0xc0 */
	elem_operand[0] = 0xc0;
	comp_operand = compound_create_with_elements(elem_operand, 1);
	if (comp_operand == NULL)
		die("incrementer_create_body: can't create operand1.");
	codn_data[codn_idx++].comp_data = comp_operand->elements.data;

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
	codn_data[codn_idx++].comp_data = comp_opcode->elements.data;

	/* protein3 */
	prot[2] = protein_create_with_compounds(comp_opcode, NULL);
	if (prot[2] == NULL)
		die("incrementer_create_body: can't create prot2.");
	prot[1]->list.next = &prot[2]->list;
	prot[2]->list.next = NULL;

	cell->prot_head.next = &prot[0]->list;
	cell->codn_head.next = &codn_data[0].list;

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
