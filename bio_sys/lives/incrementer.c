#include <element.h>
#include <compound.h>
#include <protein.h>
#include <cell.h>
#include <lib.h>

/* インクリメンタ細胞の機械語コード
protein1: opcode=0x48 0x89, operand=0xf8	mov %rdi,%rax
protein2: opcode=0x48 0xff, operand=0xc0	inc %rax
protein3: opcode=0xc3     , operand=NULL	ret
 */

struct body *incrementer_create_body(void)
{
	/* 空の細胞を生成 */
	struct cell *cell = cell_create();
	if (cell == NULL)
		die("incrementer_create_body: can't create cell.");

	/* タンパク質のリスト(関数)を生成し細胞へ配置 */
	element_t elem_opcode[2], elem_operand[1];
	struct compound *comp_opcode, *comp_operand;
	struct protein *prot[3];

	elem_opcode[0] = 0x48; elem_opcode[1] = 0x89;
	comp_opcode = compound_create_with_elements(elem_opcode, 2);
	if (comp_opcode == NULL)
		die("incrementer_create_body: can't create opcode0.");
	elem_operand[0] = 0xf8;
	comp_operand = compound_create_with_elements(elem_operand, 1);
	if (comp_operand == NULL)
		die("incrementer_create_body: can't create operand0.");
	prot[0] = protein_create_with_compounds(comp_opcode, comp_operand);
	if (prot[0] == NULL)
		die("incrementer_create_body: can't create prot0.");

	elem_opcode[0] = 0x48; elem_opcode[1] = 0xff;
	comp_opcode = compound_create_with_elements(elem_opcode, 2);
	if (comp_opcode == NULL)
		die("incrementer_create_body: can't create opcode1.");
	elem_operand[0] = 0xc0;
	comp_operand = compound_create_with_elements(elem_operand, 1);
	if (comp_operand == NULL)
		die("incrementer_create_body: can't create operand1.");
	prot[1] = protein_create_with_compounds(comp_opcode, comp_operand);
	if (prot[1] == NULL)
		die("incrementer_create_body: can't create prot1.");
	prot[0]->list.next = prot[1];

	elem_opcode[0] = 0xc3;
	comp_opcode = compound_create_with_elements(elem_opcode, 1);
	if (comp_opcode == NULL)
		die("incrementer_create_body: can't create opcode2.");
	prot[2] = protein_create_with_compounds(comp_opcode, NULL);
	if (prot[2] == NULL)
		die("incrementer_create_body: can't create prot2.");
	prot[1]->list.next = prot[2];
	prot[2]->list.next = NULL;

	cell->prot_list = prot;

	/* 結合する化合物の数(引数の数)を細胞へ設定 */
	cell->num_args = 1;

	/* DNAを生成し細胞へ配置 */
	/* TODO: T.B.D */

	/* 細胞にその他の設定を行う */
	cell->list.next = NULL;
	/* TODO: T.B.D */

	/* 器官を生成し細胞を配置 */
	struct organ *orgn = organ_create_with_cell(cell);
	if (orgn == NULL)
		die("incrementer_create_body: can't create organ.");
	orgn->list.next = NULL;

	/* 初期値として器官の管に数値 1 のデータ化合物を配置 */
	struct compound *comp_init_val = compound_create_with_data(1);
	if (comp_init_val == NULL)
		die("incrementer_create_body: can't create compound.");
	comp_init_val->list.next = NULL;
	orgn->vessel = comp_init_val;

	/* 生体へ器官を配置 */
	struct body *body = body_create_with_organ(orgn);
	if (body == NULL)
		die("incrementer_create_body: can't create body.");

	return body;
}
