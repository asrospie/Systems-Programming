#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

    // Create the server
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in s;
    bzero((char *) &s, sizeof(s));
    s.sin_family = (short) AF_INET;
    s.sin_addr.s_addr = htonl(INADDR_ANY);
    s.sin_port = htons(0);
    bind(listener, (struct sockaddr *)&s, sizeof(s));    
    int length = sizeof(s);
    getsockname(listener, (struct sockaddr *)&s, (socklen_t*)&length);
    
    // print out the relevant information
    char addressMsg[length];
    sprintf(addressMsg, "%d %d\n", ntohs(s.sin_port), s.sin_addr.s_addr);     
    write(0, addressMsg, strlen(addressMsg));

    // Connection storage
    int clientMax = 10;
    int connections[clientMax];
    for (int i = 0; i < 10; i++) {
        connections[i] = -1;
    }

    // Create listener
    listen(listener, 5);
    

    // Loop to listen for connections and send messages
    int maxfd, act;
    length = sizeof(s);
    fd_set readfds;

    for(;;) {
        // clear out the current fds
        FD_ZERO(&readfds);

        // add server socket to list
        FD_SET(listener, &readfds);
        maxfd = listener;

        // Add new sockets to readfds
        for (int i = 0; i < clientMax; i++) {
            int tmpsd = connections[i];
            
            // if valid file descriptor, add to readfds
            if (tmpsd > 0) {
                FD_SET(tmpsd, &readfds);
            }
            
            // Find highest file descriptor
            if (tmpsd > maxfd) {
                maxfd = tmpsd;
            }
        }

        // wait for new activity using select
        act = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        
        // Check for select errors
        if (act < 0) {
            char selectErr[] = "There was an error in select.\n";
            write(1, selectErr, strlen(selectErr));
            exit(0);
        }

        // Handle incoming connection
        if (FD_ISSET(listener, &readfds)) {
            int newSock = accept(listener, (struct sockaddr *)&s, 
                                    (socklen_t*)&length);

            // Add new connection to connections list
            for (int i = 0; i < clientMax; i++) {
                if (connections[i] == -1) {
                    connections[i] = newSock;
                    break;
                }
            }
        }

        // Create the buffer for the message to be stored
        int bufferSize = 136;
        char buffer[bufferSize];
        bzero(buffer, bufferSize);

        for (int i = 0; i < clientMax; i++) {
            int fd = connections[i];
            if (FD_ISSET(fd, &readfds)) {
                    
                // Read in message from client
                char head;
                read(fd, &head, 1);
                int charsRead = read(fd, buffer, bufferSize);

                // Exit message handling
                if (head == '2') {
                    close(fd);
                    connections[i] = -1;
                    char goodbye[50];  
                    sprintf(goodbye, "%s has left the server.", buffer);
                    for (int j = 0; j < clientMax; j++) {
                        if (connections[j] != listener && connections[j] > 0) {
                            write(connections[j], goodbye, strlen(goodbye));
                        }
                    }
                    continue;
                } 
                
                // New connection Message
                if (head == '0') {
                    char welcome[50];
                    memset(welcome, '\0', 50);
                    sprintf(welcome, "%s has joined the server!", buffer);
                    welcome[strlen(welcome)] = '\0';
                    
                    // Print welome message to all clients
                    for (int j = 0; j < clientMax; j++) {
                        if (connections[j] != listener && connections[j] > 0) {
                            write(connections[j], welcome, strlen(welcome));
                        }
                    }
                }

                // else, write message to other clients
                else {
                    buffer[charsRead] = '\0';
                    for (int j = 0; j < clientMax; j++) {
                        if (connections[j] != listener && connections[j] > 0
                            && connections[j] != fd) {
                            write(connections[j], buffer, strlen(buffer));
                        }
                    }
                }
            }
        }

    }

    return 0;
}
