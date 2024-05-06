/* 
 * server.c - An iterative server 
 * Sam Smallman, Project 4, CSCI-3240-001
 */ 
/* $begin servermain */
#include "csapp.h"

int timeFlag = 0; //global flag to close socket in case of timeout

void sig_handler(int signum){
  timeFlag = 1;
  //Close if alarm goes off
}

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];

    size_t n; 
    char buf[MAXLINE]; 
    rio_t rio;
    char* ack = "Acknowledged.\n"; // server acknowledgement message
    char* err = "Incorrect Username or Password.\n"; //error for if they do not match
    char* err2 = "Sorry, connection timed out.\n";

    char usrname[255]; //input username variable
    char psword[255]; //input password variable

    FILE *fp; //file pointer
    fp = fopen("StockFile.txt", "r"); //open file
    char buffer[255]; //reading buffer
    char result[10]; //to contain stock value
    char user[255]; //username from file variable
    char pass[255]; //password from file variable
    int match = 0; //variable to keep eof error from false positives near eof

    signal(SIGALRM,sig_handler);//registration of signal handler

    if(fp == NULL)
    {
     	printf("File not found.\n");
        exit(0); //no point running a server where you can't validate user connections
    }

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
	clientlen = sizeof(struct sockaddr_storage); 
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
	alarm(30); //set alarm for 30 seconds 
	Rio_readinitb(&rio, connfd);
        if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { 
		if(timeFlag == 1){
			snprintf(buf, 40,"%s\n",err2); //put timeout error in buffer
			Rio_writen(connfd, buf, 40);
			goto END;}
		alarm(0); //turn off alarm
                strcpy(usrname,buf); //sets usrname to client input
		snprintf(buf, 20,"%s\n",ack); //put server acknowledgement in buffer
		Rio_writen(connfd, buf, 20); //send timeout message to client
        }
	alarm(30); //reset alarm for 30 seconds
	if((n=Rio_readlineb(&rio, buf, MAXLINE))!=0){
		if(timeFlag == 1){
			snprintf(buf, 40,"%s\n",err2); //put timeout error in buffer
			Rio_writen(connfd, buf, 40); //send timeout message to client
			goto END;}
		alarm(0); //turn off alarm
                strcpy(psword,buf); //sets psword to client input
		while(fscanf(fp, "%s\n", buffer)==1) //reading loop
    		{
			strcpy(user,buffer); //user=buffer
			strncat(user, "\n", 1); //"/n" needed to match user input
			fscanf(fp,"%s\n",buffer);
			strcpy(pass,buffer); //pass=buffer
			strncat(pass, "\n", 1);
			if((strcmp(usrname, user)==0) && (strcmp(psword, pass) == 0)) //match found
    			{
        			fscanf(fp,"%s\n",buffer);
				strcpy(result,buffer);//result=buffer
				strncat(result, "\n", 1);
				match = 1;
				snprintf(buf, 40,"%s\n",result); //put stock value result in buffer
				break;
			}
			else //continue past stock value
   			{
				fscanf(fp,"%s\n",buffer);
			}
    		}
		if (feof(fp) && match!=1) 
		{
  			snprintf(buf, 40,"%s\n",err); //put error in buffer
		}
		Rio_writen(connfd, buf, 40);
        }
	fclose(fp);
	END: //label for the section that closes the socket
	Close(connfd);
        //exit(0);//TEST
    }
    exit(0);
}
/* $end servermain */
