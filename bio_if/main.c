#include <bio_if.h>
#include <stdio.h>

void cell_init(struct cell *c);
void func_hello(void);

struct func_protein hello_protein = {
	.func = func_hello
};

int main(void)
{
	struct cell hello_cell;
	cell_init(&hello_cell);
	cell_exec(&hello_cell);

	return 0;
}

void cell_init(struct cell *c)
{
	c->p_list = &hello_protein;
}

void func_hello(void)
{
	puts("Hello world!");
}
