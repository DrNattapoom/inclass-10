#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include "pcsa_net.c"

#define BUFSIZE 1024
typedef struct sockaddr SA;

void writeFile(int connFd, char* outputFileName) {
    char buf[BUFSIZE];
    ssize_t bytesRead;

    int file = open(outputFileName, O_TRUNC | O_CREAT | O_WRONLY, 0666);
    if (file <= 0) {
        printf("unable to open %s file\n", outputFileName);
        return;
    }

    while ((bytesRead = read(connFd, buf, BUFSIZE)) > 0) {
        printf("DEBUG: Read %ld bytes\n", bytesRead);

        write_all(file, buf, bytesRead);
    }

    if (bytesRead == 0) {
        printf("DEBUG: Connection closed by the client");
    }
}

// ./echo_server 3000
int main(int argc, char* argv[]) { 
    // an fd for listening for incoming connn
    int listenFd = open_listenfd(argv[1]);

    // output filename
    char* outputFileName = argv[2];

    while (1) {
        // store addr of the client
        struct sockaddr_storage clientAddr; 
        // size of the above
        socklen_t clientLen = sizeof(struct sockaddr_storage); 

        // block until connects to the socket
        int connFd = accept(listenFd, (SA *) &clientAddr, &clientLen);

        // print the address of the incoming client
        char hostBuf[BUFSIZE], svcBuf[BUFSIZE];
        if (getnameinfo((SA *) &clientAddr, clientLen, hostBuf, BUFSIZE, svcBuf, BUFSIZE, 0) == 0) {
            printf("Connection from %s:%s\n", hostBuf, svcBuf); 
        } else {
            printf("Connection from UNKNOWN.");
        }
        
        writeFile(connFd, outputFileName);
        close(connFd);
    }

    return 0;

}
