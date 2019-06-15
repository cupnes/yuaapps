#include <lib.h>

#define WAIT_CYC_US_AFTER_COMP	100000000 /* 100s */

int main(void)
{
	set_fg(255, 255, 255);
	set_bg(0, 70, 250);
	clear_screen();

	putchar('0');

	exec_ap(open("ap_test_1"), 1);
	exec_ap(open("ap_test_2"), 2);
	exec_ap(open("ap_test_3"), 3);

	while (1)
		sleep(WAIT_CYC_US_AFTER_COMP);

	return 0;
}
