#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/kfifo.h>
#include <linux/kthread.h>
#include <linux/timer.h>

/* Configure the Module License, Descrption and Authorship */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joyce Cho");
MODULE_DESCRIPTION("Example Module that use kfifo to pass process information");

static struct task_struct *thread1;
static struct task_struct *thread2;
static struct task_struct *data;

static DECLARE_KFIFO(kfifo, struct task_struct*, 16);
static DEFINE_MUTEX(lock_fifo);

static int thread1_function(void *unused)
{
  while (!kthread_should_stop())
  {
    printk(KERN_INFO "Enter Thread 1\n");
    /* lock mutex */
    mutex_lock(&lock_fifo);

    /* put data into kfifo*/
    kfifo_put(&kfifo, current);

    /* unlock mutex */
    mutex_unlock(&lock_fifo);
    
    /* wake up every 3 seconds*/
    ssleep(3);
  }

  printk(KERN_INFO "Exit Thread 1\n");
  return 0;
}

static int thread2_function(void *unused)
{
  while (!kthread_should_stop())
  {
    while (!kfifo_is_empty(&kfifo)) 
    {
      printk(KERN_INFO "Enter Thread 2\n");
      
      /* lock mutex */
      mutex_lock(&lock_fifo);
      
      /* get data from kfifl */
      kfifo_get(&kfifo, &data);

      /* print the data from kfifo to kernel log */
      printk(KERN_INFO "Previous process -> PID: %d,  vruntime: %llu\n", 
             list_prev_entry(data, tasks)->pid, list_prev_entry(data, tasks)->se.vruntime);
      printk(KERN_INFO "Current Process -> PID: %d,  vruntime: %llu\n",	
             data->pid, data->se.vruntime);
      printk(KERN_INFO "Next Process -> PID: %d, vruntime: %llu\n",
             list_next_entry(data, tasks)->pid,list_next_entry(data, tasks)->se.vruntime);

      /* unlock mutex */
      mutex_unlock(&lock_fifo);
      ssleep(3);
    }
  }

  printk(KERN_INFO "Exit Thread 2\n");
  return 0;
}


int kthread_queue_enter(void)
{

  printk(KERN_INFO "Load kernel module");

  INIT_KFIFO(kfifo);

  /* create Thread 1*/
  thread1 = kthread_create(thread1_function, NULL, "Thread 1");
  if(IS_ERR(thread1))	
  {
    printk(KERN_ERR "Error when creating thread 1.\n");
    return -1;
  }
  else
  {
    wake_up_process(thread1);
  }

  /* create Thread 2*/
  thread2 = kthread_create(thread2_function, NULL, "Thread 2");
  if(IS_ERR(thread2))	
  {
    printk(KERN_ERR "Error when creating thread 2.\n");
    return -1;
  }
  else
  {
    wake_up_process(thread2);    
  }

  return 0;
}

void kthread_queue_exit(void)
{

  kthread_stop(thread1);
  kthread_stop(thread2);
  kfifo_free(&kfifo);
  
  printk(KERN_INFO "Remove module");
}

module_init(kthread_queue_enter);
module_exit(kthread_queue_exit);




