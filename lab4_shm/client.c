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

#define BUFFER_SIZE 100
#define MEM_SIZE    50

int main (void)
{
	key_t key;
	int shmid, id = 1576; 
	char *shm, buffer[BUFFER_SIZE];

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
	
	if ((shm = shmat(shmid, NULL, 0)) == (char*)-1)
		perror("connect error");

	while(1)
	{
		sleep(2);
		memcpy(buffer, shm, strlen(shm));
		printf("Клиент читает из разделяемой памяти:\n");
		printf("%s\n\n", buffer);		
	}
	return 0;
}