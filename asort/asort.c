#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	if (argc < 2) {
		char *error = "asort <file1> <file2> <file3> ... <filen>\n";
		int test = write(2, error, strlen(error));
		if (!test) {
			return 0;
		}
		return 0;
	}
	
	for (int i = 1; i < argc; i++) {
		write(1, argv[i], strlen(argv[i]));
		char *newline = "\n";
		write(1, newline, 1);
	}

	// Check to see if SORTED exists
	int dirFD = open("SORTED", O_RDONLY);
	if (dirFD < 0) {
		mkdir("SORTED/", 0777);	
	} else {
		close(dirFD);
	}

	// Run sort for argc - 1 files
	for (int i = 1; i < argc; i++) {
		if (fork() == 0) {
			//close(1);
			//close(2);
			//open("/dev/null/", O_RDWR);
			//open("/dev/null/", O_RDWR);
			
			//char path[strlen(argv[i]) + 7] = "SORTED/";
			char *path = malloc(9 + strlen(argv[i]));
			memset(path, 0, strlen(argv[i]) + 9);
			strncat(path, "./SORTED/", 9);
			strncat(path, argv[i], strlen(argv[i]));
			//char *args[] = {argv[i], "-o", path};
			//int j = execve("sort", args, NULL);
			int j = execl("/bin/sort", "sort", argv[i], "-o", "./SORTED/one.txt", (char *)  NULL);
			printf("%d\n", j);
			printf("%d\n", errno);
		}
		wait(NULL);
	}

	// Print the results of sort

	return 0;
}
