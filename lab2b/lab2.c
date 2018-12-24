#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


#define FILE_SIZE 100000000
#define BLOCK_SIZE 1000
#define NAME_GEN_FILE "gen.txt"
#define NAME_COPY_FILE "copyGen.txt"


void genFile();
void copyFile();
void set_noblock();

int main () {

    pid_t childPid = fork() ;
    int status;
    time_t start = time(0);

    if (childPid) {
        genFile();
        wait(&status);
        time_t end = time(0);
        printf("Total work time: %lld\n", (long long)(end - start));
    } else {
        sleep(1);
        copyFile();
    }

    return 0;
}

void genFile(){

	int fd;
	char * buffer = (char *)calloc(BLOCK_SIZE,sizeof(char));

	int i;
	for (i = 0; i < BLOCK_SIZE; i++)
		buffer[i]  = 'A'+i%26;

	fd = open(NAME_GEN_FILE,O_WRONLY | O_TRUNC | O_CREAT, S_IWRITE|S_IREAD);

	for (i = 0; i < FILE_SIZE; i+=BLOCK_SIZE) 
		write(fd,buffer,BLOCK_SIZE);
	
	close(fd);
	printf("%s generated\n", NAME_GEN_FILE);
}


void copyFile(){

	fd_set readds;
    fd_set writeds;

    struct timeval tv;

    int value;
	int readfd, writefd;

	readfd = open(NAME_GEN_FILE, O_RDONLY);
	writefd = open(NAME_COPY_FILE, O_WRONLY | O_TRUNC | O_CREAT, S_IWRITE|S_IREAD);

	char * buff = (char*)calloc(FILE_SIZE, sizeof(char));

	set_noblock(readfd, 0);
    set_noblock(writefd, 0);

    FD_ZERO(&readds);
    FD_ZERO(&writeds);

    FD_SET(readfd, &readds);
    FD_SET(writefd, &writeds);

    int max_fd = (readfd > writefd) ? readfd : writefd;
    int bytes = -1;

    tv.tv_sec = 3;
    tv.tv_usec = 0;

	if ((value = select(max_fd, &readds, &writeds, NULL, &tv)) == -1) {
        perror("Select()");
    } else if (value) {
        for (;;){
            if (FD_ISSET(readfd, &readds)) {
                bytes = read(readfd, buff, BLOCK_SIZE);
                if (bytes > 0) {
                    write(writefd, buff, BLOCK_SIZE);
                } else {
                    break;
                }
            }
        }
    } 

	close(readfd);
	close(writefd);
	printf("%s copied\n", NAME_GEN_FILE);
}

void set_noblock(int fd, int blocking) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) exit(EXIT_FAILURE);
    if (blocking) flags &= ~O_NONBLOCK;
    else flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}