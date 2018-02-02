/* 
 * 
 * Description: Kernel Module that uses a kernel timer to wake up every 500 ms
 * with two parameters. 
 * Author: Joyce Cho
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

/* Configure the Module License, Descrption and Authorship */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joyce Cho");
MODULE_DESCRIPTION("Example Module that use timer to wake up every 500 ms");

struct timer_list my_timer;

/* Default value of input argument */
static char *name = "name-default";
static int count = 500;
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "Specify your name");
module_param(count, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(count, "Specify the timer count time");

/* Timer callback */
void my_timer_callback (unsigned long data)
{
  static int cnt = 0;
  printk(KERN_INFO "Number of times the timer has expired: %d\n", cnt++);
  mod_timer(&my_timer, (jiffies+msecs_to_jiffies(count)));
}

/* Static private declaration of module initiatlization routine */
static int problem5_module_entry(void)
{
  printk("Registering the module!\n");
  printk(KERN_INFO "INFO: Registering the module!\n");
  printk(KERN_INFO "your name is : %s\n", name);
  printk(KERN_INFO "the count time is : %d\n", count);
  printk(KERN_ALERT "ALERT: Registering the module!\n");
  
  setup_timer(&my_timer, my_timer_callback, 0); //timer_setup() in 4.15
  add_timer(&my_timer);

  return 0;
}

static void problem5_module_exit(void)
{
  del_timer(&my_timer);
  printk("Unloaded kernel module!\n");
  printk(KERN_INFO "INFO: Registering the module!\n");
  printk(KERN_ALERT "ALERT: Registering the module!\n");
  return;
}

/* Define the Entry and Exit function */
module_init(problem5_module_entry);
module_exit(problem5_module_exit);


