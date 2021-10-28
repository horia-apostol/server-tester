/* 
 * Apostol Horia-Andrei 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SCFIFO "Server-Client"
#define CSFIFO "Client-Server"

int main() {
	char command[256] = {'\0'}, message[256] = {'\0'};
	int writeBytes, readBytes;
	
	mknod(SCFIFO, S_IFIFO | 0666, 0);
	printf("Waiting for server to respond...\n");
	int csfd = open(CSFIFO, O_WRONLY);
	int scfd = open(SCFIFO, O_RDONLY);
	printf("Connected to server!\n");
	while (fgets(command, 256, stdin)) {
		if ((writeBytes = write(csfd, command, strlen(command))) == -1)
			perror("Client write FIFO error!");
		if ((readBytes = read(scfd, message, 256)) == -1)
			perror("Client read FIFO error!");
		else {
			message[readBytes-1] = '\0';
			if (strcmp(message, "quit") == 0)
				break;
			printf("%s\n", message);
		}
	}
}

//I never use camelCase, underscore bug in Geany
