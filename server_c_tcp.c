#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

//skeleton code copied from http://www.linuxhowtos.org/data/6/server.c, written by LinuxHowtos
//the basic framework for creating the socket, getting ip address and prot number from argv, 
//connecting, reading and writing were taken from this website. From here, code was modified to
//match the functionality of the assignment and comments were added for readability.

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    //check to make sure the right number of arguments were made
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    //create a socket with domain AF_INET(IPv4) and type SOCK_STREAM(TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //perform basic error checking on the socket created
    if (sockfd < 0) 
    	error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    //read in the port number from argv
    portno = atoi(argv[1]);

    //set the server address to match the family of the socket
    serv_addr.sin_family = AF_INET;

    //since we don't need to specify an ip address for binding, set ip to INADDR_ANY
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    //set the port number of serv_addr to the port number that the socket will be binded to
    serv_addr.sin_port = htons(portno);

    //bind the socket to the address specified in serv_addr
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    	error("ERROR on binding");

    while(1){
	    listen(sockfd,5);

	    //create a new socket to receive the contents of the client message
	    clilen = sizeof(cli_addr);
	    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	    if (newsockfd < 0) 
	    	error("ERROR on accept");

	    bzero(buffer,256);

	    //read from the new socket and store the input in the buffer, perform basic error checking
	    n = read(newsockfd,buffer,255);
	    if (n < 0) 
	    	error("ERROR reading from socket");


	    printf("Here is the message: %s\n",buffer);

	    //compute the response
	    int sum = 0;
	  	for (int i = 0; i < strlen(buffer)-1; i++){
			if(buffer[i] < '0' || buffer[i] > '9'){
	      		bzero(buffer,256);
		  		strcpy(buffer, "Sorry, cannot compute!");
		    	i = strlen(buffer);	
		 	}else{
	    		sum = sum + buffer[i] - '0';
			}
		}
		bzero(buffer,256);
		snprintf(buffer , sizeof(buffer), "%d", sum);

		//write the buffer to the client
		n = write(newsockfd,buffer,256);
		if (n < 0) 
		    error("ERROR writing to socket");

		while(sum > 9){
			//compute the response
		    int sum = 0;
		  	for (int i = 0; i < strlen(buffer)-1; i++){
		    	sum = sum + buffer[i] - '0';
			}
			bzero(buffer,256);
			snprintf(buffer , sizeof(buffer), "%d", sum);

			//write the buffer to the client
			n = write(newsockfd,buffer,256);
			if (n < 0) 
		    	error("ERROR writing to socket");
		}

		//close the new socket
		close(newsockfd);
	}

    return 0; 
}