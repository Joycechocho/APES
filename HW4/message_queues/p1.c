#include <stdlib.h>  
#include <stdio.h>  
#include <string.h> 
#include <stdint.h>  
#include <sys/stat.h>  
#include <sys/types.h>  
#include <mqueue.h>  

#define QUEUE_NAME "/queue"

typedef struct _Message_t
{
  int8_t message[100];
  uint8_t led;
  size_t length;
}Message_t;
  
int main(int argc, char **argv)  
{  
 
  mqd_t mq;  
  struct mq_attr attr;    

  /* data for communication */
  Message_t send_message ={0};
  const char* msg = "This message is sent from p1 to p2";
  memmove(send_message.message,msg,strlen(msg));
  send_message.length = strlen(msg);
  send_message.led = 1;
  
  /* initialize the queue attributes */  
  attr.mq_flags = 0;  
  attr.mq_maxmsg = 10;  
  attr.mq_msgsize = sizeof(Message_t);  
  attr.mq_curmsgs = 0;  
  
  /* create the message queue */  
  mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);  
  if (mq == -1) 
  {
    perror("CREATE ERROR");
  }
    
  /* send the message */
  if(mq_send(mq, (char*)&send_message, sizeof(send_message), 0) == -1)
  {
    perror("SEND ERROR");
  }

  /* receive the message */ 
  Message_t reveived_msg = {0};

  if(mq_receive(mq, (char*)&reveived_msg, sizeof(reveived_msg),0) == -1)
  {
    perror("RECEIVE ERROR"); 
  }
	
  printf("String Printing: %s\tLED status: %s\n",
	reveived_msg.message, reveived_msg.led?"ON":"OFF");

  mq_close(mq);
  mq_unlink(QUEUE_NAME);
  
  return 0;  
}  
