void spin_lock(unsigned int *lock_flag)
{
	unsigned char got_lock = 0;
	do {
		if (*lock_flag)
			CPU_PAUSE();

		unsigned int lock = 1;
		asm volatile ("xchg %[lock], %[lock_flag]"
			      : [lock]"+r"(lock), [lock_flag]"+m"(*lock_flag)
			      :: "memory", "cc");

		if (!lock)
			got_lock = 1;
	} while (!got_lock);
}

void spin_unlock(volatile unsigned int *lock_flag)
{
	*lock_flag = 0;
}
