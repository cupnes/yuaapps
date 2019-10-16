#pragma once

#define MAX_DNA_LEN	64

struct queue {
	struct queue *next;
	struct queue *prev;
};

/* protein */
struct protein_head {
	struct queue q;
	unsigned char type;
	unsigned char sub_type;
};

struct func_protein {
	struct protein_head h;
	void *func;
	struct data_protein *args;
};

struct data_protein {
	struct protein_head h;
	unsigned char *data;
};

/* cell */
struct cell_head {
	struct queue q;
};

struct cell {
	struct cell_head h;
	struct func_protein *p_list;
	struct func_protein *p_buf;
	unsigned char dna[MAX_DNA_LEN];
	struct cell_nerve {
		struct cell_nerve *next;
		unsigned char val;
	} nerve;
	unsigned long long life_duration;
};

/* organ */
struct organ {
	struct cell *c_list;
	struct protein_head *vessel;
};

void cell_exec(struct cell *c);
