#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
  uint32_t i=0, loop_index=0;
  int32_t* array;
  int32_t* sorted_buffer;

  FILE* fp;
  fp=fopen("sys_output.txt","a");
  
  while(loop_index<3)
  {
    uint32_t buffer_size=(256+ rand()%256)*sizeof(int32_t);
    fprintf(fp, "%u bites of buffer has been allocated\n ", buffer_size);
    
    array=(int32_t*)malloc(buffer_size) ;  

    fprintf(fp, "return value: %ld\n", syscall(333, array, buffer_size, sorted_buffer));
    loop_index++;
  }
  
  return 0;
}
