/* 
 * 
 * Description: Kernel module that takes a process ID to investigate the process tree lineage
 * Author: Joyce Cho
 *
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/pid.h>

/* Configure the Module License, Descrption and Authorship */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is the module for evaluating the process tree");
MODULE_AUTHOR("Joyce Cho");

/* Default value of input argument */
static int pid = 1;
module_param(pid, int, S_IRUGO);
MODULE_PARM_DESC(pid, "Specify the process ID");

/* Find number of children */
int number_of_children(struct task_struct *task)
{
  struct list_head *node;
  int count=0;

  list_for_each(node, &(task->children))
  {
    count++;
  }
  return count;
}

/* Static private declaration of module initiatlization routine */ 
static int kthread_practice_module_init(void)
{
  struct task_struct *task;
  struct pid *pid_struct;
  printk(KERN_INFO "Enter Kernel module\n");
   
  if(pid==1)
  {
    task=current;
    /* trace from the current process back to init */
    //for(task=current; task!=&init_task; task=task->parent)
    for_each_process(task)
    {
      /* Print info of the current process*/
      printk(KERN_INFO "[Process Info] Thread Name: %s, Process ID: %d, Process Status: %d, Number of Children: %u, Nice Value: %d", task->comm, task->pid, task->state, number_of_children(task), task_nice(task));
    }
  }
  else
  {
    pid_struct = find_get_pid(pid);
    task = pid_task(pid_struct, PIDTYPE_PID);
    /* trace from the specific process back to init */
    //for(task=pid_task(pid_struct, PIDTYPE_PID); task!=&init_task; task=task->parent)
    for_each_process(task)
    {
      /* Print info of the given process*/
      printk(KERN_INFO "[Process Info] Thread Name: %s, Process ID: %d, Process Status: %d, Number of Children: %u, Nice Value: %d", task->comm, task->pid, task->state, number_of_children(task), task_nice(task));
    }
  }
    
  return 0;
}

static void kthread_practice_module_exit(void)
{
  printk(KERN_INFO "Kernel module exits\n");
}

module_init(kthread_practice_module_init);
module_exit(kthread_practice_module_exit);
