#pragma once

#define TRUE	1
#define FALSE	0
#define NULL	(void *)0
#define CPU_PAUSE()	asm volatile ("pause")

typedef unsigned char bool_t;
typedef unsigned long long size_t;

void spin_lock(unsigned int *lock_flag);
void spin_unlock(volatile unsigned int *lock_flag);
