#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void){
        int fd;
        pid_t childpid;
        char writebuffer[80];
        char readbuffer[80];
        time_t t;

        char* path = "./fifo.fifo";

        unlink(path);
        mkfifo(path, 0777);
        
        if((childpid = fork()) == -1){
                perror("fork");
                return 1;
        }

        if(childpid == 0){
                fd = open(path, O_WRONLY);

                time(&t);
                sprintf(writebuffer, "%s\n", ctime(&t));

                write(fd, writebuffer, sizeof(writebuffer));
                close(fd);
        }

        else{
                fd = open(path, O_RDONLY);

                read(fd, readbuffer, sizeof(readbuffer));
                printf("Received data: %s", readbuffer);
                close(fd);
        }

        return 0;
}