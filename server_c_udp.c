/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

//skeleton code copied from http://www.linuxhowtos.org/data/6/server_udp.c, written by LinuxHowtos
//the basic framework for creating the socket, getting ip address and prot number from argv, 
//connecting, reading and writing were taken from this website. From here, code was modified to
//match the functionality of the assignment and comments were added for readability.

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
  int sock, length, n;
  socklen_t fromlen;
  struct sockaddr_in server;
  struct sockaddr_in from;
  char buffer[256];

  //make sure that the number of arguments in argv are correct
  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(0);
  }
  
  //create socket and perform basic error handling, use SOCK_DGRAM to indicate udp
  sock=socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) 
    error("Opening socket");

  length = sizeof(server);
  bzero(&server,length);

  //set the family of the address to match that of the socket
  server.sin_family=AF_INET;

  //since we don't need to specify an ip address for binding, set ip to INADDR_ANY
  server.sin_addr.s_addr=INADDR_ANY;

  //set the port number of the address to the input port number
  server.sin_port=htons(atoi(argv[1]));

  //bind the socket to the address stored in server and perform basic error handling
  if (bind(sock,(struct sockaddr *)&server, length)<0) 
    error("binding");

  fromlen = sizeof(struct sockaddr_in);

  //loop and check for messages from the client
  while (1) {
    //receive a message from the client and store in the buffer 
    n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from,&fromlen);
    if (n < 0) 
      error("recvfrom");

    //compute the response
    int sum = 0;
    for (int i = 0; i < strlen(buffer)-1; i++){
      if(buffer[i] < '0' || buffer[i] > '9'){
        bzero(buffer,256);
        i = strlen(buffer); 
      }else{
        sum = sum + buffer[i] - '0';
      }
    }
    bzero(buffer,256);
    snprintf(buffer , sizeof(buffer), "%d", sum);

    //send a message back to the client and perform basic error checking
    n = sendto(sock,buffer,sizeof(buffer), 0,(struct sockaddr *)&from,fromlen);
    if (n  < 0) 
      error("sendto");

    while(sum > 9){
      //compute the response
        int sum = 0;
        for (int i = 0; i < strlen(buffer)-1; i++){
          sum = sum + buffer[i] - '0';
      }
      bzero(buffer,256);
      snprintf(buffer , sizeof(buffer), "%d", sum);

      //write the buffer to the 
      n = sendto(sock,buffer,sizeof(buffer), 0,(struct sockaddr *)&from,fromlen);
      if (n  < 0) 
        error("sendto");
    }
  }
  return 0;
}
