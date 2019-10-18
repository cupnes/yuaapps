#include <stdio.h>

#include <protein.h>

unsigned long long func_hello(
	unsigned long long _t1 __attribute__((unused)),
	unsigned long long _t2 __attribute__((unused)),
	unsigned long long _t3 __attribute__((unused)),
	unsigned long long _t4 __attribute__((unused)));

struct func_protein fp_samples[MAX_FP_ID] = {
	{
		.type = P_TYPE_FUNC,
		.id = FP_ID_HELLO,
		.func = func_hello,
		.args = {
			{ .d_type = DP_TYPE_NONE },
			{ .d_type = DP_TYPE_NONE },
			{ .d_type = DP_TYPE_NONE },
			{ .d_type = DP_TYPE_NONE }
		}
	}
};

unsigned long long func_hello(
	unsigned long long _t1 __attribute__((unused)),
	unsigned long long _t2 __attribute__((unused)),
	unsigned long long _t3 __attribute__((unused)),
	unsigned long long _t4 __attribute__((unused)))
{
	puts("Hello world!");
	return 0;
}
