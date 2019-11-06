













        
            
                } 
            }
            }
            }
            }             
        } 
        } 
    }
    } 
    } 
    }   
 */
 */ 
/**
/**
}
}
			
				}
					}
 * Alec Rospierski
			// allocate memory for the file name
 * argc: the number of arguments
 * argv: the arguments passed in
            break;
            char c;
    char deleted[4];        // Contains binary one at pos i if ith entry deleted
        char error[] = " doesn't exist\n";
                char error[] = "File ";
        char error[] = " is not a valid archive file.\n";
                char errorTwo[] = " already exists. Remove the file and restart.\n";
        char error[] = "utar <archive-file-name>\n";
			char *file_name = malloc(sizeof(char) * size);
                char string[toRead];
					char *tmp = realloc(file_name, sizeof(char) * size);
    // check if archive exists
    // check if archive file is valid
            // Check if file exists else create file
    close(fd);
                close(rfd);
                continue;
            continue;
 * CS3411
            } else {
        } else {
    } else {
			file_name = memset(file_name, 0, size);
					file_name = tmp;
		// find the next header if needed
        for (int i = 0; i < header.block_count; i++) {
        	free(file_name);
				free(file_name);
						free(tmp);
    fstat(fd, &archiveStat); 
            // Get file name
} hdr;
    hdr header;
                if ((actual = header.file_size[i] - j) < 50) {
    if (argc != 2) {
            // if deleted, skip
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        if (header.block_count == 4) {
            if (header.deleted[i] == 1) {
    if (!(header.magic == 0x63746172 && header.eop == archiveStat.st_size)) {
				// if length has been exceeded, reallocate more space
				if (len == size) {
            if ((rfd = open(file_name, O_RDONLY)) >= 0) {
					if (tmp == NULL) {
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
            int actual;
    int block_count;        // Number of entries in the block in-use
    int eop;                // End of file pointer
    int fd;
    int file_name[4];       // pointer to the name of the file
    int file_size[4];       // File size in bytes for files 1..4
            int j = 0;
			int len = 0;
    int magic;              // This must have a value of 0x63746172;
int main(int argc, char **argv) {
    int next;               // pointer to the next header block
            int numRead = 0;
            int rfd;
			int size = 50;
            int toRead = 50;
int utar(int argc, char **argv) {
                j += numRead;
				len++;
        // loop through header block to get to individual files
    // Loop through the headers to reach every file
			// loop until file name is complete
            lseek(fd, header.file_name[i], SEEK_SET);
            lseek(fd, header.next, SEEK_SET);
                numRead = read(fd, string, toRead);
                read(fd, &c, 1);
            read(fd, &c, 1);
            read(fd, &header, sizeof(hdr));
    read(fd, &header, sizeof(hdr));
                return -1;
        return -1;
        return -1;
        return -1;
    return 1;
    return 1;
						return -1;
 * returns -1 if failed, 1 if successful
                rfd = open(file_name, O_RDWR | O_CREAT, 0644);
    // set up basic information
					size += 50;
                strncat(file_name, &c, 1);       
    struct stat archiveStat;
                    toRead = actual;
typedef struct {
 * untars an archive file returned by the corresponding tar program
 * utar
        utar(argc, argv);
 * utar untars a group of files
    while (1) {
            while (c != 0) {
            while (j < header.file_size[i]) {
        write(2, argv[1], strlen(argv[1]));
        write(2, argv[1], strlen(argv[1]));
                write(2, error, strlen(error));
        write(2, error, strlen(error));
        write(2, error, strlen(error));
        write(2, error, strlen(error));
                write(2, errorTwo, strlen(errorTwo));
                write(2, file_name, strlen(file_name));
                write(rfd, string, numRead);
