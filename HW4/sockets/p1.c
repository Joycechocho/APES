#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _Message_t
{
  int8_t message[100];
  uint8_t led;
  size_t length;
}Message_t;

void main(void)
{
  /* data for communication */
  Message_t send_message ={0};
  const char* msg = "This message is sent from p1 to p2";
  memmove(send_message.message,msg,strlen(msg));
  send_message.length = strlen(msg);
  send_message.led = 0;  

  int s;
  int var;

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons((u_short) 1234);

  /* create the socket */
  s = socket(PF_INET, SOCK_STREAM, 0);
  connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  /* send the message */ 
  send(s, (char*)&send_message, sizeof(send_message), 0);

  sleep(2);

  /* receive the message */ 
  Message_t reveived_msg = {0};
  var = read(s, (char*)&reveived_msg, 1024);
  printf("String Printing: %s\tLED status: %s\n",
	reveived_msg.message, reveived_msg.led?"ON":"OFF");

  /* close the connection */
  close(s);
}
