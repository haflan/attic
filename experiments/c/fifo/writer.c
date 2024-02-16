#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";

    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    /* write "Hi" to the FIFO */
    fd = open(myfifo, O_WRONLY | O_ASYNC);
    write(fd, "Hi", sizeof("Hi"));
    printf("Sent hi");
    sleep(1); // sleep until interrupt (SIGIO) 
    write(fd, "you", sizeof("you"));
    printf("Sent you");
    close(fd);

    /* remove the FIFO */
    unlink(myfifo);

    return 0;
}
