#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define BUFFER_SIZE 100
#define MEM_SIZE    50

int main (void)
{
	key_t key;
	int shmid, id = 1523, bufLen; 
	char *shm, buffer[BUFFER_SIZE];
	time_t t;
	int semid;
	struct sembuf lock = { 0, -1, SEM_UNDO };
	struct sembuf unlock = { 0, 1, SEM_UNDO };

	unlink("./file");
	key = ftok("./file", id);

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

	if ((semid  = semget(key, 1,  0666)) < 0)
		perror("sem error");
	
	if ((shm = shmat(shmid, NULL, 0)) == (char*)-1)
		perror("connect error");

	while(1)
	{
		semop(semid,&lock,1);
		bufLen = strlen(buffer);
		time(&t);
		sprintf(buffer, "%s\n", ctime(&t));
		printf("\nСервер записывает в файл\n");
		memcpy(shm, buffer, bufLen);
		semop(semid,&unlock,1);	
		sleep(3);	
	}

	return 0;
}