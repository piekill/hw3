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


#define DRIVER_DESC   "set_syscall_table system call"

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

struct syscall_t syscall_table[10];

extern int (*do_set_syscall_table)(char *tname, int flag);
extern int register_syscall(char *tname, int num, long fn);
EXPORT_SYMBOL(register_syscall);

int set_syscall_table_mod(char *tname, int flag)
{
	int err = 0;
	int i;
	
	current->sysflag = flag;
	printk("process %d sysflag set\n", current->pid);
	
	if (current->syscall_table != NULL) {
		if (strcmp(current->syscall_table->name, tname)) {
			printk("Use another syscall table\n");
			spin_lock(&current->syscall_table->rflock);
			current->syscall_table->refcont -= 1;
			if (current->syscall_table->refcont == 0)
				current->syscall_table->active = 0;
			spin_unlock(&current->syscall_table->rflock);
		} else {
			return 0;
		}
	}
	
	for(i = 0; i < 10; i++) {
	  if (strcmp(tname, syscall_table[i].name) == 0) {
	    if(flag) {
			current->syscall_table = &syscall_table[i];
			spin_lock(&syscall_table[i].rflock);
			syscall_table[i].refcont += 1;
			spin_unlock(&syscall_table[i].rflock);

	    } else {
	      current->syscall_table = NULL;
	      spin_lock(&syscall_table[i].rflock);
	      syscall_table[i].refcont -= 1;
	      if (syscall_table[i].refcont == 0)
				syscall_table[i].active = 0;
	      spin_unlock(&syscall_table[i].rflock);
	    }
	  }	
	}
	
	if (i == 10)
		err = -1;
	printk("Exit set_syscall_table_mod\n");
	
	return err;
}

int register_syscall(char *tname, int num, long fn)
{
	int i;
	int err = 0;
	
	
	printk(KERN_INFO "register new fn at %s\n", tname);
	for (i = 0; i < 10; i++) {
		if (strcmp(tname, syscall_table[i].name) == 0) {
			break;
		} else if (syscall_table[i].active == 0) {
			memcpy(syscall_table[i].name, tname, strlen(tname));
			spin_lock_init(&syscall_table[i].rflock);
			syscall_table[i].active = 1;
			break;
		}
	}
	
	if (i < 10) 
		syscall_table[i].table[num] = fn;

	
	return err;
}


static int __init set_syscall_table_init(void)
{
	int i;
	if (do_set_syscall_table == NULL) {
		do_set_syscall_table = &(set_syscall_table_mod);
		
		/*Initialize syscall tables*/
		for(i = 0; i < 10; i++) {
			memset(syscall_table[i].table, 0, 350);
			memset(syscall_table[i].name, 0, 50);
			syscall_table[i].refcont = 0;
			syscall_table[i].active = 0;
		}
		
		if (do_set_syscall_table != NULL)
			printk(KERN_ALERT "Intall set_syscall_tabl module.\n");
		return 0;
	} else { 
		printk(KERN_ALERT "set_syscall_tabl in use.\n");
		return -1;
	}
}

static void __exit set_syscall_table_exit(void)
{
	if (do_set_syscall_table == set_syscall_table_mod) {
		do_set_syscall_table = NULL;
		printk(KERN_ALERT "Unintall set_syscall_tabl module.\n");
	}
}

module_init(set_syscall_table_init);
module_exit(set_syscall_table_exit);
