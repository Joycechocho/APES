#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct _Message_t
{
  int8_t message[100];
  uint8_t led;
  size_t length;
}Message_t;

int main()
{

  /* data for communication */
  Message_t send_message ={0};
  const char* msg = "This message is sent from p2 to p1";
  memmove(send_message.message,msg,strlen(msg));
  send_message.length = strlen(msg);
  send_message.led = 1;  

  /* the size (in bytes) of shared memory object */
  const int SIZE = 4096;
 
  /* name of the shared memory object */
  const char* name = "OS";
 
  /* shared memory file descriptor */
  int shm_fd;
 
  /* pointer to shared memory object */
  void* ptr;
 
  /* open the shared memory object */
  shm_fd = shm_open(name, O_RDWR, 0666);
 
  /* memory map the shared memory object */
  ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
 
  /* read from the shared memory object */
  printf("%s", (char*)ptr);

  sleep(1);
  /* memory map the shared memory object */
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

  /* write to the shared memory object */
  sprintf(ptr, "String Printing: %s\tLED status: %s\n",
  send_message.message, send_message.led?"ON":"OFF");
  //sleep(15); 

  /* remove the shared memory object */
  shm_unlink(name);
  return 0;
}
