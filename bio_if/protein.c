#include <stdio.h>

#include <protein.h>

unsigned long long func_hello(unsigned long long, unsigned long long,
			      unsigned long long, unsigned long long);

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

unsigned long long func_hello(unsigned long long _t1, unsigned long long _t2,
			      unsigned long long _t3, unsigned long long _t4)
{
	puts("Hello world!");
	return 0;
}
