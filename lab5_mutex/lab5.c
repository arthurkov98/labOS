#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define PTHREAD_NUM 10
#define BUFFER_SIZE 128

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char buffer[BUFFER_SIZE];

void* writer(void *arg)
{
	time_t t;
	while(1)
	{
		if(!pthread_mutex_lock(&mutex)){
			time(&t);
			sprintf(buffer, "%s\n", ctime(&t));
			pthread_mutex_unlock(&mutex);
			sleep(1);
		}
	}
	pthread_exit(0);
}

void* reader(void *arg)
{
	while(1)
	{
		if(!pthread_mutex_lock(&mutex)){
			printf("%s\n", buffer);
			pthread_mutex_unlock(&mutex);
			sleep(1);
		}
	}
	pthread_exit(0);   
}

int main(void)
{
	pthread_t read_tid;
	pthread_t write_tid;

	if (pthread_create(&write_tid, NULL, writer, NULL) < 0)
		perror("Can't create writer thread\n");
	
	if (pthread_create(&read_tid, NULL, reader, NULL) < 0)
		perror("Can't create reader thread\n");

	if (pthread_join(write_tid, NULL) < 0)
		perror("Join writer failed\n");

	if (pthread_join(read_tid, NULL) < 0)
		perror("Join reader failed\n");
	return 0;
}	