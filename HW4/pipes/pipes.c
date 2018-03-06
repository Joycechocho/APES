#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

typedef struct _Message_t
{
  int8_t message[100];
  uint8_t led;
  size_t length;
}Message_t;

int main(int argc,char* argv[])
{

  /* data from father process to son process */
  Message_t send_message_fs ={0};
  const char* msg_fs = "This message is sent from father to son";
  memmove(send_message_fs.message,msg_fs,strlen(msg_fs));
  send_message_fs.length = strlen(msg_fs);
  send_message_fs.led = 0;
  
  /* data from son process to father process */
  Message_t send_message_sf ={0};
  const char* msg_sf = "This message is sent from son to father";
  memmove(send_message_sf.message,msg_sf,strlen(msg_sf));
  send_message_sf.length = strlen(msg_sf);
  send_message_sf.led = 1;

  int son2father[2];
  int father2son[2];
  pipe(son2father);    //this pipe is for son process sending data to father process
  pipe(father2son);    //this pipe is for father process sending data to son process
  char rd[20];
  if(fork() == 0)   
  {
    /* son process*/
    close(son2father[0]);   //close the pipe for reading in son2father
    close(father2son[1]);   //close the pipe for writing in father2son
     
    /* write data from son process to father process*/
    write(son2father[1],(char*)&send_message_sf, sizeof(send_message_sf));

    /* receive the message */ 
    Message_t reveived_msg_c = {0};
    /* read data from fater process */
    read(father2son[0],(char*)&reveived_msg_c, sizeof(reveived_msg_c));

    printf("in child process: String Printing: %s\tLED status: %s\n",
	reveived_msg_c.message, reveived_msg_c.led?"ON":"OFF");
  }
  else
  {
    /* father process*/
    close(son2father[1]);   //close the pipe for writing in son2father
    close(father2son[0]);   //close the pipe for reading in father2son
    
    /* write the data from father process to son process */
    write(father2son[1],(char*)&send_message_fs, sizeof(send_message_fs));
    
    /* receive the message */
    Message_t reveived_msg_f = {0};        
    
    /* read data from son process*/
    read(son2father[0],(char*)&reveived_msg_f, sizeof(reveived_msg_f));
 
    printf("in father process: String Printing: %s\tLED status: %s\n",
	reveived_msg_f.message, reveived_msg_f.led?"ON":"OFF");
  }
     
    return 0;
}
