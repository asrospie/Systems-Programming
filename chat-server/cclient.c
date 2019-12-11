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
    }

    int cpid;
    
    // Child Process will listen to the server
    if ((cpid = fork() == 0)) {
        char buffer[128];
        int charsRead;
        for (;;) {
            if ((charsRead = read(sock, buffer, 128)) > 0) {
                write(0, buffer, strlen(buffer));
                char newLine = '\n';
                write(0, &newLine, 1);
                bzero(buffer, 128);
            }
        }
    }

    // Loop for sending messages
    for(;;) {
        
        // Taking in input
        char input[136];
        int counter = 0;

        // Set up message header
        input[counter++] = '1';
        input[counter++] = '<';
        for (int i = 0; i < strlen(nickname); i++) {
            input[counter++] = nickname[i];   
        }
        input[counter++] = '>';
        input[counter++] = ' ';
        
        char ch;
        read(1, &ch, 1);
        while (ch != '\n') {
            input[counter] = ch;
            counter++;
            if (counter == 128) {
                char errMsg[] = "Message is too long.\n";
                write(0, errMsg, strlen(errMsg));
                return 0;
            }
            read(1, &ch, 1);
        }          
        input[counter] = '\0';

        // Writing to the socket
        int num;
        if ((num = write(sock, input, counter)) < 0) {
            char errMsg[] = "Write to server failed.\n";
            write(0, errMsg, strlen(errMsg));
        } else {
            printf("%s WAS SENT\n", input);
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
