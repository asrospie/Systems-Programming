#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int countChar(char *string, char ch) {
    int count = 0;
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == ch) {
            count++;
        }
    }
    return count;
}

int maxWordLen(char *string) {
    int max = 0;
    int temp = 0;
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == ' ' || string[i] == '\n') {
            if (temp > max) {
                max = temp;
            }
            temp = 0;
        }
        temp++;
    }
    return max;
}

char **splitCommand(char *string) {
    int length = maxWordLen(string);
    int words = countChar(string, ' '); 
    char **result = malloc(length * (words + 2));
    char *ptr = strtok(string, " ");
    for (int i = 0; i < words + 1; i++) {
        result[i] = ptr;
        ptr = strtok(0, " "); 
    }
    result[words + 1] = "\n";
    return result;   
}

int main(int argc, char **argv) {

    // Loop for the command line
    while (1) {
        char *input = malloc(sizeof(char) * 100);
        input = memset(input, '\0', 100);
        int count = 0;
        char ch;

        // Read command until new line
        do {
            read(1, &ch, 1);
            input[count++] = ch;
        } while (ch != '\n');
        
        char **cmd = splitCommand(input);

        // if exit, exit simplesh
        char exit[] = "exit";
        int test;
        if ((test = strncmp(cmd[0], exit, 4)) == 0) {
            free(cmd);
            free(input);
            break;
        }

        // Test print all of the commands
        int i = 0;
        while (strcmp(cmd[i], "\n") != 0) {
            printf("%s\n", cmd[i]);
            i++;
        } 
        free(cmd);
        free(input);
    }

    // Print good-bye message
    char message[] = "Good-Bye!\n";
    write(1, message, strlen(message));

    return 0;
}
