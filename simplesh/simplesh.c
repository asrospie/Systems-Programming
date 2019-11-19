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
        if (string[i] == '\n') {
            string[i] = '\0';
        }
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

char **splitString(char *string, int newLine) {
    int length = maxWordLen(string);
    int words = countChar(string, ' '); 
    char **result = malloc(length * (words + 2));
    char *ptr = strtok(string, " ");
    for (int i = 0; i < words + 1; i++) {
        result[i] = ptr;
        ptr = strtok(0, " "); 
    }
    if (newLine) {
        result[words + 1] = "\n";
    }
    return result;   
}

int waitForProcess(int cpid) {
    int stat;
    waitpid(cpid, &stat, 0);

    // If passed, Return pass message
    // Else, return fail message
    if (WIFEXITED(stat)) {
        int status = WEXITSTATUS(stat);
        printf("STATUS: %d\n", status);
        if (status == 0) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}


/**
 * The main Program
*/
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

            // Reallocate Memory if needed for input
            // TODO
        } while (ch != '\n');
        
        char **cmd = splitString(input, 1);

        // if exit, exit simplesh
        char exitMess[] = "exit";
        int test;
        if ((test = strncmp(cmd[0], exitMess, 4)) == 0) {
            free(cmd);
            free(input);
            break;
        }

        // Handle the commands
        int i = 0;
        while (strcmp(cmd[i], "\n") != 0) {

            // Build the arguments
            char *args = malloc(sizeof(char) * strlen(input));
            args = memset(args, '\0', sizeof(char) * strlen(input));
            while (strcmp(cmd[i], "\n") != 0) {
                args = strncat(args, cmd[i], strlen(cmd[i]));
                char space = ' ';
                args = strncat(args, &space, 1);
                i++;
            }

            char **argArray = splitString(args, 0);
            
            // execute process
            int cpid;
            if ((cpid = fork()) == 0) {
                int ret = execvp(argArray[0], argArray);
                if (ret == -1) {
                    free(argArray);
                    exit(1);
                }
            }

            // Wait for process to finish
            int testProcess = waitForProcess(cpid);
            // Process Failed
            if (testProcess == 0) {
                printf("TEMPORARY FAIL MESSAGE\n");
            }

            free(args);
            free(argArray);
            char newLine = '\n';
            write(1, &newLine, 1);
        }

        // Test print all of the commands
        // int i = 0;
        // while (strcmp(cmd[i], "\n") != 0) {
        //     printf("%s\n", cmd[i]);
        //     i++;
        // } 
        free(cmd);
        free(input);
    }

    // Print good-bye message
    char message[] = "Good-Bye!\n";
    write(1, message, strlen(message));

    return 0;
}
