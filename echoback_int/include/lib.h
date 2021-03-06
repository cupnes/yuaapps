#ifndef _LIB_H_
#define _LIB_H_

enum SYSCCALL_NO {
	SYSCALL_PUTC,
	SYSCALL_GETC,
	SYSCALL_VPUTC,
	SYSCALL_KBC_HDLR,
	MAX_SYSCALL_NUM
};

unsigned long long syscall(
	unsigned long long syscall_id __attribute__((unused)),
	unsigned long long arg1 __attribute__((unused)),
	unsigned long long arg2 __attribute__((unused)),
	unsigned long long arg3 __attribute__((unused)));
void putc(char c);
char getc(void);
void set_kbc_handler(void *handler);

#endif
