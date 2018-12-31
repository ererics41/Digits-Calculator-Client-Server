#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//skeleton code copied from http://www.linuxhowtos.org/data/6/client_udp.c, written by LinuxHowtos
//the basic framework for creating the socket, getting ip address and prot number from argv, 
//connecting, reading and writing were taken from this website. From here, code was modified to
//match the functionality of the assignment and comments were added for readability.

void error(const char *);
int main(int argc, char *argv[]){
  int sock, n;
  unsigned int length;
  struct sockaddr_in server, from;
  struct hostent *hp;
  char buffer[256];

  //check to ensure the right number of argvs were passed in
  if (argc != 3) { 
    printf("Usage: server port\n");
    exit(1);
  }

  //create a socket, use SOCK_DRAM to indicate use of udp then perform basic error checking
  sock= socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) 
    error("socket");

  //set the family of the server address(server) to match the socket
  server.sin_family = AF_INET;

  //get the host ip address from argv
  hp = gethostbyname(argv[1]);
  if (hp==0) 
    error("Unknown host");

  //copy the host ip address into the server address(server)
  bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);

  //set the server address(server)'s port number to the port number from argv
  server.sin_port = htons(atoi(argv[2]));

  length=sizeof(struct sockaddr_in);

  //prompt user to enter a string and store it in buffer
  printf("Enter string: ");
  bzero(buffer,256);
  fgets(buffer,255,stdin);

  //send the contents of buffer to the server
  n=sendto(sock,buffer, strlen(buffer),0,(const struct sockaddr *)&server,length);
  if (n < 0) 
    error("Sendto");

  //receive a response from the server and store it in buffer
  n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
  if (n < 0) 
    error("recvfrom");
  
  //print the values returned from the server
  printf("From server: %s\n",buffer);

  //if the output from the server has more digits than 1 then keep reading
  while(strlen(buffer) > 1){
    bzero(buffer,256);
    n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
    if (n < 0) 
      error("recvfrom");
    printf("From server: %s\n",buffer);
  }

  //close the socket
  close(sock);
  return 0;
}

void error(const char *msg)
{
  perror(msg);
  exit(0);
}