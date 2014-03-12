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
#include <linux/cred.h>
#include <linux/set_syscall_table.h>

#define DRIVER_DESC   "override system call getuid"
#define __NR_getuid2 199
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

// add 1 to original uid
// tested ok
asmlinkage int getuid2(void)
{
	printk("Calling getuid2\n");
	return current_uid() + 1;
}

static int __init getuid2_init(void)
{
	printk(KERN_INFO "Install getuid2 module\n");
	register_syscall("vector_1", __NR_getuid2, (long)getuid2);
	return 0;
}

static void __exit getuid2_exit(void)
{
	printk(KERN_INFO "Uninstall getuid2 module\n");
	register_syscall("vector_1", __NR_getuid2, 0);
}

module_init(getuid2_init);
module_exit(getuid2_exit);

