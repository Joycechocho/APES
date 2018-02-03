#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include <../../hello/error.h>

SYSCALL_DEFINE3(hello, int32_t*, buffer, size_t, size, int32_t*, sorted_buffer)
{
  int32_t *kernel_buffer=NULL;
  int32_t temp;
  uint32_t i, j; 

  printk(KERN_INFO "A system call of string sorting has been entered\n");

  if(buffer==NULL || sorted_buffer==NULL || size<1)
  {
    printk(KERN_ALERT "Null buffer!\n");
    return ERROR_INVALID_INPUT; 
  }

  kernel_buffer = (int32_t*)kmalloc(size*sizeof(int), GFP_KERNEL);
  if(kernel_buffer==NULL)
  {
   printk(KERN_ALERT "kmalloc failed!\n");
   return ERROR_KMALLOC_FAILED;
  }else
  {
    printk(KERN_INFO "Dynamically allocated a buffer with %lu bytes in kernel space\n", sizeof(int)*size);
  }

  if(copy_from_user(kernel_buffer, buffer, size*sizeof(int))!=0)
  {
    printk(KERN_ALERT "copy data from user space unsuccessful!\n");
    return ERROR_COPY_FROM_USER_FAILED;
  }else
  {
    printk(KERN_INFO "copy the dynamically allocated buffer into user space!\n");
  }
 
  printk(KERN_INFO "Sorting Started!");
  for(i=0; i<size; i++) 
  {
    for(j=i; j<size; j++)
    {
      if(*(kernel_buffer+j)>*(kernel_buffer+i))
      {
        temp=*(kernel_buffer+i);
        *(kernel_buffer+i)=*(kernel_buffer+j);
        *(kernel_buffer+j)=temp;
      }
    }
  }

  printk(KERN_INFO "Sorting Complicated!");

  if(copy_to_user(sorted_buffer, kernel_buffer, size*sizeof(int))!=0)
  {
    printk(KERN_ALERT "copy data to user space unsuccessful!\n");
    return ERROR_COPY_TO_USER_FAILED;
  }else
  {
    printk(KERN_INFO "copy the sorted buffer into user space!\n");
  }

  kfree(kernel_buffer);

  printk(KERN_INFO "System call exits!");
  return 0;
}

