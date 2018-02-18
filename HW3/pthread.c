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

int counts[26]={0};

clock_t t; //Caculate the timestamp when entering/exiting the threads

static int thread_1_count;
static int thread_2_count;

const char * filename = "./output.txt";
static void * task1(void *arg);
static void * task2(void *arg);

struct thread_info
{
  //uint32_t number; 
  FILE * FH_p;
};

struct Node
{
  char *c;
  struct count *next;
}*head;

struct periodic_info {
  int sig;
  sigset_t alarm_sig;
};

/* Set the POSIX timers to wake thread 2 up */
static int make_periodic(int unsigned period, struct periodic_info *info)
{
  static int next_sig;
  int ret;
  unsigned int ns;
  unsigned int sec;
  struct sigevent sigev;
  timer_t timer_id;
  struct itimerspec itval;

  /* Initialise next_sig first time through. We can't use static
  initialisation because SIGRTMIN is a function call, not a constant */
  if (next_sig == 0)
    next_sig = SIGRTMIN;
  /* Check that we have not run out of signals */
  if (next_sig > SIGRTMAX)
    return -1;
  info->sig = next_sig;
  next_sig++;
  /* Create the signal mask that will be used in wait_period */
  sigemptyset(&(info->alarm_sig));
  sigaddset(&(info->alarm_sig), info->sig);

  /* Create a timer that will generate the signal we have chosen */
  sigev.sigev_notify = SIGEV_SIGNAL;
  sigev.sigev_signo = info->sig;
  sigev.sigev_value.sival_ptr = (void *)&timer_id;
  ret = timer_create(CLOCK_MONOTONIC, &sigev, &timer_id);
  if (ret == -1)
    return ret;

  /* Make the timer periodic */
  sec = period / 1000000;
  ns = (period - (sec * 1000000)) * 1000;
  itval.it_interval.tv_sec = sec;
  itval.it_interval.tv_nsec = ns;
  itval.it_value.tv_sec = sec;
  itval.it_value.tv_nsec = ns;
  ret = timer_settime(timer_id, 0, &itval, NULL);
  return ret;
}

static void wait_period(struct periodic_info *info)
{
  int sig;
  sigwait(&(info->alarm_sig), &sig);
}

void signal_handler(int arg)
{
  struct thread_info *ptr;
  ptr = malloc(sizeof(struct thread_info));
  ptr->FH_p=fopen("output.txt","a");
  if(arg==SIGINT)
  {
    printf("receive INT signal:  ");
    fprintf(ptr->FH_p,"Enter INT signal handler\n");
    fflush(ptr->FH_p);
  }else if(arg==SIGUSR2)
  {
    printf("receive USR2 signal");
    fprintf(ptr->FH_p, "Enter USR2 signal handler\n");
    fclose(ptr->FH_p);
    pthread_cancel(thread1);
    pthread_cancel(thread2);
  }else if(arg==SIGUSR1)
  {
    printf("receive USR1 signal");
    fprintf(ptr->FH_p, "Enter USR1 signal handler\n");
    fclose(ptr->FH_p);
    pthread_cancel(thread1);
    pthread_cancel(thread2);
  }
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
  struct thread_info *tinfo = (struct thread_info*)arg;
  long len;
  FILE * FH_p = tinfo->FH_p;
  t=clock();
  struct Node* head=NULL;
  struct Node *temp;

  /* Get the Info of Thread1 */
  fprintf(FH_p, "Thread1: Entered Thread1 with timestamp: %d\n", (int)t);
  pid_t tid = syscall(SYS_gettid);
  snprintf(str, sizeof(str), "Thread1 Linux Thread ID:%d\n", tid);
  fprintf(FH_p, "%s\n", str);
  fprintf(FH_p, "Thread1 self() pthread ID: %ld\n", pthread_self());
  
  /* Track the number of occurence */
  FILE *fp =fopen("Valentinesday.txt", "r");
  assert(fp);
  fseek(fp, 0, SEEK_END);
  len=ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *buf =(char *) malloc(__size_t (len+1));
  buf[len]='\0';
  fread(buf, 1, __size_t len, fp);
  fclose(fp);

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

  /* Exit Thread1 */
  t=clock();
  fprintf(FH_p, "Thread1: Exited Thread1 with timestamp: %d\n", (int)t );

  fflush(FH_p);
}

static void * task2 (void *arg)
{
  char str[BUFFER_SIZE];
  uint64_t temp=0;
  struct thread_info *ptr;
  ptr = malloc(sizeof(struct thread_info));
  ptr->FH_p=fopen("output.txt","a");

  /* Get Info of Thread2 */
  pid_t tid = syscall(SYS_gettid);
  t=clock();
  fprintf(ptr->FH_p, "Thread2: Entered Thread2 with timestamp: %d\n", (int)t);
  snprintf(str, sizeof(str), "Thread2 Linux Thread ID:%d\n", tid);
  printf("Thread2 ID: %d\n",tid);
  fprintf(ptr->FH_p, "%s\n", str);
  fprintf(ptr->FH_p, "Thread2 self() pthread ID: %ld\n", pthread_self());
  
  /* Set up the POSIX timer in thread 2*/ 
  struct periodic_info info;
  make_periodic(100000, &info);
  
  while(1)
  {
    thread_2_count++;
    wait_period(&info);

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
  }

  /* Exit Thread2 */
  t=clock();
  fprintf(ptr->FH_p, "Thread2: Exited Thread2 with timestamp: %d\n", (int)t);
  fflush(ptr->FH_p);
  return NULL;
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
  signal(SIGUSR1, signal_handler);
  signal(SIGUSR2, signal_handler);

  /* Start Threading */
  t=clock();
  fprintf(FH_p, "Master: Creating Thread with timestamp: %d\n", (int)t);
  
  tinfo->FH_p = FH_p;
  size_t stack;
  pthread_attr_init(&attr);
  pthread_attr_getstacksize(&attr, &stack);
  fprintf(FH_p, "Master: Default stack size: %ld\n", stack);
  
  sigset_t alarm_sig;
  sigemptyset(&alarm_sig);
  for (int i = SIGRTMIN; i <= SIGRTMAX; i++)
  sigaddset(&alarm_sig, i);
  sigprocmask(SIG_BLOCK, &alarm_sig, NULL);

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

  sleep(3);
  fprintf(FH_p, "Master: Thread 2 has %d iterations\n", thread_2_count);
  //pthread_kill(thread1, SIGSEGV);

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
  return 0;
}


