/* Simple TCP Server where the port number is passed as an argument */
#include <stdio.h>
#include <time.h>
/* Headers needed for the socket programming */
#include <sys/types.h> /*define some data types used in system calls */
#include <sys/socket.h> /*definitions of structures needed for sockets */
#include <netinet/in.h>
/*constants and structures needed for Internet domain addresses */
#include <errno.h> /* Necessary because errno value used */
#include <string.h> /* Necessary for bzero() */
#include <stdlib.h> /* Necessary for exit() */
void error(char *msg)
{
fprintf(stderr, "%s: %s\n", msg, strerror(errno));
exit(1);
}

int main(int argc, char *argv[])
{
int sockid, clientSockid; /* socket file descriptors */
int portno; /* port number on which the server accepts connections */
socklen_t clientLen;
int n;
char buffer[256]; /* server reads characters from the socket into this buffer */
/* socket structures for the Internet addresses*/
struct sockaddr_in serv_addr, cli_addr;
if (argc < 2) error("No port provided in cmd line");
/*creates a stream socket that belongs to the INTERNET family */
sockid = socket(PF_INET, SOCK_STREAM, 0);
if (sockid == -1) error("Cannot open listening socket\n");
bzero((char *) &serv_addr, sizeof(serv_addr)); /*sets all values to zero */
portno = atoi(argv[1]);


/* setting the individual fields of the sockaddr_in structure */
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = htonl( INADDR_ANY);
serv_addr.sin_port = htons(portno); /* converts to big‐endian if necessary */
/*binds socket to the address of the current host and port number on which the
server will run */
if (bind(sockid, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
error("Cannot bind to listening socket");
/* listen on the socket for connections with a backlog queue of 5 */
if (listen(sockid, 5) == -1) error("Cannot listen");

while(1)
{
/*accept() causes the process to block until a client connects to the server */
clientLen = sizeof(cli_addr);
clientSockid = accept (sockid, (struct sockaddr *) &cli_addr, &clientLen);
if (clientSockid == -1) error("Server cannot accept client socket");
/*client has successfully connected to our server */

/* buffer is initialised and then server reads from the client socket descriptor */
bzero(buffer, 256);
n = read(clientSockid,buffer,255); /* read() blocks until something to read */
if (n == -1) error("Server cannot read from client socket");

time_t rawtime;
   struct tm *info;
   time( &rawtime );
   info = localtime( &rawtime );

/*Once a connection has been established, both ends can both read and write to
the connection */
/* simply writes a short message to the client */
n = write(clientSockid, asctime(info), 255);
if (n == -1) error("Server cannot write to client socket");
}

/* Closing sockets before ending main */
close(sockid);
close(clientSockid);
return 0;
}
