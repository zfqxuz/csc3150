#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/jiffies.h>
#include <linux/kmod.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
static struct task_struct *task;
extern long _do_fork (
	unsigned long clone_flags,
	unsigned long stack_start,  
	unsigned long stack_size,
	int __user *parent_tidptr,
	int __user *child_tidptr,
	unsigned long tls);


pid_t pid=0;


struct wait_opts {  
	enum pid_type wo_type; 
	int wo_flags; 
	struct pid * wo_pid; 
	struct siginfo __user *wo_info; 
	int __user *wo_stat;
	struct rusage __user *wo_rusage ; 
	wait_queue_t child_wait;
	int notask_error;
};

struct filename * getname(const char __user * filename);
long do_wait(struct wait_opts *wo);

int my_exec(void *j){
	printk("goofer");
	int result;
	char  files[][20]={"normal","hangup","interrupt","quit","illegal_instr","trap","abort","bus","floating","kill","segment_fault","pipe","terminate"};
	//for(int count=0; count<13;count++){
	char path[]="/opt/test/";
	strcat(path,files[0]);
	const char *const argv[]={path,NULL,NULL};
	const char *const envp[]={"HOME=/","PATH=/sbin:/user/sbin:/bin:/usr/bin"};
	struct filename * my_filename =getname(path);
	printk("here am i%s",my_filename->name);
	result=do_execve(my_filename,argv,envp);
	printk("the result in my_exec is %d",result);
	if(!result){return 0;}
	printk("and it goes here");
	
	do_exit(result);
}


void my_wait(pid_t pid){
	printk("i am waiting");
	
	int status;
	struct wait_opts wo;
	struct pid *wo_pid=NULL;
	enum pid_type type;
	type=PIDTYPE_PID;
	wo_pid=find_get_pid(pid);
	wo.wo_type=type;
	wo.wo_pid=wo_pid;
	wo.wo_flags=WEXITED|WUNTRACED;
	wo.wo_info=NULL;
	wo.wo_stat=(int __user*)&status;
	wo.wo_rusage=NULL;
	
	int a;
	a=do_wait(&wo);
	printk("do_Wait returns value %x\n",&a);
	printk("[DO_FORK]:the return signal is %x\n", (unsigned long)*wo.wo_stat);
	printk("the pid GIEGIEis%d",*wo.wo_pid);
	put_pid(wo_pid);
	return;
	}


int my_fork(void *argc){
	
	//set default sigaction for current process
	int i;
	struct k_sigaction *k_action = &current->sighand->action[0];
	for(i=0;i<_NSIG;i++){
		k_action->sa.sa_handler = SIG_DFL;
		k_action->sa.sa_flags = 0;
		k_action->sa.sa_restorer = NULL;
		sigemptyset(&k_action->sa.sa_mask);
		k_action++;
	}
	
	/* fork a process using do_fork */
	pid=_do_fork(SIGCHLD,(unsigned long)my_exec,0,NULL,NULL,0);
	printk("I am child and my pid is %d" ,pid);
	printk("I am dad and my pid is %d",(int)current->pid);
	/* execute a test program in child process */
	
	//printk(" address of target func is %x",(unsigned long)my_exec);
	//printk(" content at target address is %x",*my_exec);
	

	
	
	/* wait until child process terminates */
	my_wait(pid);
	
	return 0;
}

static int __init program2_init(void){

	printk("[program2] : Module_init\n");
	
	/* write your code here */
	task=kthread_create(&my_fork,NULL,"MY_THREAD");
	if(!IS_ERR(task)){
		printk("Kthread starts\n");
		wake_up_process(task);
	}	
	/* create a kernel thread to run my_fork */
	
	return 0;
}

static void __exit program2_exit(void){
	printk("[program2] : Module_exit\n");
}

module_init(program2_init);
module_exit(program2_exit);
