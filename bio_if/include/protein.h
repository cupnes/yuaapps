#pragma once

#include <common.h>

enum {
	P_TYPE_DATA,
	P_TYPE_FUNC,
	MAX_P_TYPE
};

/* Protein */

typedef unsigned char protein_type_t;

#define PROTEIN_HEAD				\
	struct singly_list list;		\
	protein_type_t type;

struct protein {
	PROTEIN_HEAD
};


/* Data Protein */

enum {
	DP_TYPE_NONE,
	MAX_DP_TYPE
};

typedef unsigned char data_protein_type_t;

struct data_protein {
	PROTEIN_HEAD
	data_protein_type_t d_type;
	bool_t has_value;
	unsigned long long data;
};

/* Func Protein */

#define MAX_FUNC_PROTEIN_ARGS	4

enum {
	FP_ID_HELLO,
	MAX_FP_ID
};

typedef unsigned char func_protein_id_t;

struct func_protein {
	PROTEIN_HEAD
	func_protein_id_t id;
	unsigned long long (*func)(
		unsigned long long, unsigned long long,
		unsigned long long, unsigned long long);
	struct data_protein args[MAX_FUNC_PROTEIN_ARGS];
};

extern struct func_protein fp_samples[MAX_FP_ID];
