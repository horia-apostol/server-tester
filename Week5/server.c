/* 
 * Apostol Horia-Andrei 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <utmp.h>
#include <time.h>

#define SCFIFO "Server-Client"
#define CSFIFO "Client-Server"
#define USRFILE "allowedUsers"

int main() {
	char command[256] = {'\0'};
	int readBytes;

	mknod(CSFIFO, S_IFIFO | 0666, 0);
	
	printf("Waiting for users...\n");
	
	int csfd = open(CSFIFO, O_RDONLY);
	int scfd = open(SCFIFO, O_WRONLY);
	
	printf("User connected to server!\n");
	
	int checkUserLogged = 0;
	
	do {
		if ((readBytes = read(csfd, command, 256)) == -1)
			perror("FIFO read error!");
		else {
			command[readBytes-1] = '\0';
			
			if (strncmp(command, "login : ", 8) == 0){
				int pipemsg[2], socket[2];
				if (socketpair(AF_UNIX, SOCK_STREAM, 0, socket) == -1){
					perror("Socketpair error!");
					exit(1);
				}
				if (pipe(pipemsg) == -1){
					perror("Login message pipe error!");
					exit(1);
				}
				fflush(stdout);
				pid_t pid = fork();
				int status;
				
				if (pid == -1) {
					perror("Login fork error!");
					exit(1);
				}
				else if (pid == 0) {
					
					close(pipemsg[0]);
					close(socket[0]);
					
					char message[256] = {'\0'};
					
					if (checkUserLogged) {
						strcpy(message, "User must logout first!");
						int size = strlen(message)+1;
						write(pipemsg[1], &size, 4);
						write(pipemsg[1], message, size);
						close(pipemsg[1]);
						exit(status);
					}
					char username[256] = {'\0'};
					strcpy(username, command+8);
					FILE * usersfile = fopen(USRFILE, "r");
					char filebuffer[256] = {'\0'};
					int userFound = 0;
					while (fgets(filebuffer, 256, usersfile)) {
						filebuffer[strlen(filebuffer)-1] = '\0';
						if (strcmp(filebuffer, username) == 0) {
							if (checkUserLogged == 0) {
								userFound = 1;
								checkUserLogged = 1;
								if(write(socket[1], &checkUserLogged, 4) == -1)
									perror("Socketpair write error!");
							}
							close(socket[1]);
							break;
						}
					}
					
					if (userFound) 
						strcpy(message, "Login was a success!");
					else 
						strcpy(message, "Login failed!");
					
					int size = strlen(message)+1;
					write(pipemsg[1], &size, 4);
					write(pipemsg[1], message, size);
					close(pipemsg[1]);
					exit(status);
					
				}
				else if (pid > 0){
					close(socket[1]);
					close(pipemsg[1]);
					wait(&status);
					if(read(socket[0], &checkUserLogged, 4) == -1)
						perror("Socketpair read error!");
					close(socket[0]);
					int size;
					char message[256] = {'\0'};
					read(pipemsg[0], &size, 4);
					read(pipemsg[0], message, size);
					close(pipemsg[0]);
					write(scfd, message, size);
				}	
			}
			else if (strcmp(command, "get-logged-users") == 0) {
				int pipemsg[2];
				if (pipe(pipemsg) == -1){
					perror("Get users message pipe error!");
					exit(1);
				}
				fflush(stdout);
				pid_t pid = fork();
				int status;
				if (pid == -1) {
					perror("Get users fork error!");
					exit(1);
				}
				else if (pid == 0) {

					char message[2560] = {'\0'};
					close(pipemsg[0]);
					
					if (checkUserLogged) {
						setutent();
						struct utmp *userData = getutent();
						while(userData) {
							if(userData -> ut_type == USER_PROCESS) {
								time_t secunde = userData -> ut_tv.tv_sec;
								char tempMessage[256] = {'\0'}, dataForOne[256] = {'\0'};
								struct tm timeStructPtr = *localtime(&secunde);
								strftime(tempMessage, 256, "%a %d-%m-%Y %H:%M:%S", &timeStructPtr);
								sprintf(dataForOne,"%s %s %s\n", userData -> ut_user, userData -> ut_host, tempMessage);
								strcat(message, dataForOne);
							}
							userData = getutent();
						}
						message[strlen(message)-1] = '\0';
					}
					else 
						strcpy(message, "User must be logged in!");
					
					int size = strlen(message)+1;
					write(pipemsg[1], &size, 4);
					write(pipemsg[1], message, size);
					close(pipemsg[1]);
					exit(status);
				}
				else if (pid > 0) {
					close(pipemsg[1]);
					wait(&status);
					int size;
					char message[2560] = {'\0'};
					read(pipemsg[0], &size, 4);
					read(pipemsg[0], message, size);
					close(pipemsg[0]);
					write(scfd, message, size);
				}
			}
			else if (strncmp(command, "get-proc-info : ", 16) == 0) {
				int pipemsg[2];
				if (pipe(pipemsg) == -1){
					perror("Proc info message pipe error!");
					exit(1);
				}
				fflush(stdout);
				pid_t pid = fork();
				int status;
				if (pid == -1) {
					perror("Proc info fork error!");
					exit(1);
				}
				else if (pid == 0) {

					char message[2560] = {'\0'};
					close(pipemsg[0]);
					
					if (checkUserLogged) {
						char Pid[256] = {'\0'};
						strcpy(Pid, command+16);
						char Path[256] = {'\0'};
						sprintf(Path,"/%s/%s/%s", "proc", Pid,"status");
						FILE * pidInfoFile = fopen(Path, "r");
						char filebuffer[256] = {'\0'};
						while (fgets(filebuffer, 256, pidInfoFile)) {
							
							char tempMessage[256] = {'\0'};
							filebuffer[strlen(filebuffer)-1] = '\0';
							
							if (strncmp (filebuffer, "Name", 4) == 0) 
								sprintf(tempMessage,"%s\n", filebuffer);
							
							if (strncmp (filebuffer, "State", 5) == 0) 
								sprintf(tempMessage,"%s\n", filebuffer);
							
							if (strncmp (filebuffer, "PPid", 4) == 0) 
								sprintf(tempMessage,"%s\n", filebuffer);
							
							if (strncmp (filebuffer, "Uid", 3) == 0) 
								sprintf(tempMessage,"%s\n", filebuffer);
							
							if (strncmp (filebuffer, "VmSize", 6) == 0) 
								sprintf(tempMessage,"%s\n", filebuffer);
								
							strcat(message, tempMessage);
						}
						message[strlen(message)-1] = '\0';
					}
					else 
						strcpy(message, "User must be logged in!");
					
					int size = strlen(message)+1;
					write(pipemsg[1], &size, 4);
					write(pipemsg[1], message, size);
					close(pipemsg[1]);
					exit(status);
				}
				else if (pid > 0) {
					close(pipemsg[1]);
					wait(&status);
					int size;
					char message[2560] = {'\0'};
					read(pipemsg[0], &size, 4);
					read(pipemsg[0], message, size);
					close(pipemsg[0]);
					write(scfd, message, size);
				}
			}
			else if (strcmp(command, "logout") == 0) {
				if (checkUserLogged){
					write(scfd, "Logged out!", 12);
					checkUserLogged = 0;
				}
				else
					write(scfd, "Not logged in!", 15);
			}
			else if (strcmp(command, "quit") == 0) {
				write(scfd, "quit", 5);
				exit(0);
			}
			else 
				write(scfd, "Wrong command!", 15);
		}
	} while (readBytes);	
	
	return 0;
}

//I never use camelCase, underscore bug in Geany








