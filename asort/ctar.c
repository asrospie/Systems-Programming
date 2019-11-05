#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * Alec Rospierski
 * CS3411
 * ctar tars a group of files into a tar file
 */

typedef struct {
    int magic;              // This must have a value of 0x63746172
    int eop;                // End of file pointer
    int block_count;        // Number of entries in the block in-use
    int file_size[4];       // File size in bytes for files 1..4
    char deleted[4];        // Contains binary one at pos i if ith entry deleted
    int file_name[4];       // pointer to the name of the file
    int next;               // pointer to the next header block
} hdr;

/**
 * ctar
 * creates a tar file for the given arguments
 * argc: the number of arguments
 * argv: the arguments given from command line
 * returns 1 if created -1 if failed
 */ 
int ctar(int argc, char **argv) {
    int fd = open(argv[2], O_RDWR | O_CREAT, 0644);
    
    if (fd < 0) {
        char error[] = "open failed\n";
        write(2, error, strlen(error));
        return -1;
    }
   
    // create stat struct for the archive file
    struct stat archiveStat;
    
    // Find the current header or create one if file empty
    hdr header;
    int header_pos = 0;
    fstat(fd, &archiveStat);
    if (archiveStat.st_size == 0) {
        header.magic = 0x63746172;
        header.block_count = 0;
        header.eop = 0;
        header.next = 0;
        for (int i = 0; i < 4; i++) {
            header.deleted[i] = 0;
            header.file_size[i] = 0;
            header.file_name[i] = 0;
        } 
        write(fd, &header, sizeof(hdr));
    } else {
        // set up basic information
        read(fd, &header, sizeof(hdr));

        // check if archive file is valid
        if (!(header.magic == 0x63746172 && header.eop == archiveStat.st_size)) {
            char error[] = " is not a valid archive file.\n";
            write(2, argv[2], strlen(argv[2]));
            write(2, error, strlen(error));
            return -1;
        }
        
        // fetch the next header
        while(header.block_count == 4) {
            header_pos = lseek(fd, header.next, SEEK_SET);
            read(fd, &header, sizeof(hdr));
        }
    }
    lseek(fd, 0, SEEK_END);

    // Loop through the files to write them to the archive
    int counter = 3;
    while (counter < argc) {

        // Get the file descriptor of the file to be read
        int rfd = open(argv[counter], O_RDONLY);
 
        // See if the file exists to be read
        if (rfd < 0) {
            close(rfd);
            return -1;
        } 

        // fstat the file for data
        struct stat fileStat;
        fstat(rfd, &fileStat);    
        
        // write the name of the file to the archive
        int file_loc = lseek(fd, 0, SEEK_CUR);
        char nl = 0;
        write(fd, argv[counter], strlen(argv[counter]));        
        write(fd, &nl, 1);

        char readBytes[50];

        // write the file to the archive byte by byte
        int numRead;
        while ((numRead = read(rfd, readBytes, 50))) {
            write(fd, readBytes, numRead);
        }        
        
        // update header
        lseek(fd, header_pos, SEEK_SET);
        read(fd, &header, sizeof(hdr));
        header.file_size[header.block_count] = fileStat.st_size;
        header.file_name[header.block_count] = file_loc;
        header.block_count++;
        
        if (header.block_count == 4) {
            // set the next of the previous header
            fstat(fd, &archiveStat);
            header.next = archiveStat.st_size;
            lseek(fd, header_pos, SEEK_SET);
            write(fd, &header, sizeof(hdr));
            
            // create new header and write it
            hdr newHeader;
            newHeader.magic = 0x63746172;
            newHeader.block_count = 0;
            newHeader.eop = 0;
            newHeader.next = 0;
            for (int i = 0; i < 4; i++) {
                newHeader.deleted[i] = 0;
                newHeader.file_size[i] = 0;
                newHeader.file_name[i] = 0;
            }
            header_pos = lseek(fd, 0, SEEK_END);
            write(fd, &newHeader, sizeof(hdr));
        } else {
            lseek(fd, header_pos, SEEK_SET);
            write(fd, &header, sizeof(hdr));
        }
        lseek(fd, 0, SEEK_END);

        // Close the file read in
        close(rfd);
        counter++;
    }

    lseek(fd, 0, SEEK_SET);
    read(fd, &header, sizeof(hdr));
    header.eop = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    write(fd, &header, sizeof(hdr));    

    // Close the archive file
    close(fd);
    return 1;
}

/**
 * delete
 * deletes a file from a tar file
 * archive: the archive file name
 * file: the file to be deleted
 * returns -1 if delete failed, 1 if successful
 */ 
int delete(char *archive, char *file) {
    // test if archive exists
    int fd;
    if ((fd = open(archive, O_RDWR)) < 0) {
        char error[] = " doesn't exist\n";
        write(2, archive, strlen(archive));
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
        write(2, archive, strlen(archive));
        write(2, error, strlen(error));
        return -1;
    }   
    
    // loop through the headers to find file with file name
    int header_pos = 0;
    while (1) {
        
        // loop through current header
        for (int i = 0; i < header.block_count; i++) {
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

            // if the file is found, rewrite the header and return
            if ((strcmp(file, file_name)) == 0) {
                header.deleted[i] = 1;
                lseek(fd, header_pos, SEEK_SET);
                write(fd, &header, sizeof(hdr));
                close(fd);
                return 1;
            }
            free(file_name);
        }

        // fetch the next header block if needed
        if (header.block_count == 4) {
            header_pos = lseek(fd, header.next, SEEK_SET);
            read(fd, &header, sizeof(hdr));
            continue;
        } else {
            break;
        }
    }
    close(fd);

    // File is not found
    write(2, file, strlen(file));
    char error[] = " was not found in the given archive file.\n";
    write(2, error, strlen(error)); 
    return -1;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        char error[] = "ctar -a <archive-file-name> file1 file2 ...\n";
        char errorTwo[] = "ctar -d <archive-file-name> file1\n";
        write(2, error, strlen(error));
        write(2, errorTwo, strlen(errorTwo));
        return -1;
    }

    // Handle the input
    // -a calls ctar
    // -d calls delete
    if (strcmp(argv[1], "-a") == 0) {
        ctar(argc, argv);
    } else if (strcmp(argv[1], "-d") == 0) { 
        delete(argv[2], argv[3]);
    } else { // Flag is not supported
        char error[] = "Flags for ctar are -a and -d\n";
        write(2, error, strlen(error));
        return -1;
    }

    return 0;
}
