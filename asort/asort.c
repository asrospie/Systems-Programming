#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * Alec Rospierski
 * asort
 * Sorts n files using the sort system call in n forked processes
 */ 
int main(int argc, char **argv) {
	if (argc < 2) {
		char *error = "asort <file1> <file2> <file3> ... <filen>\n";
		int test = write(2, error, strlen(error));
		if (!test) {
			return 0;
		}
		return 0;
	}

	// Check to see if SORTED exists
	int dirFD = open("SORTED", O_RDONLY);
	if (dirFD < 0) {
		mkdir("SORTED/", 0777);	
	} else {
		close(dirFD);
	}

    int cpid[argc - 1];
    int stat;

	// Run sort for argc - 1 files
	for (int i = 1; i < argc; i++) {
		if ((cpid[i - 1] = fork()) == 0) {
			close(1);
			close(2);
			open("/dev/null/", O_RDWR);
			open("/dev/null/", O_RDWR);
	        
            // Create the path for the sorted file		
            char path[9 + strlen(argv[i])];
            sprintf(path, "./SORTED/%s", argv[i]);

            // Execute the sort system call using the specified path
			execl("/bin/sort", "sort", argv[i], "-o", path, (char *)  NULL);
                
            // If execl fails, print error message and quit program
            char *terminate = "Unable to call sort system call.\n";
            write(2, terminate, strlen(terminate));
            return 1;
		}
	}

    int counter = 0;
    // Print the results of the sorts
    for (int i = 0; i < argc - 1; i++) {
        waitpid(cpid[i], &stat, 0);

        // Create the buffer that will be printed
        int length = strlen(argv[i + 1]) + 20;
        char buffer[length];
        
        // Check to see if the sort process terminated
        // else quit program for unforseen error
        if (WIFEXITED(stat)) {
            int status = WEXITSTATUS(stat);
        
            // If terminated without errors, print success message
            // else print fail message
            if (status == 0) {
                sprintf(buffer, "%s : success (%d)\n", argv[i + 1], status);
                write(1, buffer, strlen(buffer));
            } else {
                sprintf(buffer, "%s : fail (%d)\n", argv[i + 1], status);
                write(1, buffer, strlen(buffer));
                counter++;
            }
            
        } else {
            char *unforseen = "Sort has failed\n";
            write(2, unforseen, strlen(unforseen));
            return 1; 
        }
    }

    // if all completed successfully, print success message
    // else print the case message
    if (counter == 0) {
        char *finish = "All files successfully sorted.\n";
        write(1, finish, strlen(finish));
    } else {
        char finish[100];
        sprintf(finish, "%d of %d files successfully sorted!\n", argc - 1 - counter, argc - 1);
        write(1, finish, strlen(finish)); 
    }

	return 0;
}
