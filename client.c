/*
 * client.c - A client
 */
/* $begin clientmain */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
    host = argv[1]; 
    port = argv[2]; //12772

    clientfd = Open_clientfd(host, port);
    printf("Welcome, please enter your username with the keyboard:\n"); //welcome prompt
    Rio_readinitb(&rio, clientfd);

    if (Fgets(buf, MAXLINE, stdin) != NULL) {
	Rio_writen(clientfd, buf, strlen(buf));
	Rio_readlineb(&rio, buf, MAXLINE);
	Fputs(buf, stdout);
    }
    printf("Please enter your password with the keyboard:\n"); //psword prompt
    Rio_readinitb(&rio, clientfd);
    if (Fgets(buf, MAXLINE, stdin) != NULL) {
	Rio_writen(clientfd, buf, strlen(buf)); //server side receives this
	Rio_readlineb(&rio, buf, MAXLINE);
	Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:client:close
    exit(0);
}
/* $end clientmain */
