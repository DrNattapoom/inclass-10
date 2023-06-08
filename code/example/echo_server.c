#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include "pcsa_net.h"

#define BUFSIZE 1024
typedef struct sockaddr SA;

void echo_logic(int connFd) {
    char buf[BUFSIZE];
    ssize_t bytesRead;

    // read everything the client has sent & write it back to the client
    while ((bytesRead = read(connFd, buf, BUFSIZE)) > 0) {
        printf("DEBUG: Read %ld bytes\n", bytesRead);

        // write(connFd, buf, bytesRead);  BUG: short counts
        write_all(connFd, buf, bytesRead); // from pcsa_net
    }
    // if bytesRead < 0 -- ERROR, should be handled!
    if (bytesRead == 0)
        printf("DEBUG: Connection closed by the client");
}

// ./echo_server 3000
int main(int argc, char* argv[]) { 
    // an fd for listening for incoming connn
    int listenFd = open_listenfd(argv[1]);

    for (;;) {
        struct sockaddr_storage clientAddr; // to store addr of the client
        socklen_t clientLen = sizeof(struct sockaddr_storage); // size of the above

        // ...gonna block until someone connects to our socket
        int connFd = accept(listenFd, (SA *) &clientAddr, &clientLen);

        // print the address of the incoming client
        char hostBuf[BUFSIZE], svcBuf[BUFSIZE];
        if (getnameinfo((SA *) &clientAddr, clientLen, hostBuf, BUFSIZE, svcBuf, BUFSIZE, 0) == 0) 
            printf("Connection from %s:%s\n", hostBuf, svcBuf); 
        else 
            printf("Connection from UNKNOWN.");
        
        echo_logic(connFd);
        close(connFd);
    }

    return 0;
}
