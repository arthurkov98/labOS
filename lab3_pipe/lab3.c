#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int main(void){
        int fd[2];
        pid_t childpid;
        char writebuffer[80];
        char readbuffer[80];
        time_t t;

        pipe(fd);
        
        if((childpid = fork()) == -1){
                perror("fork");
                return 1;
        }

        if(childpid == 0){
                close(fd[0]);

                time(&t);
                sprintf(writebuffer, "%s\n", ctime(&t));

                write(fd[1], writebuffer, sizeof(writebuffer));
        }

        else{
                close(fd[1]);

                read(fd[0], readbuffer, sizeof(readbuffer));
                printf("Received data: %s", readbuffer);
        }

        return 0;
}