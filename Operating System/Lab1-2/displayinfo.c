#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched.h>
#include<linux/init.h>

int init_module(void)
{
	int n_total=0;
	int n_running=0;
	int n_interruptible=0;
	int n_uninterruptible=0;
	int n_stopped=0;
	int n_traced=0;
	int n_exit_dead=0;
	int n_exit_zombie=0;
	int n_exit_trace=0;
	int n_dead=0;
	int n_wakekill=0;
	int n_waking=0;
	int n_parked=0;
	int n_noload=0;
	struct task_struct *p = &init_task;
	
	printk(KERN_INFO "Process info!\n");
	printk(KERN_INFO "%-16s%-16s%-16s%-16s\n", "Process name", "Process ID", "Process state", "Parent process name");
	for_each_process(p)
	{
		n_total++;
		switch(p->state)
		{
			case TASK_RUNNING:
				n_running++;
				break;
			case TASK_INTERRUPTIBLE:
				n_interruptible++;
				break;
			case TASK_UNINTERRUPTIBLE:
				n_uninterruptible++;
				break;
			case __TASK_STOPPED:
				n_stopped++;
				break;			
			case __TASK_TRACED:
				n_traced++;
				break;
			case EXIT_DEAD:
				n_exit_dead++;
				break;
			case EXIT_ZOMBIE:
				n_exit_zombie++;
				break;
			case EXIT_TRACE:
				n_exit_trace++;
				break;
			case TASK_DEAD:
				n_dead++;
				break;
			case TASK_WAKEKILL:
				n_wakekill++;
				break;
			case TASK_WAKING:
				n_waking++;
				break;
			case TASK_PARKED:
				n_parked++;
				break;
			case TASK_NOLOAD:
				n_noload++;
				break;
		}
		//from left to right : name pid state ppid
		printk(KERN_INFO "%-16s%-16d%-16ld%-16s\n", p->comm, p->pid, p->state, p->parent->comm);
	}
	printk(KERN_INFO "Total : %d\nRunning : %d\nInterruptible : %d\nUninterruptible : %d\nStopped : %d\nTraced : %d\nExit_Dead : %d\nExit_Zombie : %d\nExit_Trace : %d\nDead : %d\nWakekill : %d\nWaking : %d\nParked : %d\nNoload : %d\n", n_total, n_running, n_interruptible, n_uninterruptible, n_stopped, n_traced, n_exit_dead, n_exit_zombie, n_exit_trace, n_dead, n_wakekill, n_waking, n_parked, n_noload);
	
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Exit!\n");
	return;	
}

MODULE_LICENSE("GPL");