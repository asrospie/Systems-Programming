






















 
   
    
    
    
    
        
        
        
        
        
            
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
 */ 
/**
/**
/**
}
}
}
    // -a calls ctar
 * Alec Rospierski
            // allocate memory for the file name
 * archive: the archive file name
 * argc: the number of arguments
 * argv: the arguments given from command line
            break;
            char c;
    char deleted[4];        // Contains binary one at pos i if ith entry deleted
        char error[] = "ctar -a <archive-file-name> file1 file2 ...\n";
        char error[] = " doesn't exist\n";
        char error[] = "Flags for ctar are -a and -d\n";
            char error[] = " is not a valid archive file.\n";
        char error[] = " is not a valid archive file.\n";
        char error[] = "open failed\n";
        char errorTwo[] = "ctar -d <archive-file-name> file1\n";
    char error[] = " was not found in the given archive file.\n";
            char *file_name = malloc(sizeof(char) * size);
        char nl = 0;
        char readBytes[50];
                    char *tmp = realloc(file_name, sizeof(char) * size);
        // check if archive file is valid
    // check if archive file is valid
                close(fd);
    close(fd);
    close(fd);
            close(rfd);
        close(rfd);
    // Close the archive file
        // Close the file read in
            continue;
        counter++;
            // create new header and write it
 * creates a tar file for the given arguments
    // create stat struct for the archive file
 * CS3411
 * ctar
        ctar(argc, argv);
 * ctar tars a group of files into a tar file
    // -d calls delete
 * delete
        delete(argv[2], argv[3]);
 * deletes a file from a tar file
        } else {
        } else {
    } else {
    } else { // Flag is not supported
    } else if (strcmp(argv[1], "-d") == 0) { 
        // fetch the next header
        // fetch the next header block if needed
    // File is not found
            file_name = memset(file_name, 0, size);
                    file_name = tmp;
 * file: the file to be deleted
    // Find the current header or create one if file empty
            for (int i = 0; i < 4; i++) {
        for (int i = 0; i < 4; i++) {
        for (int i = 0; i < header.block_count; i++) {
            free(file_name);
                        free(tmp);
            fstat(fd, &archiveStat);
    fstat(fd, &archiveStat);
    fstat(fd, &archiveStat);
        fstat(rfd, &fileStat);    
        // fstat the file for data
            // Get file name
        // Get the file descriptor of the file to be read
    // Handle the input
} hdr;
    hdr header;
    hdr header;
            hdr newHeader;
        header.block_count++;
        header.block_count = 0;
            header.deleted[i] = 0;
                header.deleted[i] = 1;
        header.eop = 0;
    header.eop = lseek(fd, 0, SEEK_END);
        header.file_name[header.block_count] = file_loc;
            header.file_name[i] = 0;
        header.file_size[header.block_count] = fileStat.st_size;
            header.file_size[i] = 0;
        header.magic = 0x63746172;
        header.next = 0;
            header.next = archiveStat.st_size;
            header_pos = lseek(fd, 0, SEEK_END);
            header_pos = lseek(fd, header.next, SEEK_SET);
            header_pos = lseek(fd, header.next, SEEK_SET);
    if (archiveStat.st_size == 0) {
    if (argc < 3) {
    if (fd < 0) {
    if ((fd = open(archive, O_RDWR)) < 0) {
        if (header.block_count == 4) {
        if (header.block_count == 4) {
        if (!(header.magic == 0x63746172 && header.eop == archiveStat.st_size)) {
    if (!(header.magic == 0x63746172 && header.eop == archiveStat.st_size)) {
                // if length has been exceeded, reallocate more space
                if (len == size) {
        if (rfd < 0) {
    if (strcmp(argv[1], "-a") == 0) {
            if ((strcmp(file, file_name)) == 0) {
            // if the file is found, rewrite the header and return
                    if (tmp == NULL) {
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
    int block_count;        // Number of entries in the block in-use
    int counter = 3;
int ctar(int argc, char **argv) {
int delete(char *archive, char *file) {
    int eop;                // End of file pointer
    int fd;
    int fd = open(argv[2], O_RDWR | O_CREAT, 0644);
        int file_loc = lseek(fd, 0, SEEK_CUR);
    int file_name[4];       // pointer to the name of the file
    int file_size[4];       // File size in bytes for files 1..4
    int header_pos = 0;
    int header_pos = 0;
            int len = 0;
    int magic;              // This must have a value of 0x63746172
int main(int argc, char **argv) {
    int next;               // pointer to the next header block
        int numRead;
        int rfd = open(argv[counter], O_RDONLY);
            int size = 50;
                len++;
        // loop through current header
    // Loop through the files to write them to the archive
    // loop through the headers to find file with file name
            // loop until file name is complete
        lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    lseek(fd, 0, SEEK_SET);
            lseek(fd, header.file_name[i], SEEK_SET);
                lseek(fd, header_pos, SEEK_SET);
            lseek(fd, header_pos, SEEK_SET);
            lseek(fd, header_pos, SEEK_SET);
        lseek(fd, header_pos, SEEK_SET);
            newHeader.block_count = 0;
                newHeader.deleted[i] = 0;
            newHeader.eop = 0;
                newHeader.file_name[i] = 0;
                newHeader.file_size[i] = 0;
            newHeader.magic = 0x63746172;
            newHeader.next = 0;
                read(fd, &c, 1);
            read(fd, &c, 1);
            read(fd, &header, sizeof(hdr));
            read(fd, &header, sizeof(hdr));
        read(fd, &header, sizeof(hdr));
        read(fd, &header, sizeof(hdr));
    read(fd, &header, sizeof(hdr));
    read(fd, &header, sizeof(hdr));
    return 0;
                        return -1;
                return 1;
            return -1;
            return -1;
        return -1;
        return -1;
        return -1;
        return -1;
        return -1;
    return -1;
    return 1;
 * returns 1 if created -1 if failed
 * returns -1 if delete failed, 1 if successful
        // See if the file exists to be read
            // set the next of the previous header
        // set up basic information
    // set up basic information
                    size += 50;
                strncat(file_name, &c, 1);
    struct stat archiveStat;
    struct stat archiveStat;
        struct stat fileStat;
    // test if archive exists
typedef struct {
        // update header
    while (1) {
            while (c != 0) {
    while (counter < argc) {
        while(header.block_count == 4) {
        while ((numRead = read(rfd, readBytes, 50))) {
        write(2, archive, strlen(archive));
        write(2, archive, strlen(archive));
            write(2, argv[2], strlen(argv[2]));
            write(2, error, strlen(error));
        write(2, error, strlen(error));
        write(2, error, strlen(error));
        write(2, error, strlen(error));
        write(2, error, strlen(error));
        write(2, error, strlen(error));
    write(2, error, strlen(error)); 
        write(2, errorTwo, strlen(errorTwo));
    write(2, file, strlen(file));
        write(fd, argv[counter], strlen(argv[counter]));        
                write(fd, &header, sizeof(hdr));
            write(fd, &header, sizeof(hdr));
            write(fd, &header, sizeof(hdr));
        write(fd, &header, sizeof(hdr));
    write(fd, &header, sizeof(hdr));    
            write(fd, &newHeader, sizeof(hdr));
        write(fd, &nl, 1);
            write(fd, readBytes, numRead);
        // write the file to the archive byte by byte
        // write the name of the file to the archive
