#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 256

int main(void)
{
  uint32_t i=0, loop_index=0;
  
  clock_t t;
  FILE* fp;
  fp=fopen("sys_output.txt","a");
  
  while(loop_index<3)
  {

    int32_t* sorted_buffer=(int32_t*)malloc(SIZE*sizeof(int32_t));
    int32_t* array=(int32_t*)malloc(SIZE*sizeof(int32_t));

    fprintf(fp, "unsorted: \n");

    for(i=0;i<SIZE;i++)
    {
      *(array+i)=rand()%1000;
      fprintf(fp, "%d\t", *(array+i));
    } 
    t=clock();
    fprintf(fp, "\nSyscall: return value: %ld\n", syscall(333, array, SIZE, sorted_buffer));
    t=clock()-t;
    fprintf(fp, "\nsorted: \n ");

    for(i=0;i<SIZE;i++)
    {
      fprintf(fp, "%d\t", *(sorted_buffer+i));   
    }
     
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    fprintf(fp, "\nsys call took %f seconds to execute\n\n\n\n\n", time_taken);
    loop_index++;
  }  

  /* System calls fail */
  fprintf(fp, "\nSystem Call fail. Return Value: %ld\n", syscall(333, NULL, SIZE, NULL));
  fclose(fp);
  return 0;
}
