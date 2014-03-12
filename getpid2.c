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

#define DRIVER_DESC   "override system call getpid"

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");



asmlinkage int getpid2(void)
{
  printk("process %d called getpid2\n", current->pid);
  return current->pid + 1;
}

static int __init getpid2_init(void)
{
	printk(KERN_INFO "Install getpid2 module\n");
	register_syscall("vector_2", 20, (long)getpid2);
	return 0;
}

static void __exit getpid2_exit(void)
{
	printk(KERN_INFO "Uninstall getpid2 module\n");
	register_syscall("vector_2", 20, 0);
}

module_init(getpid2_init);
module_exit(getpid2_exit);

