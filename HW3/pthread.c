#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <assert.h>



#define BUFFER_SIZE (64)
#define NUM_THREADS 2
#define INTERVAL 100

pthread_t thread1, thread2;

void TimerStop(int signum)
{
  printf("Stop Timer");
}

void TimerSet(int interval)
{
  struct itimerval it_val;
  it_val.it_value.tv_sec = interval/1000000;
  it_val.it_value.tv_usec=0;
  it_val.it_interval.tv_sec=0;
  it_val.it_interval.tv_usec=0;

  if(signal(SIGALRM, TimerStop)==SIG_ERR)
  {
    perror("Unable to catch SIGALRM");
    exit(1); 
  }
  if(setitimer(ITIMER_REAL, &it_val, NULL) == -1)
  {
    perror("Error calling setitimer()");
    exit(1);
  }  
}

const char * filename = "./output.txt";
static void * task1(void *arg);
static void * task2(void *arg);

struct thread_info
{
  //pthread_t tid;
  uint32_t number; 
  FILE * FH_p;
};

int counts[26]={0};
struct Node
{
  char *c;
  struct count *next;
}*head;

void signal_handler(int arg)
{
  struct thread_info *ptr;
  ptr = malloc(sizeof(struct thread_info));
  ptr->FH_p=fopen("output.txt","a");
  printf("receive INT signal:  ");
  //FILE *fp;
  //fp=fopen("signal_output.txt", "w");
  fprintf(ptr->FH_p,"Enter INT signal handler\n");
  fflush(ptr->FH_p);
  fclose(ptr->FH_p);
  //pthread_cancel(thread1);
  //pthread_cancel(thread2);
  //signal(SIGINT, signal_handler);
  //signal(SIGSEGV, signal_handler);
}

clock_t t;

void push(struct thread_info **head_ref, char data)
{
  struct Node* new_node=(struct Node*) malloc(sizeof(struct Node));
  new_node->c=data; 
  new_node->next=(*head_ref);
  (*head_ref)=new_node;
  if(!isalpha(new_node->c))
  {
    return;
  }
  counts[(int)(tolower(new_node->c)-'a')]++;
}

static void * task1 (void *arg)
{
  char str[BUFFER_SIZE];
  //uint64_t temp=0;
  struct thread_info *tinfo = (struct thread_info*)arg;
  long len;
  FILE * FH_p = tinfo->FH_p;

  t=clock();
  fprintf(FH_p, "Thread1: Entered Thread1 with timestamp: %d\n", (int)t);



  
  pid_t tid = syscall(SYS_gettid);
  


  snprintf(str, sizeof(str), "Thread1 Linux Thread ID:%d\n", tid);
  fprintf(FH_p, "%s\n", str);
  //fprintf(FH_p, "Thread1: tid: %ld\b", tinfo->tid);
  fprintf(FH_p, "Thread1 self() pthread ID: %ld\n", pthread_self());
  
  FILE *fp =fopen("Valentinesday.txt", "r");
  assert(fp);
  fseek(fp, 0, SEEK_END);
  len=ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *buf =(char *) malloc(__size_t (len+1));
  buf[len]='\0';
  fread(buf, 1, __size_t len, fp);
  fclose(fp);
  
  struct Node* head=NULL;
  struct Node *temp;
  temp=malloc(sizeof(struct Node)*len);
  temp->c=buf;
  for(int i=0;i<len;i++)
  {
    push(&head, temp->c[i]);
  }
  for(int i=0;i<26;i++)
  {
    if(counts[i]==3)
    fprintf(FH_p, "Thread1: %c has %2d occurence\n", i+'a', counts[i]);
  }
  //fprintf(FH_p, "Thread1: number %d\n", tinfo->number);

  //while(tinfo->number)
  //{
  //  temp *= 10;
  //  temp += tinfo->number % 10;
  //  tinfo->number /= 10;
  //}
  //temp = temp / 0;
  //fprintf(FH_p, "Thread1: reversed number %ld\n", temp);

  t=clock();
  fprintf(FH_p, "Thread1: Exited Thread1 with timestamp: %d\n", (int)t );

  fflush(FH_p);
  //fclose(FH_p);
}

static void * task2 (void *arg)
{
  char str[BUFFER_SIZE];
  uint64_t temp=0;
  //struct thread_info *tinfo = (struct thread_info*)arg;
  //FILE * FH_p = tinfo->FH_p;
  struct thread_info *ptr;
  ptr = malloc(sizeof(struct thread_info));
  ptr->FH_p=fopen("output.txt","a");

  pid_t tid = syscall(SYS_gettid);
  t=clock();
  fprintf(ptr->FH_p, "Thread2: Entered Thread2 with timestamp: %d\n", (int)t);
  snprintf(str, sizeof(str), "Thread2 Linux Thread ID:%d\n", tid);
  printf("Thread2 ID: %d\n",tid);
  fprintf(ptr->FH_p, "%s\n", str);
  //fprintf(FH_p, "Thread2: tid: %ld\b", tinfo->tid);
  fprintf(ptr->FH_p, "Thread2 self() pthread ID: %ld\n", pthread_self());

  TimerSet(INTERVAL);;
  while(1)
  {
    /* Caculate CPU Utilization */
    long double a[4], b[4], loadavg;
    char dump[50];
    FILE *fp;
  
    fp = fopen("/proc/stat","r");
    fscanf(fp, "%*s %Lf %Lf %Lf %Lf", &a[0], &a[1], &a[2], &a[3]);
    fclose(fp);
    sleep(1);
    
    fp = fopen("/proc/stat","r");
    fscanf(fp, "%*s %Lf %Lf %Lf %Lf", &b[0], &b[1], &b[2], &b[3]);
    fclose(fp);
   
    loadavg=((b[0]+b[1]+b[2])-(a[0]+a[1]+a[2]))/((b[0]+b[1]+b[2]+b[3])-(a[0]+a[1]+a[2]+a[3]));
    fprintf(ptr->FH_p, "The current CPU utilization is %Lf\n", loadavg);
    printf("The current CPU utilization is %Lf\n",loadavg);
    fflush(ptr->FH_p);
    //fclose(FH_p);
  }
  t=clock();
  fprintf(ptr->FH_p, "Thread2: Exited Thread2 with timestamp: %d\n", (int)t);
  fflush(ptr->FH_p);
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
  signal(SIGINT, signal_handler);

  /* Start Threading */
  t=clock();
  fprintf(FH_p, "Master: Creating Thread with timestamp: %d\n", (int)t);
  
  tinfo->FH_p = FH_p;
  tinfo->number = 1234;
  size_t stack;
  pthread_attr_init(&attr);
  pthread_attr_getstacksize(&attr, &stack);
  fprintf(FH_p, "Master: Default stack size: %ld\n", stack);
  
  

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


