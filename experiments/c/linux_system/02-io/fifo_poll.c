#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_BUF 1024

// Example use of poll()
// Reads from and writes to same fifo, using poll() to watch POLLIN and POLLOUT
// events and act when the fd is ready

char * myfifo = "./this.fifo";

int main()
{
	int fd;
	/* create the FIFO (named pipe) */
	mkfifo(myfifo, 0666);
	/* open in nonblock to avoid waiting */
	fd = open(myfifo, O_RDWR);
	if (fd == -1) {
		printf("main: %d\n", errno);
		return 1;
	}

	// Variables to write return values and read data to 
	int len, ret;
	char input[MAX_BUF], output[MAX_BUF];

	// Define array of pfd(s). Only one file descriptor in this case
	struct pollfd pfds[1]; 
	pfds[0].fd = fd;
	pfds[0].events = POLLIN | POLLOUT; // Watch poll in and out events
	while (strcmp(input, "exit\n")) {
		ret = poll(pfds, 1, 1); 	// 1 fd, 1 second timeout
		if (ret == -1) {
			perror("poll");
			return 1;
		}
		if (!ret) {
			continue;
		}
		if (pfds[0].revents & POLLIN) {
			len = read(fd, output, MAX_BUF);
			printf("Received: %d bytes\n", len);
			output[len] = '\0'; // ignore garbage from previous read
			printf("Received: %s", output);
			continue;
		}
		if (pfds[0].revents & POLLOUT) {
			printf("> ");
			fgets(input, MAX_BUF, stdin);
			write(fd, input, strlen(input));
		}
	}
	close(fd);

	return 0;
}
