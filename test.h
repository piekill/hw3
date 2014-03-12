#include <unistd.h>
#include <syscall.h>
#include <stdio.h>

#define __NR_set_syscall_table 349
#define __NR_getpid 20

long set_syscall_table(char *tname, int flag)
{
	return syscall(__NR_set_syscall_table, tname, flag);
}


int getpid()
{
	return syscall(__NR_getpid);
}

