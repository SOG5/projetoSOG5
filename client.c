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
    int fd, nbyte = 256, readbuf = 10048;
    char *buf1 = (char *)malloc(nbyte * sizeof(char));
    char *buf2 = (char *)malloc(readbuf * sizeof(char));
    char *agenda = "agenda ";
    size_t num;
    // FIFO file path
    char *myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    while (1)
    {
        memset(buf2, 0, strlen(buf2));
        memset(buf1, 0, strlen(buf1));
        write(1, agenda, sizeof(agenda));
        // Open FIFO for write only
        fd = open(myfifo, O_WRONLY);

        read(0, buf1, nbyte);

        write(fd, buf1, nbyte);

        close(fd);
        // Open FIFO for Read only
        fd = open(myfifo, O_RDONLY);

        // Read from FIFO
        read(fd, buf2, readbuf);
        close(fd);
        // Print the read message
        printf("%s", buf2);

        memset(buf1, 0, strlen(buf2));
    }
    return 0;
}