#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>


#define BUFFER_SIZE (64)
#define NUM_THREADS 2

const char * filename = "./output.txt";
static void * task1(void *arg);
static void * task2(void *arg);

struct thread_info
{
  //pthread_t tid;
  uint32_t number; 
  FILE * FH_p;
};

void signal_handler(int arg)
{
  FILE *fp;
  fp=fopen("signal_output.txt", "w");
  fprintf(fp,"enter signal handler...threads closed");
  fclose(fp);
  signal(SIGSEGV, signal_handler);
}

clock_t t;

static void * task1 (void *arg)
{
  char str[BUFFER_SIZE];
  uint64_t temp=0;
  struct thread_info *tinfo = (struct thread_info*)arg;
  FILE * FH_p = tinfo->FH_p;

  t=clock();
  fprintf(FH_p, "Thread1: Entered Thread1 with timestamp: %d\n", (int)t);



  
  pid_t tid = syscall(SYS_gettid);
  


  snprintf(str, sizeof(str), "Thread1 Linux Thread ID:%d\n", tid);
  fprintf(FH_p, "%s\n", str);
  //fprintf(FH_p, "Thread1: tid: %ld\b", tinfo->tid);
  fprintf(FH_p, "Thread1 self() pthread ID: %ld\n", pthread_self());
  fprintf(FH_p, "Thread1: number %d\n", tinfo->number);

  while(tinfo->number)
  {
    temp *= 10;
    temp += tinfo->number % 10;
    tinfo->number /= 10;
  }
  //temp = temp / 0;
  fprintf(FH_p, "Thread1: reversed number %ld\n", temp);

  t=clock();
  fprintf(FH_p, "Thread1: Exited Thread1 with timestamp: %d\n", (int)t );

  fflush(FH_p);
}

static void * task2 (void *arg)
{
  char str[BUFFER_SIZE];
  uint64_t temp=0;
  struct thread_info *tinfo = (struct thread_info*)arg;
  FILE * FH_p = tinfo->FH_p;

  pid_t tid = syscall(SYS_gettid);
  t=clock();
  fprintf(FH_p, "Thread2: Entered Thread2 with timestamp: %d\n", (int)t);
  snprintf(str, sizeof(str), "Thread2 Linux Thread ID:%d\n", tid);
  fprintf(FH_p, "%s\n", str);
  //fprintf(FH_p, "Thread2: tid: %ld\b", tinfo->tid);
  fprintf(FH_p, "Thread2 self() pthread ID: %ld\n", pthread_self());

  t=clock();
  fprintf(FH_p, "Thread2: Exited Thread2 with timestamp: %d\n", (int)t);
  fflush(FH_p);
}

void main()
{
  FILE * FH_p;
  int ret;
  char str[BUFFER_SIZE];
  pthread_attr_t attr;
  struct thread_info *tinfo;
  tinfo = (struct thread_info *)malloc(sizeof(struct thread_info));
  if(! tinfo)
  {
    printf("ERROR: Could not allocate thread info struct\n");
    exit(1);
  }
  
 
  FH_p = fopen(filename, "w");
  if(!FH_p)
  {
    printf("ERROR: Could not open file %s for writing\n", filename);
    exit(1);
  }
  pid_t tid = syscall(SYS_gettid);
  snprintf(str, sizeof(str), "Master: PPID: %d PID: %d TID:%d\n", getppid(), getpid(), tid);
  ret = fprintf(FH_p, "%s\n", str);
  if(!ret)
  {
    printf("ERROR: File Write failed! code: %d\n", ret);
  }  

  signal(SIGSEGV, signal_handler);

  /* Start Threading */
  t=clock();
  fprintf(FH_p, "Master: Creating Thread with timestamp: %d\n", (int)t);
  
  tinfo->FH_p = FH_p;
  tinfo->number = 1234;
  size_t stack;
  pthread_attr_init(&attr);
  pthread_attr_getstacksize(&attr, &stack);
  fprintf(FH_p, "Master: Default stack size: %ld\n", stack);
  
  pthread_t thread1, thread2;

  if(pthread_create(&thread1, NULL, task1, (void*)tinfo))
  {
    printf("ERROR: Could not creat thread1!\n");
    exit(1);
  }
  
  if(pthread_create(&thread2, NULL, task2, (void*)tinfo))
  {
    printf("ERROR: Could not creat thread2!\n");
    exit(1);
  }

  pthread_kill(thread1, SIGSEGV);

  if(pthread_join(thread1, NULL))
  {
    printf("ERRORL Could not join thread!\n"); 
    exit(1);
  }

  if(pthread_join(thread2, NULL))
  {
    printf("ERRORL Could not join thread!\n"); 
    exit(1);
  }

  t=clock();
  fprintf(FH_p,"Master: Thread returned with timestamp: %d\n", (int)t);
  /* Threads closed */

  fflush(FH_p);
  if(fclose(FH_p))
  {
    printf("ERROR: Could not close file %s\n", filename);
  }
  return;
}


