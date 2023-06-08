#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include "pcsa_net.c" // #include "pcsa_net.h"

#define BUFSIZE 1024


int main(int argc, char* argv[]) {
    int clientFd = open_clientfd(argv[1], argv[2]);
    char *msg = "Hello, World!!!";

    ssize_t numToWrite = strlen(msg);
    char *writeBuf = msg;

    while (numToWrite > 0) {
        ssize_t numWritten = write(clientFd, writeBuf, numToWrite);
        if (numWritten < 0) { fprintf(stderr, "ERROR writing, meh\n"); break; }
        numToWrite -= numWritten;
        writeBuf += numWritten;
    }
    
    char buf[BUFSIZE];
    ssize_t numRead = read(clientFd, buf, BUFSIZE);
    if (numRead >0) {
        buf[numRead] = '\0'; /* Terminate the string */
        printf("From Server: %s\n", buf);
    }

    close(clientFd);

    return 0;
}
