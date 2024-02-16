#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define MAX_BUF 1024

char * myfifo = "./this.fifo";

void * read_fifo() {
	// open, read, and display the message from the FIFO
	int fd = open(myfifo, O_RDONLY);
	if (fd == -1) {
		printf("read_fifo: %d", errno);
	}
	char buf[MAX_BUF];
	while (strcmp(buf, "exit")) {
		int len = read(fd, buf, MAX_BUF);
		printf("Received: %d bytes\n", len);
		printf("Received: %s\n", buf);
		if (!strcmp(buf, "exit")) {
			printf("Should exit");
		}
	}
	close(fd);
}

int main()
{
	int fd;
	/* create the FIFO (named pipe) */
	mkfifo(myfifo, 0666);
	/* open in nonblock to avoid waiting */
	pthread_t read_thread;
	pthread_create(&read_thread, NULL, read_fifo, (void *) NULL);
	fd = open(myfifo, O_WRONLY | O_ASYNC);
	if (fd == -1) {
		printf("main: %d\n", errno);
		return 1;
	}
	/* write to the FIFO */
	char input[MAX_BUF];
	while (strcmp(input, "exit")) {
		fgets(input, MAX_BUF, stdin);
		write(fd, input, strlen(input));
	}
	pthread_join(read_thread, NULL);
	close(fd);

	return 0;
}
