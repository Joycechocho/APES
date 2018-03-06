#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORTNUM 1234

typedef struct _Message_t
{
  int8_t message[100];
  uint8_t led;
  size_t length;
}Message_t;

int main(void)
{  
  /* data for communication */
  Message_t send_message ={0};
  const char* msg = "This message is sent from p2 to p1";
  memmove(send_message.message,msg,strlen(msg));
  send_message.length = strlen(msg);
  send_message.led = 1; 

  /* arguments for socket connection */
  int listen_s, connected_s, nbytes;
  struct sockaddr_in serv_addr, clnt_addr;
  char buf[256]; 
  unsigned long addr;
  int addrlen;

  if ( (listen_s = socket(PF_INET, SOCK_STREAM, 0) ) == -1 )
  {
    perror("ERROR in socket()");
    exit(1);
  }

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons((u_short)PORTNUM);

  if ( bind(listen_s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1 )
  {
    perror("ERROR in bind()");
    exit(2);
  }

  if ( listen(listen_s, 10) == -1 )
  {
    perror("ERROR in listen()");
    exit(3);
  }

  bzero(&clnt_addr, sizeof(clnt_addr));
  addrlen = sizeof(clnt_addr);

  if ( (connected_s = accept(listen_s, (struct sockaddr *)&clnt_addr, &addrlen) ) == -1 )
  {
    perror("ERROR in accept()");
    exit(4);
  }

  printf("Connection successful on %s\n", inet_ntoa(clnt_addr.sin_addr));

  /* receive the message */ 
  Message_t reveived_msg = {0};
  nbytes = recv(connected_s, (char*)&reveived_msg, sizeof(reveived_msg), 0);
  if (nbytes > 0)
  {
    buf[nbytes] = '\0';
    printf("String Printing: %s\tLED status: %s\n",
	reveived_msg.message, reveived_msg.led?"ON":"OFF");
  }

  /* send the message */
  send(connected_s , (char*)&send_message, sizeof(send_message) , 0 );
 
  /* close the socket */
  close(connected_s);
  close(listen_s);
  exit(0);
}
