// C program to implement one side of FIFO
// This side writes first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd, nbyte = 256;
    char *buf1 = (char *)malloc(nbyte * sizeof(char));
    char *buf2 = (char *)malloc(nbyte * sizeof(char));
    size_t num;
    // FIFO file path
    char *myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

    char arr1[80], arr2[80];
    while (1)
    {
        // Open FIFO for write only
        fd = open(myfifo, O_WRONLY);

        // Take an input arr2ing from user.
        // 80 is maximum length
        read(0, buf1, nbyte);

        // Write the input arr2ing on FIFO
        // and close it
        write(fd, buf1, nbyte);
        close(fd);
        memset(buf1, 0, strlen(buf1));

        // Open FIFO for Read only
        fd = open(myfifo, O_RDONLY);

        // Read from FIFO
        read(fd, buf2, nbyte);

        // Print the read message
        printf("User2: %s\n", buf2);
        close(fd);
        memset(buf2, 0, strlen(buf2));

    }
    return 0;
}