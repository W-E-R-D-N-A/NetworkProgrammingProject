# NetworkProgrammingProject
My network programming project from Intro to Computer Systems. A simple client-server pair using socket programming in C to take and verify a username & password from the client 
and send back data from the server (market stock value).

Makefile
        "make clean; make" to compile everything

connect_timeo.c
        Connection timeout timer

csapp.{c,h}
        CS:APP3e functions
echo.c
        Echo routine used by all of our echo server examples

client.c
        Client for servers

server.c
        Iterative server

hostinfo.c
        Using getaddrinfo to get host IP address

unp.h
        Utilized by connect_timeo.c

StockFile.txt
        Text file containing sample username, password, and stock price value data

tiny/
tiny.tar
        The Tiny Web server
