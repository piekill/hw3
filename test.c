#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include "test.h"
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_LEN 1024

int main(int argc, char *argv[])
{
	pid_t pid = -1;
	int fd = -1;
	char buf[MAX_LEN];
	int len = 0;
	int test_flag[] = {1, 1, 1};
	errno = 0;
	if (test_flag[0]) {
		printf("test getpid\n");
		set_syscall_table("vector_1", 1);
		printf("process id: %d\n", getpid());
		set_syscall_table("vector_2", 1);
		sleep(10000);
		printf("process id: %d\n", getpid());
		printf("user id: %d\n", getuid());
	}
	if ((pid = fork()) == 0)
	{
		printf("child process id %d\n", getpid());
		set_syscall_table("vector_1", 0);
		printf("child process id %d\n", getpid());
		return 0;
	}
	else 
	{
		printf("father process id %d\n", getpid());
	}
	if (test_flag[1]) {
		printf("test getuid\n");
		set_syscall_table("vector_1",1 );
		printf("user id : %d\n", getuid());
		set_syscall_table("vector_1", 0);
		printf("user id : %d\n", getuid());
	}

	if (test_flag[2]) {
		printf("test open\n");
		//set_syscall_table("vector_1", 0);
		printf("test open2\n");
		set_syscall_table("vector_1", 1);
		fd = open("infile.txt", O_RDONLY, S_IRWXU);
		printf("fd : %d\n", fd);
		len = read(fd, buf, MAX_LEN);
		buf[len] = '\0';
		printf("len : %d\n ------------content : \n %s\n", len, buf);
		close(fd);
	}
	return 0;
}

