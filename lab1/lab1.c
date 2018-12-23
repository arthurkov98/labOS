#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	int status;
	pid_t childPid;
	childPid = fork();
	if(childPid > 0){
		printf("PARENT: this parent's process\n");
		printf("PARENT: my PID %d\n", getpid());
		printf("PARENT: I wait child\n");

		while(wait(&status) != childPid);

		printf("PARENT: success\n");

	}
	else{
		printf("CHILD: this child's process\n");
		printf("CHILD: my PID%d\n", getpid());
		return 0;
	}
	return 0;
}