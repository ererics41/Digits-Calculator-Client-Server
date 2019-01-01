# Digits-Calculator-Client-Server
A client-server application using socket programming in C to calculate the sum of digits in a number. A client sends as number
to the server and then the server returns the sum of the digits of that number. If the sum of the digits is greater than 9 then 
the server sends the sum of those digits as well and continues until it sends a number less than 10. The client then prints all 
the numbers recieved from the server. If an invalid input is sent to the server, then the server will response with "Sorry, cannot 
compute". The syntax to run the code is the following:

server:
server_application_name port number 
example: tcp_server 320000

client:
client_application_name IP address port number 
example: tcp_client 128.111.49.44 320000

an example run: 
eric> client 128.111.49.44 32000
Enter string: 123456789101234567891012345678910
From server: 138
From server: 12
From server: 3

The client-server has been implemented in both UDP and TCP and a makefile has also been included.
