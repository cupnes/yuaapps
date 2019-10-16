#include <bio_if.h>

void cell_exec(struct cell *c)
{
	void (*f)(void);
	f = c->p_list->func;
	f();
}
