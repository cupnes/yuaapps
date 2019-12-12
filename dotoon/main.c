#include <lib.h>

int main(void)
{
	clear_screen();

	struct file *cpu = open("dotoon_cpu");
	exec_ap(cpu, 1);
	exec_ap(cpu, 2);
	exec_ap(cpu, 3);
	exec(cpu);

	return 0;
}
