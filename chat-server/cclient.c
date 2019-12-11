#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        char usage[] = "cclient <address> <port> <nickname>\n";
        write(0, usage, strlen(usage));
        return 0;
    }

    // Set up all of the given information
    char nickname[16];
    memset(nickname, '\0', 16);
    char *address;
    int port;
    strncpy(nickname, argv[3], strlen(argv[3]));
    address = argv[1];
    port = atoi(argv[2]);

    // Connect to the server
    struct sockaddr_in remote;
    int sock;
    struct hostent *h;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *) &remote, sizeof(remote));
    remote.sin_family = (short) AF_INET;
    h = gethostbyname(address);
    bcopy((char *)h->h_addr_list[0], (char *)&remote.sin_addr, h->h_length);
    remote.sin_port = htons(port);

    int tester = connect(sock, (struct sockaddr *)&remote, sizeof(remote));
    if (tester < 0) {
        char errMsg[] = "Could not connect to given server.\n";
        write(0, errMsg, strlen(errMsg));
        return 0;
    } else {
        char joinMsg[50];
        sprintf(joinMsg, "0<%s>", nickname);
        joinMsg[strlen(joinMsg)] = '\0';
        write(sock, joinMsg, strlen(joinMsg));
    }

    int cpid;
    int inputSize = 136;
    
    // Child Process will listen to the server
    if ((cpid = fork() == 0)) {
        char buffer[inputSize];
        memset(buffer, '\0', inputSize);
        int charsRead;
        for (;;) {
            if ((charsRead = read(sock, buffer, inputSize)) > 0) {
                write(0, buffer, strlen(buffer));
                char newLine = '\n';
                write(0, &newLine, 1);
                bzero(buffer, inputSize);
            }
        }
        exit(0);
    }

    // Loop for sending messages
    for(;;) {
        
        // Taking in input
        int msgSize = 128;
        char input[msgSize];
        int counter = 0;
        
        char ch;
        read(1, &ch, 1);
        while (ch != '\n') {
            input[counter] = ch;
            counter++;
            if (counter == msgSize) {
                char errMsg[] = "Message is too long.\n";
                write(0, errMsg, strlen(errMsg));
                return 0;
            }
            read(1, &ch, 1);
        }          
        input[counter] = '\0';

        // Exit message
        if (strncmp(input, "exit", 4) == 0) {
            char exitMsg[25];
            sprintf(exitMsg, "2<%s>", nickname);
            write(sock, exitMsg, strlen(exitMsg));
            close(sock);
            kill(cpid, SIGTERM);
            return 0;
        }

        // Writing to the socket
        int num;
        char message[inputSize];
        sprintf(message, "1<%s> %s", nickname, input);
        if ((num = write(sock, message, strlen(message))) < 0) {
            char errMsg[] = "Write to server failed.\n";
            write(0, errMsg, strlen(errMsg));
        } 

        //if (strncmp(input, "exit", 4) == 0) {
        //    char quit[] = "You have terminated connection.\n";
        //    write(0, quit, strlen(quit));
        //    write(sock, input, strlen(input));
        //    close(sock);
        //    kill(cpid, SIGTERM);
        //}
    }

    return 0;
}
