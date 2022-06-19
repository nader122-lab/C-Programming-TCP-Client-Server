/* Simple TCP Client Example */
#include <stdio.h>
/* Headers needed for the socket programming */
#include <sys/types.h> /*define some data types used in system calls */
#include <sys/socket.h> /*definitions of structures needed for sockets */
#include <netinet/in.h>
/*constants and structures needed for Internet domain addresses */
#include <netdb.h> /* Define structures for the server information*/
#include <errno.h> /* Necessary because errno value used */
#include <string.h> /* Necessary for memset() */
#include <stdlib.h> /* Necessary for exit() */

void error(char *msg)
{
fprintf(stderr, "%s: %s\n", msg, strerror(errno));
exit(1);
}
int main(int argc, char *argv[])
{
int sockfd; /* socket file descriptor */
int portno; /* port number of the server that this client wants to connect */
int n;
struct sockaddr_in serv_addr;/* socket structures for the Internet addresses*/
struct hostent *server;
/*The variable server is a pointer to a structure of type hostent, defined in netdb.h */
char buffer[256];
if (argc < 3) {
fprintf(stderr,"usage %s hostname port\n", argv[0]);
exit(0);
}
portno = atoi(argv[2]);

/* Takes the third argument in cmd line for the port as interger */
sockfd = socket(PF_INET, SOCK_STREAM, 0); /* Creates client socket */
if (sockfd == -1)
error("ERROR opening client socket");
server = gethostbyname(argv[1]);
/* Takes the host name (e.g. www.google.com) and returns a pointer to a hostent struct */
if (server == NULL) {
fprintf(stderr,"ERROR, no such host\n");
exit(0);
}

/* sets the fields in serv_addr */
memset(&serv_addr,0, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);serv_addr.sin_port = htons(portno);
/* Connecting to server */
if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
error("ERROR connecting to server");

printf("Please enter the message: ");
memset(buffer,0, 256);
fgets(buffer,255,stdin);
/* Sending the message to the server */
n = write(sockfd,buffer,strlen(buffer));
if (n == -1)
error("Client: ERROR writing to socket");
memset(buffer,0, 256);
/* Reading message back from server */
n = read(sockfd, asctime(info), 255);
printf("%s\n",asctime(info));
if (n == -1)
error("Client: ERROR reading from socket");
printf("%s\n",buffer);

/*Closing client socket */
close(sockfd);
return 0;
}
