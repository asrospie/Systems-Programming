#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * Alec Rospierski
 * CS3411
 * utar untars a group of files
 */

typedef struct {
    int magic;              // This must have a value of 0x63746172;
    int eop;                // End of file pointer
    int block_count;        // Number of entries in the block in-use
    int file_size[4];       // File size in bytes for files 1..4
    char deleted[4];        // Contains binary one at pos i if ith entry deleted
    int file_name[4];       // pointer to the name of the file
    int next;               // pointer to the next header block
} hdr;

/**
 * utar
 * untars an archive file returned by the corresponding tar program
 * argc: the number of arguments
 * argv: the arguments passed in
 * returns -1 if failed, 1 if successful
 */ 
int utar(int argc, char **argv) {
    // check if archive exists
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        char error[] = " doesn't exist\n";
        write(2, argv[1], strlen(argv[1]));
        write(2, error, strlen(error));
        return -1;
    } 

    // set up basic information
    hdr header;
    read(fd, &header, sizeof(hdr));
    struct stat archiveStat;
    fstat(fd, &archiveStat); 

    // check if archive file is valid
    if (!(header.magic == 0x63746172 && header.eop == archiveStat.st_size)) {
        char error[] = " is not a valid archive file.\n";
        write(2, argv[1], strlen(argv[1]));
        write(2, error, strlen(error));
        return -1;
    } 

    // Loop through the headers to reach every file
    while (1) {
        
        // loop through header block to get to individual files
        for (int i = 0; i < header.block_count; i++) {
            // if deleted, skip
            if (header.deleted[i] == 1) {
                continue;
            }

            // Get file name
            lseek(fd, header.file_name[i], SEEK_SET);
            char c;
			int size = 50;
			
			// allocate memory for the file name
			char *file_name = malloc(sizeof(char) * size);
			file_name = memset(file_name, 0, size);
            read(fd, &c, 1);
			int len = 0;

			// loop until file name is complete
            while (c != 0) {
                strncat(file_name, &c, 1);       
				len++;

				// if length has been exceeded, reallocate more space
				if (len == size) {
					size += 50;
					char *tmp = realloc(file_name, sizeof(char) * size);
					if (tmp == NULL) {
						free(tmp);
						return -1;
					}
					file_name = tmp;
				}
                read(fd, &c, 1);
            }             

            int rfd;

            // Check if file exists else create file
            if ((rfd = open(file_name, O_RDONLY)) >= 0) {
                close(rfd);
                char error[] = "File ";
                char errorTwo[] = " already exists. Remove the file and restart.\n";
                write(2, error, strlen(error));
                write(2, file_name, strlen(file_name));
                write(2, errorTwo, strlen(errorTwo));
				free(file_name);
                return -1;
            } else {
                rfd = open(file_name, O_RDWR | O_CREAT, 0644);
            }
            
            int j = 0;
            int toRead = 50;
            int actual;
            int numRead = 0;
            while (j < header.file_size[i]) {
                if ((actual = header.file_size[i] - j) < 50) {
                    toRead = actual;
                } 
                char string[toRead];
                numRead = read(fd, string, toRead);
                write(rfd, string, numRead);
                j += numRead;
            }
        	free(file_name);
        } 

		// find the next header if needed
        if (header.block_count == 4) {
            lseek(fd, header.next, SEEK_SET);
            read(fd, &header, sizeof(hdr));
            continue;
        } else {
            break;
        } 
    }   
    close(fd);

    return 1;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        char error[] = "utar <archive-file-name>\n";
        write(2, error, strlen(error));
        return -1;
    } else {
        utar(argc, argv);
    }
    return 1;
}
