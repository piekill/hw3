#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/mount.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/set_syscall_table.h>

#define DRIVER_DESC   "override system call open"
#define __NR_open 5
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

asmlinkage int open2(const char* filename, int flags, int mode) {
	long ret = 0;
	printk("Calling open2\n");
	printk ("filename : %s, flag : %d, mode : %d\n", filename, flags, mode);
	if (force_o_largefile())
		flags |= O_LARGEFILE;

	ret = do_sys_open(AT_FDCWD, filename, flags, mode);
	asmlinkage_protect(3, ret, filename, flags, mode);
	return ret;
}

static int __init open2_init(void) {
	printk (KERN_INFO "Install open2 module\n");
	register_syscall("vector_1", __NR_open, (long)open2);
	return 0;
}

static void __exit open2_exit(void) {
	printk(KERN_INFO "Uninstall open2 modul\n");
	register_syscall("vector_1", __NR_open, 0);
}

module_init(open2_init);
module_exit(open2_exit);
