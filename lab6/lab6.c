#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 15
#define MEM_SIZE    16

pthread_mutex_t mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int counter;
char* shm;

void* consume(void *arg)
{
	while (1) {
        if(pthread_mutex_lock(&mutex)) continue;
        while (counter < BUFFER_SIZE) {
            pthread_cond_wait(&cond, &mutex);
        }
        for (;counter;shm[--counter] = ' ') 
        	printf("reduce buffer %s\n", shm);
        printf("reduce buffer %s\n", shm);
        pthread_mutex_unlock(&mutex);
    }
	pthread_exit(0);
}

void* produce(void *arg)
{
	while(1) {
        if(pthread_mutex_lock(&mutex)) continue;
        shm[counter] = '-';
        counter++;
        printf("%d: %s\n", counter, shm);
        if (counter > BUFFER_SIZE-1) {
            printf("Buffer is full\n");
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&mutex);
        usleep(50000);
    }
	pthread_exit(0);   
}

int main(void)
{
	pthread_t consumer;
	pthread_t producer;
	key_t key;
	int shmid, id = 1576; 

	unlink("./file2");
	key = ftok("./file2", id);

	if ((shmid = shmget(key, MEM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		if (errno == EEXIST)
		{		
			if ((shmid = shmget(key, MEM_SIZE,  0666)) < 0)
				perror("access error");
		}else
		{
			perror("access error");
		}
	}
	
	if ((shm = shmat(shmid, NULL, 0)) == (char*)-1)
		perror("connect error");

	shm[BUFFER_SIZE+1] = '\0';

	if (pthread_create(&producer, NULL, produce, NULL) < 0)
		perror("Can't create writer thread\n");
	
	if (pthread_create(&consumer, NULL, consume, NULL) < 0)
		perror("Can't create reader thread\n");

	if (pthread_join(producer, NULL) < 0)
		perror("Join writer failed\n");

	if (pthread_join(consumer, NULL) < 0)
		perror("Join reader failed\n");
	return 0;
}	