obj-y += set_syscall_table.o
obj-m += getpid2.o
obj-m += getuid2.o
obj-m += open2.o

all:
	make -C /lib/modules/3.2.2+/build M=$(PWD) modules
	gcc -Wall -Werror test.c -o test
clean:
	make -C /lib/modules/3.2.2+/build M=$(PWD) clean
	rm test
