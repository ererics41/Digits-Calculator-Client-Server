#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

//skeleton code copied from http://www.linuxhowtos.org/data/6/client.c, written by LinuxHowtos
//the basic framework for creating the socket, getting ip address and prot number from argv, 
//connecting, reading and writing were taken from this website. From here, code was modified to
//match the functionality of the assignment and comments were added for readability.

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    //check to make sure the correct number of args were made
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    //read in the port number and convert to integer
    portno = atoi(argv[2]);

    //create a socket with domain AF_INET(IPv4) and type SOCK_STREAM(TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //perform basic error checking on the created socket
    if (sockfd < 0) 
        error("ERROR opening socket");

    //get the ip address from args and store it as a hostent*. Check to 
    //make sure the address isn't null
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    //set the family of the serv_addr to match the socket
    serv_addr.sin_family = AF_INET;

    //copy the contents of server(stores ip address) into serv_addr
    bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);

    //set the port number of serv_addr to the port number read from argv
    serv_addr.sin_port = htons(portno);

    //connect to the server by passing in the socket and server address(serv_addr).
    //perform basic error handling
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    printf("Enter string: ");
    bzero(buffer,256);
    //take user input, we only need to accomadate max 128 chars so reading in a max os 256 is fine
    fgets(buffer,256,stdin);

    //write to the server, store the status as n and check to see if there's an error
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
        error("ERROR writing to socket");

    //clear the buffer with bzero and then read the output from the server and store it in buffer
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
        error("ERROR reading from socket");

    //print the values returned from the server
    printf("From server: %s\n",buffer);

    //if the output from the server has more digits than 1 then keep reading
    while(strlen(buffer) > 1){
    	bzero(buffer,256);
	    n = read(sockfd,buffer,255);
	    if (n < 0) 
	        error("ERROR reading from socket");
	    printf("From server: %s\n",buffer);
    }

    //close the socket
    close(sockfd);
    return 0;
}